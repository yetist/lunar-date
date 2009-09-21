/* vi: set sw=4 ts=4: */
/*
 * lunar-date-private.c: This file is part of liblunar
 *
 * Copyright (C) 2009 yetist <yetist@gmail.com>
 *
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

#include <lunar/lunar-date.h>
#include "lunar-date-private.h"
#include <glib/gi18n-lib.h>

gboolean leap (guint year)
{
	return((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0));
} 

gint _cmp_date (gint month1, gint day1, gint month2, gint day2)
{
	if (month1!=month2) return(month1-month2);
	if (day1!=day2) return(day1-day2);
	return(0);
}

/**
 * year_jieqi:
 *
 * 传回 year 年第 n 个节气的日期(公历).
 * 以小寒为第0个节气. 1900-2100年应该没问题.
 **/
void year_jieqi(int year, int n, char* result)
{

	/* 1900/1/6 02:05:00 小寒  */
	static const double x_1900_1_6_2_5 = 693966.08680556;
	static const int termInfo[] = {
			0	  ,21208 ,42467 ,63836 ,85337 ,107014,
			128867,150921,173149,195551,218072,240693,
			263343,285989,308563,331033,353350,375494,
			397447,419210,440795,462224,483532,504758
	};
	static const char* solar_term_name[] = {
		N_("Xi\307\216oh\303\241n"), N_("D\303\240h\303\241n"), N_("L\303\254ch\305\253n"), N_("Y\307\224shu\307\220"),
		N_("J\304\253ngzh\303\251"), N_("Ch\305\253nf\304\223n"), N_("Q\304\253ngm\303\255ng"), N_("G\307\224y\307\224"), 
		N_("L\303\254xi\303\240"), N_("Xi\307\216om\307\216n"), N_("M\303\241ngzh\303\262ng"), N_("Xi\303\240zh\303\254"), 
		N_("Xi\307\216osh\307\224"), N_("D\303\240sh\307\224"), N_("L\303\254q\304\253u"), N_("Ch\303\271sh\307\224"), 
		N_("B\303\241il\303\262u"), N_("Q\304\253uf\304\223n"), N_("H\303\241nl\303\262u"), N_("Shu\304\201ngji\303\240ng"), 
		N_("L\303\254d\305\215ng"), N_("Xi\307\216oxu\304\233"), N_("D\303\240xu\304\233"), N_("D\305\215ngzh\303\254") 
	};
	static const int mdays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

	int y, m, d, diff;
	unsigned days, _days;

	_days = x_1900_1_6_2_5+365.2422*(year-1900)+termInfo[n]/(60.*24);

	days = 100 * (_days - _days/(3652425L/(3652425L-3652400L)));
	y	 = days/36524; days%=36524;
	m	 = 1 + days/3044;		 /* [1..12] */
	d	 = 1 + (days%3044)/100;    /* [1..31] */

	diff =y*365+y/4-y/100+y/400+mdays[m-1]+d-((m<=2&&((y&3)==0)&&((y%100)!=0||y%400==0))) - _days;

	if(diff > 0 && diff >= d)	 /* ~0.5% */
	{
		if(m == 1)
		{
			--y; m = 12;
			d = 31 - (diff-d);
		}
		else 
		{			 
			d = mdays[m-1] - (diff-d);
			if(--m == 2)
				d += ((y&3)==0) && ((y%100)!=0||y%400==0);
		}
	}
	else
	{
		if((d -= diff) > mdays[m])	  /* ~1.6% */
		{
			if(m == 2)
			{
				if(((y&3)==0) && ((y%100)!=0||y%400==0))
				{
					if(d != 29)
						m = 3, d -= 29;
				}
				else
				{
					m = 3, d -= 28;
				}
			}
			else
			{
				d -= mdays[m];
				if(m++ == 12)
					++y, m = 1;
			}
		}
	}	 

	g_sprintf(result, "%04d%02d%02d %s", y, m, d, _(solar_term_name[n]));
	result = NULL;
}

/* Compute the number of days from the Solar date BYEAR.1.1 */
/* 返回从阳历年1201.1.1日经过的天数 */
//long Solar2Day1(CLDate *d)
glong _date_calc_days_since_reference_year (CLDate *d, GError **error)
{
	glong days, delta;
	int i;

	delta = d->year - REFERENCE_YEAR;
	if (delta<0) 
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_INTERNAL,
				_("Internal error: pick a larger constant for BYEAR."));
		return 0;
	}
	days = delta * 365 + delta / 4 - delta / 100 + delta / 400;
	for (i=1; i< d->month; i++)
		days += days_in_solar_month[i];
	if ((d->month > 2) && leap(d->year))
		days++;
	days += d->day - 1;

	if ((d->month == 2) && leap(d->year))
	{
		if (d->day > 29) 
		{
			g_set_error(error, LUNAR_DATE_ERROR,
					LUNAR_DATE_ERROR_DAY,
					_("Day out of range: \"%d\""),
					d->day);
			return 0;
		}
	}
	else if (d->day > days_in_solar_month[d->month]) 
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_DAY,
				_("Day out of range: \"%d\""),
				d->day);
		return 0;
	}
	return days;
}

/**
 * get_day_of_week:
 * @year: year
 * @month: month of year
 * @day: day of month
 *
 * calc weekday by year, month, day.
 *
 * Return value: week day.
 **/
gint get_day_of_week (gint year, gint month, gint day)
{
	int val;
	if ((month == 1) || (month == 2))
	{
		month += 12;
		year--;
	}

	val = (day + 2*month + 3*(month+1)/5 + year + year/4 - year/100 + year/400 +1 ) % 7;
	return val;
}

/**
 * get_weekth_of_month:
 * @day: 日。
 *
 * 计算是本月第几个(星期几)
 * 
 * Return value: which.
 **/
gint get_weekth_of_month (gint day)
{
	gint a=1;
	while(day -7 >0)
	{
		day = day -7;
		a++;
	}
	return a;
}

int mymemfind(const char *mem, int len, const char *pat, int pat_len)
{
	register int ii;

	len -= pat_len;

	for (ii = 0; ii <= len; ii++) {
		if (mem[ii] == pat[0] && memcmp(&mem[ii], pat, pat_len) == 0) {
			return ii;
		}
	}
	return -1;
}


int
mymemcnt(const char *mem, int len, const char *pat, int pat_len)
{
	register int offset = 0;
	int nfound = 0;

	while (len >= 0) {
		offset = mymemfind(mem, len, pat, pat_len);
		if (offset == -1)
			break;
		mem += offset + pat_len;
		len -= offset + pat_len;
		nfound++;
	}
	return nfound;
}

GString* g_string_replace (GString *string, const gchar* old, const gchar* new, int count)
{
	int nfound, offset;

	int old_len = strlen(old);
	int new_len = strlen(new);

	if (string->len == 0 || (old_len == 0 && new_len == 0) || old_len > string->len)
		goto return_same;

	nfound = (old_len > 0) ? mymemcnt(string->str, string->len, old, old_len) : string->len + 1;
	if (count < 0)
		count = INT_MAX;
	else if (nfound > count)
		nfound = count;
	if (nfound == 0)
		goto return_same;

	GString *new_str = g_string_new("");

	if (old_len > 0) {
		for (; nfound > 0; --nfound) {
			offset = mymemfind(string->str, string->len, old, old_len);
			if (offset == -1)
				break;

			new_str = g_string_append_len(new_str, string->str, offset);
			string->str += offset + old_len;
			string->len -= offset + old_len;

			new_str = g_string_append(new_str, new);
		}
		if (string->len > 0)
			new_str = g_string_append_len(new_str, string->str, string->len);
	}
	else {
		for (;;++string->str, --string->len) {
			g_string_append(new_str, new);
			if (--nfound <= 0) {
				g_string_append_len(new_str, string->str, string->len);
				break;
			}
			g_string_append_c(new_str, *string->str);
		}
	}
	g_string_free(string, FALSE);
	return new_str;

return_same:
	return string;
}

char* num_2_hanzi(int n)
{
	GString *str = g_string_new("");
	int d;
	while (n > 10)
	{
		d = n % 10;
		n = n/10;
		str = g_string_prepend(str, _(hanzi_num[d]));
	}
	str = g_string_prepend(str, _(hanzi_num[n]));

	gchar* ret = g_strdup(g_strstrip(str->str));
	g_string_free(str, TRUE);
	return ret;
}

char* mday_2_hanzi(int n)
{
	GString *str = g_string_new("");
	int d;

	if ((n % 10) == 0)
	{
		n /= 10;
		str = g_string_append(str, _(hanzi_num[n]));
		str = g_string_append(str, _(hanzi_num[10]));
	}
	else if ((n / 10) == 1)
	{
		n = n % 10;
		str = g_string_append(str, _(hanzi_num[10]));
		str = g_string_append(str, _(hanzi_num[n]));
	}
	else if (n > 10)
	{
		d = n % 10;
		n = n/10;
		str = g_string_append(str, _(hanzi_num[n]));
		str = g_string_append(str, _(hanzi_num[10]));
		str = g_string_append(str, _(hanzi_num[d]));
	}
	else
		str = g_string_append(str, _(hanzi_num[n]));

	gchar* ret = g_strdup(g_strstrip(str->str));
	g_string_free(str, TRUE);
	return ret;
}

/*
vi:ts=4:wrap:ai:
*/
