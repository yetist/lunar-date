/* vi: set sw=4 ts=4: */
/*
 * lunar-date.c
 *
 * This file is part of lunar-date.
 *
 * Copyright (C) 2007-2016 yetist <yetist@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *
 * Thanks to the lunar authors:
 * Fung F. Lee	 <lee@umunhum.stanford.edu>
 * Ricky Yeung	 <cryeung@hotmail.com>
 * because algorithm from lunar: http://packages.debian.org/unstable/utils/lunar
 *
 * Special thanks to
 * redwooods <redwooods@gmail.com>
 * who added solar term.
 * */

#if HAVE_CONFIG_H
	#include <config.h>
#endif
#include <glib/gi18n-lib.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <lunar-date/lunar-date.h>
#include <lunar-date/lunar-version.h>
#include "lunar-date-private.h"
#include "lunar-date-resources.h"

/**
 * SECTION:lunar-date
 * @Short_description: Chinese Lunar Date Library
 * @Title: LunarDate
 *
 * The #LunarDate provide Chinese lunar date library.
 */


enum {
	LAST_SIGNAL
};

enum {
	PROP_0,
};

struct _LunarDate
{
	GObject		 object;
	CLDate       *solar;
	CLDate       *lunar;
	CLDate       *lunar2;
	CLDate       *gan;
	CLDate       *zhi;
	CLDate       *gan2;
	CLDate       *zhi2;
	glong	     days;
	guint	     lunar_year_months[NUM_OF_YEARS];
	guint	     lunar_year_days[NUM_OF_YEARS];
	guint	     lunar_month_days[NUM_OF_MONTHS +1];
	GKeyFile     *keyfile;
	GHashTable   *holiday_solar;
	GHashTable   *holiday_lunar;
	GHashTable   *holiday_week;
};

static void lunar_date_set_property  (GObject		   *object,
										 guint			   prop_id,
										 const GValue	  *value,
										 GParamSpec		  *pspec);
static void lunar_date_get_property  (GObject		   *object,
										 guint			   prop_id,
										 GValue			  *value,
										 GParamSpec		  *pspec);
static void _cl_date_make_all_lunar_data(LunarDate *date);
static void lunar_date_init_i18n(void);

G_DEFINE_TYPE (LunarDate, lunar_date, G_TYPE_OBJECT);

static void
lunar_date_class_init (LunarDateClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);

	gobject_class->set_property = lunar_date_set_property;
	gobject_class->get_property = lunar_date_get_property;
}

static void lunar_date_init_holiday (LunarDate *date)
{
	date->holiday_solar = g_hash_table_new(g_str_hash, g_str_equal);
	date->holiday_lunar = g_hash_table_new(g_str_hash, g_str_equal);
	date->holiday_week = g_hash_table_new(g_str_hash, g_str_equal);

	{
		GResource     *resource;
		GBytes        *bytes;
		gconstpointer data;
		GKeyFile      *keyfile;
		gchar         **groups;
		gsize         i = 0, size;

	   	resource = lunar_date_get_resource();
		bytes = g_resource_lookup_data (resource, "/org/moses/lunar/holiday.dat", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
		data = g_bytes_get_data (bytes, &size);

		keyfile = g_key_file_new();
		g_key_file_load_from_data (keyfile, data, size,  G_KEY_FILE_NONE, NULL);
		g_bytes_unref(bytes);

		groups = g_key_file_get_groups (keyfile, &size);
		while(i < size) {
			gchar        buf[127];
			gchar **keys;
			gsize j=0, len;
			gchar *value;
			keys = g_key_file_get_keys (keyfile, groups[i], &len, NULL);
			while(j < len) {
				char *p;
				value = g_key_file_get_locale_string (keyfile, groups[i], keys[j], NULL, NULL);
				//p = strchr(keys[j], '[');
				//p = '\0';
				//keys[4] = '\0';
				//g_print("j=%s   ", p);
				g_print("group:%s, key=%s, value=%s\n", groups[i], keys[j], value);
				j++;
			}
			i++;
		}

	}
}

static void
lunar_date_init (LunarDate *date)
{
	gchar *cfgfile;
	
	lunar_date_init_i18n();

	date->keyfile = g_key_file_new();
	date->solar = g_new0 (CLDate, 1);
	date->lunar = g_new0 (CLDate, 1);
	date->lunar2 = g_new0 (CLDate, 1);
	date->gan	= g_new0 (CLDate, 1);
	date->zhi	= g_new0 (CLDate, 1);
	date->gan2	 = g_new0 (CLDate, 1);
	date->zhi2	 = g_new0 (CLDate, 1);

	lunar_date_init_holiday (date);

	// search in $XDG_CONFIG_HOME/lunar-date/holydat.dat
	cfgfile = g_build_filename(g_get_user_config_dir() , "lunar-date", "holiday.dat", NULL);
	if (!g_file_test(cfgfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
	{
		const gchar* const * langs =  g_get_language_names();
		int i = 0;
		while(langs[i] && langs[i][0] != '\0')
		{
			if (g_str_has_prefix(langs[i], "zh_") && (strlen(langs[i])>= 5))
			{
				gchar *lang = g_strconcat("holiday.", g_strndup(langs[i], 5), NULL);
				g_free(cfgfile);
#ifdef RUN_IN_SOURCE_TREE
				cfgfile = g_build_filename("..", "data", lang, NULL);
				if ( !g_file_test(cfgfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
				{
					g_free(cfgfile);
					cfgfile = g_build_filename("date", lang, NULL);
				}
#else
				cfgfile = g_build_filename(LUNAR_HOLIDAYDIR, lang, NULL);
#endif
				if (g_file_test(cfgfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
				{
					break;
				}
				g_free(lang);
			}
			i++;
		}
	}

	if (!g_key_file_load_from_file(date->keyfile, cfgfile, G_KEY_FILE_KEEP_COMMENTS, NULL))
	{
		g_critical("Format error \"%s\" !!!\n", cfgfile);
	}
	g_free(cfgfile);

	_cl_date_make_all_lunar_data(date);
}

/**
 * lunar_date_new:
 *
 * Allocates a #LunarDate and initializes it. Free the return value with lunar_date_free().
 *
 * Return value: a newly-allocated #LunarDate
 **/
LunarDate*
lunar_date_new (void)
{
	return g_object_new (LUNAR_TYPE_DATE, NULL);
}

static void
lunar_date_set_property (GObject	  *object,
							guint		  prop_id,
							const GValue *value,
							GParamSpec	 *pspec)
{
	LunarDate *date;

	date = LUNAR_DATE (object);

	switch (prop_id)
	{

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
lunar_date_get_property (GObject	  *object,
							guint		  prop_id,
							GValue		 *value,
							GParamSpec	 *pspec)
{
	LunarDate *date;

	date = LUNAR_DATE (object);

	switch (prop_id)
	{

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void _cl_date_calc_lunar(LunarDate *date, GError **error);
static void _cl_date_calc_solar(LunarDate *date, GError **error);
static gint _cl_date_make_lunar_month_days(LunarDate *date, gint year);
static void _cl_date_days_to_lunar (LunarDate *date, GError **error);
static void _cl_date_days_to_solar(LunarDate *date, GError **error);
static void _cl_date_calc_ganzhi(LunarDate *date);
static void _cl_date_calc_bazi(LunarDate *date);
static gint _cl_date_get_bazi_lunar (LunarDate *date);
static void _date_calc_days_since_lunar_year (LunarDate *date, GError **error);

GQuark lunar_date_error_quark (void)
{
	static GQuark quark = 0;

	if (G_UNLIKELY (quark == 0))
		quark = g_quark_from_static_string ("lunar-date-error-quark");

	return quark;
}

/**
 * lunar_date_set_solar_date:
 * @date: a #LunarDate.
 * @year: year to set.
 * @month: month to set.
 * @day: day to set.
 * @hour: hour to set.
 * @error: location to store the error occuring, or NULL to ignore errors.
 *
 * Sets the solar year, month, day and the hour for a #LunarDate.
 **/
void			lunar_date_set_solar_date	  (LunarDate *date,
		GDateYear year,
		GDateMonth month,
		GDateDay day,
		guint8 hour,
		GError **error)
{
	GError *calc_error = NULL;

	if (year < BEGIN_YEAR || year > BEGIN_YEAR+NUM_OF_YEARS || (year == BEGIN_YEAR && month == 1))
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_YEAR,
				_("Year out of range."));
		return;
	}

	if (!g_date_valid_month(month))
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_MONTH,
				_("Month out of range."));
		return;
	}

	if (hour == 24) hour = 0;
	if (hour < 0 || hour > 23)
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_HOUR,
				_("Hour out of range."));
		return;
	}

	date->solar->year = year;
	date->solar->month = month;
	date->solar->day = day;
	date->solar->hour = hour;
	/* 计算农历 */
	_cl_date_calc_lunar(date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	_cl_date_calc_ganzhi(date);
	_cl_date_calc_bazi(date);

}

/**
 * lunar_date_set_lunar_date:
 * @date: a #LunarDate.
 * @year: year to set.
 * @month: month to set.
 * @day: day to set.
 * @hour: hour to set.
 * @isleap: indicate whether the month is a leap month.
 * @error: location to store the error occuring, or #NULL to ignore errors.
 *
 * Sets the lunar year, month, day and the hour for a #LunarDate. If the month is a leap month, you should set the isleap to TRUE.
 **/
void			lunar_date_set_lunar_date	  (LunarDate *date,
		GDateYear year,
		GDateMonth month, 
		GDateDay day,
		guint8 hour,
		gboolean isleap,
		GError **error)
{
	GError *calc_error = NULL;

	if (!(year>=BEGIN_YEAR && year< BEGIN_YEAR+NUM_OF_YEARS))
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_YEAR,
				_("Year out of range."));
		return;
	}

	if (!g_date_valid_month(month))
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_MONTH,
				_("Month out of range."));
		return;
	}

	if (hour == 24) hour = 0;
	if (hour < 0 || hour > 23)
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_HOUR,
				_("Hour out of range."));
		return;
	}

	date->lunar->year = year;
	date->lunar->month = month;
	date->lunar->day = day;
	date->lunar->hour = hour;
	date->lunar->isleap = isleap;
	/* 计算公历 */
	_cl_date_calc_solar(date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	_cl_date_calc_ganzhi(date);
	_cl_date_calc_bazi(date);
}

/**
 * lunar_date_get_jieri:
 * @date: a #LunarDate
 * @delimiter: used to join the holidays.
 *
 * Returns the all holiday of the date, joined with the delimiter. The date must be valid.
 *
 * Return value:  a newly-allocated holiday string of the date.
 * This can be changed in  <ulink url="http://www.freedesktop.org/wiki/Specifications/basedir-spec">$XDG_CONFIG_HOME</ulink>/liblunar/hodiday.dat file.
 *
 **/
gchar*		lunar_date_get_jieri		  (LunarDate *date, const gchar *delimiter)
{
	GString* jieri;

	gint weekday, weekth;
	gchar* str_day;

	jieri=g_string_new("");

	if (date->keyfile != NULL)
	{

		if (g_key_file_has_group(date->keyfile, "LUNAR"))
		{
			str_day = g_strdup_printf("%02d%02d", date->lunar->month, date->lunar->day);
			if (g_key_file_has_key (date->keyfile, "LUNAR", str_day, NULL))
			{
				char* freeme;
				jieri=g_string_append(jieri, delimiter);
				freeme = g_key_file_get_value (date->keyfile, "LUNAR", str_day, NULL);
				jieri=g_string_append(jieri, freeme);
				g_free(freeme);
			}
			g_free(str_day);
		}

		if (g_key_file_has_group(date->keyfile, "SOLAR"))
		{
			str_day = g_strdup_printf("%02d%02d", date->solar->month, date->solar->day);
			if (g_key_file_has_key (date->keyfile, "SOLAR", str_day, NULL))
			{
				char *freeme;
				jieri=g_string_append(jieri, delimiter);
				freeme = g_key_file_get_value (date->keyfile, "SOLAR", str_day, NULL);
				jieri=g_string_append(jieri, freeme);
				g_free(freeme);
			}
			g_free(str_day);
		}

		weekday = get_day_of_week ( date->solar->year, date->solar->month, date->solar->day);
		weekth = get_weekth_of_month ( date->solar->day);
		if (g_key_file_has_group(date->keyfile, "WEEK"))
		{
			str_day = g_strdup_printf("%02d%01d%01d", date->solar->month, weekth, weekday);
			if (g_key_file_has_key (date->keyfile, "WEEK", str_day, NULL))
			{
				char *freeme;
				jieri=g_string_append(jieri, delimiter);
				freeme = g_key_file_get_value (date->keyfile, "WEEK", str_day, NULL);
				jieri=g_string_append(jieri, freeme);
				g_free(freeme);
			}
			g_free(str_day);
		}

		//jie2qi4
		static char str_jq[24][20] = {'9'};
		int i;
		char yc[5] = {str_jq[0][0], str_jq[0][1], str_jq[0][2], str_jq[0][3], '\0'};
		if(atoi(yc) != date->solar->year)
		{
			for(i=0; i<24; i++)
			{
				year_jieqi(date->solar->year, i, str_jq[i]);
			}
		}
		str_day = g_strdup_printf("%04d%02d%02d", date->solar->year, date->solar->month, date->solar->day);
		for (i=0; i<24; i++)
		{
			gchar** jq_day;
			jq_day = g_strsplit(str_jq[i], " ", 2);
			if (g_ascii_strcasecmp(jq_day[0], str_day) == 0)
			{
				jieri=g_string_append(jieri, delimiter);
				jieri=g_string_append(jieri, jq_day[1]);
			}
			g_strfreev(jq_day);
		}
		g_free(str_day);
	}

	gchar* oo = g_strdup(g_strstrip(jieri->str));
	g_string_free(jieri, TRUE);
	return oo;
}

/**
 * lunar_date_strftime:
 * @date: a #LunarDate
 * @format: specify the output format.
 *
 * 使用给定的格式来输出字符串。类似于strftime的用法。可使用的格式及输出如下：
 *
 * %(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时		公历：大写->二OO八年一月二十一日
 *
 * %(year)年%(month)月%(day)日%(hour)时		公历：小写->2008年1月21日
 *
 * %(NIAN)年%(YUE)月%(RI)日%(SHI)时			阴历：大写->丁亥年腊月十四日，(月份前带"闰"表示闰月)
 *
 * %(nian)年%(yue)月%(ri)日%(shi)时			阴历：小写->2007年12月14日，(月份前带"*"表示闰月)
 *
 * %(Y60)年%(M60)月%(D60)日%(H60)时			干支：大写->丁亥年癸丑月庚申日
 *
 * %(Y8)年%(M8)月%(D8)日%(H8)时				八字：大写->丁亥年癸丑月庚申日
 *
 * %(shengxiao)								生肖：猪
 * %(jieri)									节日(节日、纪念日、节气等)：立春
 *
 * 使用%(jieri)时，如果此日没有节日或节气，那么将为空。
 * 支持自定义节日，只要按照格式修改 <ulink url="http://www.freedesktop.org/wiki/Specifications/basedir-spec">$XDG_CONFIG_HOME</ulink>/liblunar/hodiday.dat 文件即可。
 *
 * Return value: a newly-allocated output string, nul-terminated
 **/
gchar* lunar_date_strftime (LunarDate *date, const char *format)
{
	gchar *s, *tmp;
	gchar* t1;
	gchar tmpbuf[32];

	gchar* str = g_strdup(format);

	//GString *str = g_string_new(format);

	//solar-upper case
	if (strstr(format, "%(YEAR)") != NULL)
	{
		num_2_hanzi(date->solar->year, tmpbuf, sizeof(tmpbuf));
		t1 = str_replace(str, "\%\\(YEAR\\)", tmpbuf);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(MONTH)") != NULL)
	{
		mday_2_hanzi(date->solar->month, tmpbuf, sizeof(tmpbuf));
		t1 = str_replace(str, "\%\\(MONTH\\)", tmpbuf);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(DAY)") != NULL)
	{
		mday_2_hanzi(date->solar->day, tmpbuf, sizeof(tmpbuf));
		t1 = str_replace(str, "\%\\(DAY\\)", tmpbuf);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(HOUR)") != NULL)
	{
		mday_2_hanzi(date->solar->hour, tmpbuf, sizeof(tmpbuf));
		t1 = str_replace(str, "\%\\(HOUR\\)", tmpbuf);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//solar-lower case
	if (strstr(format, "%(year)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->solar->year);

		t1 = str_replace(str, "\%\\(year\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(month)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->solar->month);
		t1 = str_replace(str, "\%\\(month\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(day)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->solar->day);
		t1 = str_replace(str, "\%\\(day\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(hour)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->solar->hour);
		t1 = str_replace(str, "\%\\(hour\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//lunar-upper case
	if (strstr(format, "%(NIAN)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->year]), _(zhi_list[date->zhi->year]));
		t1 = str_replace(str, "\%\\(NIAN\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(YUE)") != NULL)
	{
		if (date->lunar->isleap)
			tmp = g_strdup_printf("%s%s", _("R\303\271n"), _(lunar_month_list[date->lunar->month-1]));
		else
			tmp = g_strdup_printf("%s", _(lunar_month_list[date->lunar->month-1]));
		t1 = str_replace(str, "\%\\(YUE\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(RI)") != NULL)
	{
		t1 = str_replace(str, "\%\\(RI\\)", _(lunar_day_list[date->lunar->day-1]));
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(SHI)") != NULL)
	{
		t1 = str_replace(str, "\%\\(SHI\\)", _(zhi_list[date->zhi->hour]));
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//lunar-lower case
	if (strstr(format, "%(nian)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->lunar->year);
		t1 = str_replace(str, "\%\\(nian\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(yue)") != NULL)
	{
		if (date->lunar->isleap)
			tmp = g_strdup_printf("*%d", date->lunar->month);
		else
			tmp = g_strdup_printf("%d", date->lunar->month);
		t1 = str_replace(str, "\%\\(yue\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(ri)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->lunar->day);
		t1 = str_replace(str, "\%\\(ri\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(shi)") != NULL)
	{
		tmp = g_strdup_printf("%d", date->zhi->hour);
		t1 = str_replace(str, "\%\\(shi\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//ganzhi
	if (strstr(format, "%(Y60)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->year]), _(zhi_list[date->zhi->year]));
		t1 = str_replace(str, "\%\\(Y60\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(M60)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->month]), _(zhi_list[date->zhi->month]));
		t1 = str_replace(str, "\%\\(M60\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(D60)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->day]), _(zhi_list[date->zhi->day]));
		t1 = str_replace(str, "\%\\(D60\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(H60)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->hour]), _(zhi_list[date->zhi->hour]));
		t1 = str_replace(str, "\%\\(H60\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//bazi
	if (strstr(format, "%(Y8)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->year]), _(zhi_list[date->zhi2->year]));
		t1 = str_replace(str, "\%\\(Y8\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(M8)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->month]), _(zhi_list[date->zhi2->month]));
		t1 = str_replace(str, "\%\\(M8\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	if (strstr(format, "%(D8)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->day]), _(zhi_list[date->zhi2->day]));
		t1 = str_replace(str, "\%\\(D8\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}
	/* 子时: 23点 --凌晨1 点前... */
	if (strstr(format, "%(H8)") != NULL)
	{
		tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->hour]), _(zhi_list[date->zhi2->hour]));
		t1 = str_replace(str, "\%\\(H8\\)", tmp);
		g_free(tmp);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//shengxiao
	if (strstr(format, "%(shengxiao)") != NULL)
	{
		t1 = str_replace(str, "\%\\(shengxiao\\)", _(shengxiao_list[date->zhi->year]));
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	//jieri
	if (strstr(format, "%(jieri)") != NULL)
	{
		gchar bufs[128];
		gchar *tmp;
		tmp = lunar_date_get_jieri(date, " ");
		/* 将格式化后的输出限制为3个汉字或4个ascii字符
		 * 以限制日历的示宽度
		 * 如果不是用在日历上，请使用lunar_date_get_jieri()得到输出
		 * */
		if (strstr(tmp, " " ) != NULL)
		{
			char** buf = g_strsplit(tmp, " ", -1);
			if (g_utf8_validate(*buf, -1, NULL))
				g_utf8_strncpy(bufs, *buf, 3);
			else
			{
				strncpy(bufs, *buf, 4);
				bufs[4]= '\0';
			}
			g_strfreev(buf);
		}
		else
		{
			if (g_utf8_validate(tmp, -1, NULL))
				g_utf8_strncpy(bufs, tmp, 3);
			else
			{
				strncpy(bufs, tmp, 4);
				bufs[4]= '\0';
			}
		}
		g_free(tmp);
		t1 = str_replace(str, "\%\\(jieri\\)", bufs);
		g_free(str); str=g_strdup(t1); g_free(t1);
	}

	return str;
	//return g_string_free(str, FALSE);
}

/**
 * lunar_date_free:
 * @date: a #LunarDate
 *
 * Frees a #LunarDate returned from lunar_date_new().
 **/
void			lunar_date_free					  (LunarDate *date)
{
	g_return_if_fail (date != NULL);
	g_free(date->solar);
	g_free(date->lunar);
	g_free(date->lunar2);
	g_free(date->gan);
	g_free(date->zhi);
	g_free(date->gan2);
	g_free(date->zhi2);
	g_key_file_free(date->keyfile);
}

static void _cl_date_calc_lunar(LunarDate *date, GError **error)
{
	glong days;
	GError *calc_error = NULL;
	CLDate *d;

	date->days = _date_calc_days_since_reference_year(date->solar, &calc_error) ;
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	date->days -=  _date_calc_days_since_reference_year(&first_solar_date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	/* A lunar day begins at 11 p.m. */
	if (date->solar->hour == 23)
		date->days ++;

	_cl_date_days_to_lunar(date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	date->lunar->hour = date->solar->hour;
}

static void _cl_date_calc_solar(LunarDate *date, GError **error)
{
	GError *calc_error = NULL;

	_date_calc_days_since_lunar_year(date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	_cl_date_days_to_solar(date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	date->solar->hour = date->lunar->hour;
}

//TODO
static void _cl_date_calc_ganzhi(LunarDate *date)
{
	int	year, month;

	year = date->lunar->year - first_lunar_date.year;
	month = year * 12 + date->lunar->month - 1;   /* leap months do not count */

	date->gan->year = (first_gan_date.year + year) % 10;
	date->zhi->year = (first_zhi_date.year + year) % 12;
	date->gan->month = (first_gan_date.month + month) % 10;
	date->zhi->month = (first_zhi_date.month + month) % 12;
	date->gan->day = (first_gan_date.day + date->days) % 10;
	date->zhi->day = (first_zhi_date.day + date->days) % 12;
	date->zhi->hour = ((date->lunar->hour + 1) / 2) % 12;
	date->gan->hour = (date->gan->day * 12 + date->zhi->hour) % 10;
}

static void _cl_date_calc_bazi(LunarDate *date)
{
	int	year, month;

	_cl_date_get_bazi_lunar(date);

	year = date->lunar2->year - first_lunar_date.year;
	month = year * 12 + date->lunar2->month - 1;   /* leap months do not count */

	date->gan2->year = (first_gan_date.year + year) % 10;
	date->zhi2->year = (first_zhi_date.year + year) % 12;
	date->gan2->month = (first_gan_date.month + month) % 10;
	date->zhi2->month = (first_zhi_date.month + month) % 12;
	date->gan2->day = (first_gan_date.day + date->days) % 10;
	date->zhi2->day = (first_zhi_date.day + date->days) % 12;
	date->zhi2->hour = ((date->lunar2->hour + 1) / 2) % 12;
	date->gan2->hour = (date->gan2->day * 12 + date->zhi2->hour) % 10;
}

/* Compute offset days of a lunar date from the beginning of the table */
static void _date_calc_days_since_lunar_year (LunarDate *date, GError **error)
{
	int year, i, m, leap_month;

	date->days = 0;
	year = date->lunar->year - first_lunar_date.year;
	for (i=0; i<year; i++)
		date->days += date->lunar_year_days[i];

	leap_month = _cl_date_make_lunar_month_days(date, year);
	if ((date->lunar->isleap) && (leap_month!=date->lunar->month))
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_LEAP,
				_("%d is not a leap month in year %d.\n"), 
				date->lunar->month, date->lunar->year);
		return;
	}
	for (m=1; m<date->lunar->month; m++)
		date->days+= date->lunar_month_days[m];
	if (leap_month 
			&& ((date->lunar->month>leap_month) 
				|| (date->lunar->isleap && (date->lunar->month==leap_month))
			   ))
		date->days += date->lunar_month_days[m++];
	date->days += date->lunar->day - 1;

	if (date->lunar->day > date->lunar_month_days[m]) 
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_DAY,
				_("Day out of range: \"%d\""),
				date->lunar->day);
		return;
	}
}

static void _cl_date_days_to_lunar (LunarDate *date, GError **error)
{
	int i, m, nYear, leap_month;

	glong offset = date->days;
	for (i=0; i<NUM_OF_YEARS && offset > 0; i++)
		offset -= date->lunar_year_days[i];
	if (offset <0)
		offset += date->lunar_year_days[--i];
	if (i==NUM_OF_YEARS) 
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_DAY,
				_("Year out of range. \"%d\""),
				date->solar->year);
		return;
	}
	date->lunar->year = i + first_lunar_date.year;

	leap_month = _cl_date_make_lunar_month_days(date, i);
	for (m=1; m<=NUM_OF_MONTHS && offset >0; m++)
		offset -= date->lunar_month_days[m];
	if (offset <0)
		offset += date->lunar_month_days[--m];

	date->lunar->isleap = FALSE;	/* don't know leap or not yet */

	if (leap_month>0)	/* has leap month */
	{
		/* if preceeding month number is the leap month,
		   this month is the actual extra leap month */
		date->lunar->isleap = (gboolean)(leap_month == (m - 1));

		/* month > leap_month is off by 1, so adjust it */
		if (m > leap_month) --m;
	}

	date->lunar->month = m;
	date->lunar->day = offset + 1;
}

static void _cl_date_days_to_solar(LunarDate *date, GError **error)
{
	GError *calc_error = NULL;
	gint	adj, i, m, days;
	glong offset;

	adj = (date->lunar->hour == 23)? -1 : 0;
	offset = date->days;
	offset += adj;
	offset -= _date_calc_days_since_reference_year(&first_lunar_date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	offset += _date_calc_days_since_reference_year(&first_solar_date, &calc_error);
	if (calc_error != NULL)
	{
		g_propagate_error (error, calc_error);
		calc_error = NULL;
		return;
	}
	/* offset is now the number of days from SolarFirstDate.year.1.1 */

	for (i=first_solar_date.year;
			(i<first_solar_date.year+NUM_OF_YEARS) && (offset > 0);	 i++) 
		offset -= 365 + leap(i);
	if (offset<0)
	{
		--i;	/* LeapYear is a macro */
		offset += 365 + leap(i);
	}
	if (i==(first_solar_date.year + NUM_OF_YEARS)) 
	{
		g_set_error(error, LUNAR_DATE_ERROR,
				LUNAR_DATE_ERROR_DAY,
				_("Year out of range. \"%d\""),
				i);
		return;
	}
	date->solar->year = i;

	/* assert(offset<(365+LeapYear(i))); */
	for (m=1; m<=12; m++)
	{
		days = days_in_solar_month[m];
		if ((m==2) && leap(i))	/* leap February */
			days++;
		if (offset<days)
		{
			date->solar->month = m;
			date->solar->day = offset + 1;
			return;
		}
		offset -= days;
	}
}

static void _cl_date_make_all_lunar_data(LunarDate *date)
{
	gint year, i, leap;
	long code;

	for (year = 0; year < NUM_OF_YEARS; year++)
	{
		code = years_info[year];
		leap = code & 0xf;
		date->lunar_year_days[year] = 0;
		if (leap != 0)
		{
			i = (code >> 16) & 0x1;
			date->lunar_year_days[year] += days_in_lunar_month[i];
		}
		code >>= 4;
		for (i = 0; i < NUM_OF_MONTHS-1; i++)
		{
			date->lunar_year_days[year] += days_in_lunar_month[code&0x1];
			code >>= 1;
		}
		date->lunar_year_months[year] = 12;
		if (leap != 0) 
			date->lunar_year_months[year]++;
	}
}

/* Compute the days of each month in the given lunar year */
static gint _cl_date_make_lunar_month_days(LunarDate *date, gint year)
{
	int i, leap_month;
	long code;

	code = years_info[year];
	leap_month = code & 0xf;
	code >>= 4;
	if (leap_month == 0)
	{
		date->lunar_month_days[NUM_OF_MONTHS] = 0;
		for (i = NUM_OF_MONTHS-1; i >= 1; i--)
		{
			date->lunar_month_days[i] = days_in_lunar_month[code&0x1];
			code >>= 1;
		}
	}
	else
	{
		/* 
		   There is a leap month (run4 yue4) L in this year.
		   mday[1] contains the number of days in the 1-st month;
		   mday[L] contains the number of days in the L-th month;
		   mday[L+1] contains the number of days in the L-th leap month;
		   mday[L+2] contains the number of days in the L+1 month, etc.

		   cf. years_info[]: info about the leap month is encoded differently.
		   */
		i = (years_info[year] >> 16) & 0x1;
		date->lunar_month_days[leap_month+1] = days_in_lunar_month[i];
		for (i = NUM_OF_MONTHS; i >= 1; i--)
		{
			if (i == leap_month+1) 
				i--;
			date->lunar_month_days[i] = days_in_lunar_month[code&0x1];
			code >>= 1;
		}
	}
	return leap_month;
}

/* Compare two dates and return <,=,> 0 if the 1st is <,=,> the 2nd */
static gint _cl_date_get_bazi_lunar (LunarDate *date)
{
	int m, flag;

	if (date->solar->month==1)
	{
		flag = _cmp_date(date->solar->month, date->solar->day,
				1, fest[date->solar->year - first_solar_date.year - 1][11]);
		if (flag<0) 
			date->lunar2->month = 11;
		else if (flag>0) 
			date->lunar2->month = 12;
		date->lunar2->year = date->solar->year - 1;
		return(flag==0);
	}
	for (m=2; m<=12; m++)
	{
		flag = _cmp_date(date->solar->month, date->solar->day,
				m, fest[date->solar->year - first_solar_date.year][m-2]);
		if (flag==0) m++;
		if (flag<=0) break;
	}
	date->lunar2->month = (m-2) % 12;
	date->lunar2->year = date->solar->year;
	if ((date->lunar2->month)==0)
	{
		date->lunar2->year = date->solar->year - 1;
		date->lunar2->month = 12;
	}
	return(flag==0);
}

static void lunar_date_init_i18n(void)
{
  static gboolean _lunar_calendar_gettext_initialized = FALSE;

  if (!_lunar_calendar_gettext_initialized)
    {
	  bindtextdomain (GETTEXT_PACKAGE, LUNAR_DATE_LOCALEDIR);
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
      bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif
      _lunar_calendar_gettext_initialized = TRUE;
    }
}
