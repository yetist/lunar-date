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
#include <lunar-date/lunar-date-version.h>
#include "lunar-date-private.h"
#include "lunar-date-utils.h"
#include "lunar-date-resources.h"

/**
 * SECTION:lunar-date
 * @Short_description: Chinese Lunar Date Library
 * @include: lunar-date/lunar-date.h
 * @Title: LunarDate
 * @stability: Stable
 *
 * The #LunarDate provide Chinese lunar date library.
 */

/**
 * LunarDateError:
 * @LUNAR_DATE_ERROR_INTERNAL: internal error.
 * @LUNAR_DATE_ERROR_YEAR: year setup error.
 * @LUNAR_DATE_ERROR_MONTH: month setup error.
 * @LUNAR_DATE_ERROR_DAY: day setup error.
 * @LUNAR_DATE_ERROR_HOUR: hour setup error.
 * @LUNAR_DATE_ERROR_LEAP: isleap setup error.
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
static gchar* lunar_date_get_cal_holiday (LunarDate *date, gint max_len);

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
	GResource     *resource;
	GBytes        *bytes;
	gconstpointer data;
	GKeyFile      *keyfile;
	gchar         **groups, **keys;
	gsize         i=0, j=0;
	gsize         size, len;
	gchar         *value, *p;

	date->holiday_solar = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	date->holiday_lunar = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	date->holiday_week = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	resource = lunar_date_get_resource();
	bytes = g_resource_lookup_data (resource, "/org/moses/lunar/holiday.dat", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
	data = g_bytes_get_data (bytes, &size);

	keyfile = g_key_file_new();
	g_key_file_load_from_data (keyfile, data, size,  G_KEY_FILE_NONE, NULL);

	groups = g_key_file_get_groups (keyfile, &size);
	while(i < size) {
		j = 0;
		keys = g_key_file_get_keys (keyfile, groups[i], &len, NULL);
		while(j < len) {
			value = g_key_file_get_locale_string (keyfile, groups[i], keys[j], NULL, NULL);
			p = strchr(keys[j], '[');
			*p = '\0';
			if (g_ascii_strcasecmp(groups[i], "SOLAR") == 0 ) {
				g_hash_table_insert(date->holiday_solar, g_strdup(keys[j]), g_strdup(value));
			}else if (g_ascii_strcasecmp(groups[i], "LUNAR") == 0 ) {
				g_hash_table_insert(date->holiday_lunar, g_strdup(keys[j]), g_strdup(value));
			}else if (g_ascii_strcasecmp(groups[i], "WEEK") == 0 ) {
				g_hash_table_insert(date->holiday_week, g_strdup(keys[j]), g_strdup(value));
			}
			g_free(value);
			j++;
		}
		i++;
		g_strfreev (keys);
	}
	g_strfreev (groups);
	g_key_file_free(keyfile);
	g_bytes_unref(bytes);
}

static void
lunar_date_init (LunarDate *date)
{
	lunar_date_init_i18n();

	date->solar = g_new0 (CLDate, 1);
	date->lunar = g_new0 (CLDate, 1);
	date->lunar2 = g_new0 (CLDate, 1);
	date->gan	= g_new0 (CLDate, 1);
	date->zhi	= g_new0 (CLDate, 1);
	date->gan2	 = g_new0 (CLDate, 1);
	date->zhi2	 = g_new0 (CLDate, 1);

	lunar_date_init_holiday (date);

	_cl_date_make_all_lunar_data(date);
}

/**
 * lunar_date_new:
 *
 * Allocates a #LunarDate and initializes it. Free the return value with lunar_date_free().
 *
 * Returns: a newly-allocated #LunarDate
 *
 * Since: 2.4.0
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
 *
 * Since: 2.4.0
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
		g_set_error(error, LUNAR_DATE_ERROR, LUNAR_DATE_ERROR_YEAR, _("Year out of range.(from 1900 to 2050)."));
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
	if (hour > 23)
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
 *
 * Since: 2.4.0
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
	if (hour > 23)
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
 * lunar_date_set_solar_holiday:
 * @date: a #LunarDate
 * @month: Solar month of the holiday.
 * @day: Solar day of the holiday.
 * @holiday: custom holiday string.
 *
 * Add new holiday by solar.
 *
 * Since: 3.0.0
 **/
void lunar_date_set_solar_holiday(LunarDate *date, GDateMonth month, GDateDay day, const gchar *holiday)
{
	gchar key[10];

	g_return_if_fail( date != NULL);
	g_return_if_fail( holiday != NULL);

	g_snprintf(key, sizeof(key), "%02u%02d", month, day);
	if (g_hash_table_lookup(date->holiday_solar, key) != NULL) {
		g_hash_table_remove(date->holiday_solar, key);
	}
	g_hash_table_insert(date->holiday_solar, g_strdup(key), g_strdup(holiday));
}

/**
 * lunar_date_set_lunar_holiday:
 * @date: a #LunarDate
 * @month: Lunar month of the holiday.
 * @day: Lunar day of the holiday.
 * @holiday: custom holiday string.
 *
 * Add new holiday by lunar.
 *
 * Since: 3.0.0
 **/
void lunar_date_set_lunar_holiday(LunarDate *date, GDateMonth month, GDateDay day, const gchar *holiday)
{
	gchar key[10];
	g_return_if_fail( date != NULL);
	g_return_if_fail( holiday != NULL);

	g_snprintf(key, sizeof(key), "%02u%02d", month, day);
	if (g_hash_table_lookup(date->holiday_lunar, key) != NULL) {
		g_hash_table_remove(date->holiday_lunar, key);
	}
	g_hash_table_insert(date->holiday_lunar, g_strdup(key), g_strdup(holiday));
}

/**
 * lunar_date_set_week_holiday:
 * @date: a #LunarDate
 * @month: Month of the holiday.
 * @week_of_month: which week of the month, the first week is 1, range is 1-5.
 * @day_of_week: day of the week, 0 for Sunday, 6 for Saturday, range is 0-6.
 * @holiday: custom holiday string.
 *
 * Add new holiday by week.
 *
 * Since: 3.0.0
 **/
void lunar_date_set_week_holiday(LunarDate *date, GDateMonth month, gint week_of_month, gint day_of_week, const gchar *holiday)
{
	gchar key[10];

	g_return_if_fail( date != NULL);
	g_return_if_fail( week_of_month >= 1 && week_of_month <= 5);
	g_return_if_fail( day_of_week >= 0 && day_of_week <= 6);
	g_return_if_fail( holiday != NULL);

	g_snprintf(key, sizeof(key), "%02d%01d%01d", date->solar->month, week_of_month, day_of_week);
	if (g_hash_table_lookup(date->holiday_week, key) != NULL) {
		g_hash_table_remove(date->holiday_week, key);
	}
	g_hash_table_insert(date->holiday_week, g_strdup(key), g_strdup(holiday));
}


/**
 * lunar_date_get_real_holiday:
 * @date: a #LunarDate
 * @delimiter: used to join the holidays.
 * @full: Whether to display the full name of the holiday, the short name just used for calendar.
 *
 * Returns the all holiday of the date, joined with the delimiter. The date must be valid.
 *
 * Returns:  a newly-allocated holiday string of the date or NULL.
 *
 * Since: 3.0.0
 **/
static gchar* lunar_date_get_real_holiday (LunarDate *date, const gchar *delimiter, gboolean full)
{
	GString*      jieri;
	gint          i, weekday, weekth;
	gchar         buf[256];
	gpointer      value;
	static gchar  str_jq[24][20];
	gchar         yc[5] = {str_jq[0][0], str_jq[0][1], str_jq[0][2], str_jq[0][3], '\0'};

	g_return_val_if_fail(date != NULL, NULL);
	g_return_val_if_fail(delimiter != NULL, NULL);

	jieri=g_string_new(NULL);

	//append Lunar holiday.
	g_snprintf(buf, sizeof(buf), "%02d%02d", date->lunar->month, date->lunar->day);
	if (g_hash_table_lookup_extended(date->holiday_lunar, buf, NULL, &value)) {
		gchar *p;
		strcpy(buf, value);
		buf[strlen(buf)] = '\0';
		if ((p = strchr(buf, '|')) != NULL) {
			*p = '\0';
			if (full) {
				jieri=g_string_append(jieri, ++p);
			}else {
				jieri=g_string_append(jieri, buf);
			}
		} else {
			jieri=g_string_append(jieri, value);
		}
	}

	//append solar holiday.
	g_snprintf(buf, sizeof(buf), "%02d%02d", date->solar->month, date->solar->day);
	if (g_hash_table_lookup_extended(date->holiday_solar, buf, NULL, &value)) {
		gchar *p;
		strcpy(buf, value);
		buf[strlen(buf)] = '\0';
		if (jieri->len > 0) jieri=g_string_append(jieri, delimiter);
		if ((p = strchr(buf, '|')) != NULL) {
			*p = '\0';
			if (full) {
				jieri=g_string_append(jieri, ++p);
			}else {
				jieri=g_string_append(jieri, buf);
			}
		} else {
			jieri=g_string_append(jieri, value);
		}
	}

	//append week holiday.
	weekday = get_day_of_week ( date->solar->year, date->solar->month, date->solar->day);
	weekth = get_weekth_of_month ( date->solar->day);
	g_snprintf(buf, sizeof(buf), "%02d%01d%01d", date->solar->month, weekth, weekday);
	if (g_hash_table_lookup_extended(date->holiday_week, buf, NULL, &value)) {
		gchar *p;
		strcpy(buf, value);
		buf[strlen(buf)] = '\0';
		if (jieri->len > 0) jieri=g_string_append(jieri, delimiter);
		if ((p = strchr(buf, '|')) != NULL) {
			*p = '\0';
			if (full) {
				jieri=g_string_append(jieri, ++p);
			}else {
				jieri=g_string_append(jieri, buf);
			}
		} else {
			jieri=g_string_append(jieri, value);
		}
	}

	//jie2qi4
	if(atoi(yc) != date->solar->year)
	{
		for(i=0; i<24; i++)
		{
			year_jieqi(date->solar->year, i, str_jq[i]);
		}
	}
	g_snprintf(buf, sizeof(buf), "%04d%02d%02d", date->solar->year, date->solar->month, date->solar->day);
	for (i=0; i<24; i++)
	{
		gchar** jq_day;
		jq_day = g_strsplit(str_jq[i], " ", 2);
		if (g_ascii_strcasecmp(jq_day[0], buf) == 0)
		{
			if (jieri->len > 0) jieri=g_string_append(jieri, delimiter);
			jieri=g_string_append(jieri, jq_day[1]);
		}
		g_strfreev(jq_day);
	}
	if (jieri->len > 0 ) {
		return g_string_free(jieri, FALSE);
	}else{
		g_string_free(jieri, TRUE);
		return NULL;
	}
}

/* 返回用在日历上的节假日，max_len 指定返回的最长字符数 */
static gchar* lunar_date_get_cal_holiday (LunarDate *date, gint max_len)
{
	const gint utf8_len = max_len; /* 在日历上，使用3个UTF8字符 */
	const gint ascii_len =4; /* 使用4个Ascii字符 */

	gchar holiday[128];
	gchar buf[128];
	gchar *tmp;

	tmp = lunar_date_get_real_holiday (date, " ", FALSE);

	memset(buf, '\0', sizeof(buf));
	memset(holiday, '\0', sizeof(holiday));
	if (tmp != NULL) {
		gchar *p;
		g_strlcpy(buf, tmp, sizeof(buf));
		g_free(tmp);

		if ((p = strchr(buf, ' ')) != NULL) {
			*p = '\0';
		}

		if (g_utf8_validate(buf, -1, NULL)) {
			g_utf8_strncpy(holiday, buf, utf8_len);
		} else {
			g_strlcpy(holiday, buf, ascii_len);
		}
		if (strlen(holiday) > 0) {
			return g_strdup(holiday);
		}else{
			return NULL;
		}
	} else {
		return NULL;
	}
}

/**
 * lunar_date_get_calendar:
 * @date: a #LunarDate
 * @max:  max length of the returned string.
 *
 * Returns the string about the date, used to show in calendar.
 *
 * Returns:  a newly-allocated string of the date or NULL.
 *
 * Since: 3.0.0
 **/
gchar* lunar_date_get_calendar(LunarDate *date, gint max)
{
	gchar* holiday;
	holiday = lunar_date_get_cal_holiday (date, max);
	if (holiday != NULL)
		return holiday;
	if (date->lunar->day == 1) {
		if (date->lunar->isleap)
			return g_strdup_printf("%s%s%s", _("R\303\271n"), _(lunar_month_list[date->lunar->month-1]), _("Yu\303\250"));
		else
			return g_strdup_printf("%s%s", _(lunar_month_list[date->lunar->month-1]), _("Yu\303\250"));
	} else {
		return g_strdup(_(lunar_day_list[date->lunar->day-1]));
	}
	return NULL;
}

/**
 * lunar_date_get_holiday:
 * @date: a #LunarDate
 * @delimiter: used to join the holidays.
 *
 * Returns the all holiday of the date, joined with the delimiter. The date must be valid.
 *
 * Returns:  a newly-allocated holiday string of the date or NULL.
 *
 * Since: 3.0.0
 **/
gchar* lunar_date_get_holiday (LunarDate *date, const gchar *delimiter)
{
	return lunar_date_get_real_holiday (date, delimiter, TRUE);
}

#ifndef G_DISABLE_DEPRECATED
/**
 * lunar_date_get_jieri:
 * @date: a #LunarDate
 * @delimiter: used to join the holidays.
 *
 * Returns the all holiday of the date, joined with the delimiter. The date must be valid.
 *
 * Returns:  a newly-allocated holiday string of the date or NULL.
 *
 * Deprecated:3.0.0: Use lunar_date_get_holiday() instead.
 **/
gchar* lunar_date_get_jieri(LunarDate *date, const gchar *delimiter)
{
	return lunar_date_get_holiday(date, delimiter);
}
#endif

/**
 * lunar_date_strftime:
 * @date: a #LunarDate
 * @format: specify the output format.
 *
 * 使用给定的格式来输出字符串。类似于strftime的用法。可使用的格式及输出如下：
 *
 * %(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时     公历：大写->二OO八年一月二十一日
 *
 * %(year)年%(month)月%(day)日%(hour)时     公历：小写->2008年1月21日
 *
 * %(NIAN)年%(YUE)月%(RI)%(SHI)时             农历：大写->丁亥年腊月十四己酉时，(月份前带"闰"表示闰月)
 *
 * %(nian)年%(yue)月%(ri)日%(shi)时			农历：小写->2007年12月14日，(月份前带"*"表示闰月)
 *
 * %(Y60)年%(M60)月%(D60)日%(H60)时			干支：大写->丁亥年癸丑月庚申日
 *
 * %(Y8)年%(M8)月%(D8)日%(H8)时				八字：大写->丁亥年癸丑月庚申日
 *
 * %(shengxiao)								生肖：猪
 *
 * %(holiday)								节日(节日、纪念日、节气等)：立春
 *
 * 使用%(holiday)时，输出会自动截断为3个utf8字符或4个ascii字符，如果需要全部的节日信息，请使用 lunar_date_get_holiday() 得到输出。
 *
 * Returns: a newly-allocated output string, nul-terminated
 *
 * Since: 2.4.0
 **/
gchar* lunar_date_strftime (LunarDate *date, const char *format)
{
	gchar *tmp;
	gchar* t1;
	gchar tmpbuf[32];

	gchar* str = g_strdup(format);

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

	//holiday
	if (strstr(format, "%(holiday)") != NULL)
	{
		gchar *tmp2;
		if ((tmp2 = lunar_date_get_cal_holiday(date, 3)) != NULL) {
			t1 = str_replace(str, "\%\\(holiday\\)", tmp2);
			g_free(tmp2);
			g_free(str); str=g_strdup(t1); g_free(t1);
		} else {
			t1 = str_replace(str, "\%\\(holiday\\)", "");
			g_free(str); str=g_strdup(t1); g_free(t1);
		}
	}

	return str;
}

/**
 * lunar_date_free:
 * @date: a #LunarDate
 *
 * Frees a #LunarDate returned from lunar_date_new().
 *
 * Since: 2.4.0
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
	g_hash_table_destroy(date->holiday_solar);
	g_hash_table_destroy(date->holiday_lunar);
	g_hash_table_destroy(date->holiday_week);
}

static void _cl_date_calc_lunar(LunarDate *date, GError **error)
{
	GError *calc_error = NULL;

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
	date->zhi2->hour = ((date->lunar->hour + 1) / 2) % 12;
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
	int i, m, leap_month;

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
#ifdef G_OS_WIN32
		gchar *prgdir;
		prgdir =  g_win32_get_package_installation_directory_of_module(NULL);
		gchar *localedir;
		localedir = g_build_filename (prgdir, "locale", NULL);
		bindtextdomain (GETTEXT_PACKAGE, localedir);
		g_free(prgdir);
		g_free(localedir);
#else
		bindtextdomain (GETTEXT_PACKAGE, LUNAR_DATE_LOCALEDIR);
#endif

#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
		bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif
		_lunar_calendar_gettext_initialized = TRUE;
	}
}
