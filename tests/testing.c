/* vi: set sw=4 ts=4: */
/*
 * testing.c
 *
 * This file is part of liblunar.
 *
 * Copyright (C) 2007-2021 yetist <yetist@gmail.com>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 * */

#include <lunar-date.h>
#include <stdlib.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>

static const struct {
	GDateYear year;
	GDateMonth month;
	GDateDay day;
	guint8 hour;
	const gchar *format;
	const gchar *raw_value;
	const gchar *local_value;
} solar_array[] = {
	{
	  1910, 2, 19, 2,
	  "%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时",
	  "y\304\253ji\307\224y\304\253l\303\255ng\345\271\264\303\250r\346\234\210sh\303\255ji\307\224\346\227\245\303\250r\346\227\266",
	  "一九一○年二月十九日二时"
  },
	{
	  1910, 2, 19, 2,
	  "%(year)年%(month)月%(day)日%(hour)时",
	  "1910\345\271\2642\346\234\21019\346\227\2452\346\227\266",
	  "1910年2月19日2时"
  },
	{
	  1910, 2, 19, 2,
	  "%(NIAN)年%(YUE)月%(RI)日%(SHI)时",
	  "G\304\223ngX\305\253\345\271\264Jan\346\234\210Ch\305\253sh\303\255\346\227\245Ch\307\222u\346\227\266",
	  "庚戌年一月初十日丑时"
  },
	{
	  1910, 2, 19, 2,
	  "%(nian)年%(yue)月%(ri)日%(shi)时",
	  "1910\345\271\2641\346\234\21010\346\227\2451\346\227\266",
	  "1910年1月10日1时"
  },
	{
	  1910, 2, 19, 2,
	  "%(Y60)年%(M60)月%(D60)日%(H60)时",
	  "G\304\223ngX\305\253\345\271\264W\303\271Y\303\255n\346\234\210Y\307\220M\307\216o\346\227\245D\304\253ngCh\307\222u\346\227\266",
	  "庚戌年戊寅月乙卯日丁丑时"
  },
	{
	  1910, 2, 19, 2,
	  "%(Y8)年%(M8)月%(D8)日%(H8)时",
	  "G\304\223ngX\305\253\345\271\264W\303\271Y\303\255n\346\234\210Y\307\220M\307\216o\346\227\245D\304\253ngCh\307\222u\346\227\266",
	  "庚戌年戊寅月乙卯日丁丑时"
  },
	{
	  1910, 2, 19, 2,
	  "%(shengxiao)",
	  "Dog",
	  "狗"
  },
	{
	  1910, 2, 19, 2,
	  "%(holiday)",
	  "Y\307\224s",
	  "雨水"
  },
};

static const gint num_solar_array = G_N_ELEMENTS (solar_array);

static const struct {
	GDateYear year;
	GDateMonth month;
	GDateDay day;
	guint8 hour;
	gboolean isleap;
	const gchar *format;
	const gchar *raw_value;
	const gchar *local_value;
} lunar_array[] = {
	{
	  1906, 8, 14, 8, 0,
	  "%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时",
	  "y\304\253ji\307\224l\303\255ngli\303\271\345\271\264y\304\253sh\303\255\346\234\210y\304\253\346\227\245b\304\201\346\227\266",
	  "一九○六年一十月一日八时"
  },
	{
	  1906, 8, 14, 8, 0,
	  "%(year)年%(month)月%(day)日%(hour)时",
	  "1906\345\271\26410\346\234\2101\346\227\2458\346\227\266",
	  "1906年10月1日8时"
  },
	{
	  1906, 8, 14, 8, 0,
	  "%(NIAN)年%(YUE)月%(RI)日%(SHI)时",
	  "B\307\220ngW\307\224\345\271\264Aug\346\234\210Sh\303\255s\303\254\346\227\245Ch\303\251n\346\227\266",
	  "丙午年八月十四日辰时"
  },
	{
	  1906, 8, 14, 8, 0,
	  "%(nian)年%(yue)月%(ri)日%(shi)时",
	  "1906\345\271\2648\346\234\21014\346\227\2454\346\227\266",
	  "1906年8月14日4时"
  },
	{
	  1906, 8, 14, 8, 0,
	  "%(Y60)年%(M60)月%(D60)日%(H60)时",
	  "B\307\220ngW\307\224\345\271\264D\304\253ngY\307\222u\346\234\210W\303\271Y\303\255n\346\227\245B\307\220ngCh\303\251n\346\227\266",
	  "丙午年丁酉月戊寅日丙辰时"
  },
	{
	  1906, 8, 14, 8, 0,
	  "%(Y8)年%(M8)月%(D8)日%(H8)时",
	  "B\307\220ngW\307\224\345\271\264D\304\253ngY\307\222u\346\234\210W\303\271Y\303\255n\346\227\245B\307\220ngCh\303\251n\346\227\266",
	  "丙午年丁酉月戊寅日丙辰时"
  },
	{
	  1906, 8, 14, 8, 0,
	  "%(shengxiao)",
	  "Horse",
	  "马"
  },
};

static const gint num_lunar_array = G_N_ELEMENTS (lunar_array);

static const struct {
	GDateYear year;
	GDateMonth month;
	GDateDay day;
	guint8 hour;
	const gchar *delimiter;
	const gchar *holiday;
} holiday_array[] = {
	{1942, 11, 26, 8, " ", "感恩节"},
	{2016, 5, 8, 8, ";", "母亲节"},
	{1949, 10, 1, 8, ";", "国庆节"},
};
static const gint num_holiday_array = G_N_ELEMENTS (holiday_array);

static void test_solar_date(void);
static void test_lunar_date(void);
static void test_holiday_date(void);
static void test_custom_holiday(void);

static gboolean is_installed(void)
{
	const char *result;
	result = g_dgettext ("lunar-date", "y\304\253");

	if (g_strcmp0 (result, "一") == 0) {
		return TRUE;
	}
	return FALSE;
}

static void test_solar_date(void)
{
	gint i;
	LunarDate *date;
	GError *error = NULL;
	gchar *value;

	date = lunar_date_new();
	for (i = 0; i < num_solar_array; i++)
	{
		lunar_date_set_solar_date(date, solar_array[i].year, solar_array[i].month,
			   	solar_array[i].day, solar_array[i].hour, &error);
		if (error != NULL)
			g_error_free(error);
		value = lunar_date_strftime(date, solar_array[i].format);
		if (is_installed()) {
			g_assert_cmpstr (solar_array[i].local_value, ==, value);
		} else {
			g_assert_cmpstr (solar_array[i].raw_value, ==, value);
		}
		g_free (value);
	}
	lunar_date_free(date);
}


void test_lunar_date(void)
{
	gint i;
	LunarDate *date;
	GError *error = NULL;
	gchar *value;

	date = lunar_date_new();
	for (i = 0; i < num_lunar_array; i++)
	{
		lunar_date_set_lunar_date(date, lunar_array[i].year, lunar_array[i].month,
			   	lunar_array[i].day, lunar_array[i].hour, lunar_array[i].isleap, &error);
		if (error != NULL)
			g_error_free(error);
		value = lunar_date_strftime(date, lunar_array[i].format);
		if (is_installed()) {
			g_assert_cmpstr (lunar_array[i].local_value, ==, value);
		} else {
			g_assert_cmpstr (lunar_array[i].raw_value, ==, value);
		}
		g_free (value);
	}
	lunar_date_free(date);
}

static void test_holiday_date(void)
{
	gint i;
	LunarDate *date;
	GError *error = NULL;
	gchar *holiday;

	date = lunar_date_new();
	for (i = 0; i < num_holiday_array; i++)
	{
		lunar_date_set_solar_date(date, holiday_array[i].year, holiday_array[i].month,
			   	holiday_array[i].day, holiday_array[i].hour, &error);
		if (error != NULL)
			g_error_free(error);
		holiday = lunar_date_get_holiday(date, holiday_array[i].delimiter);
		g_assert_cmpstr (holiday_array[i].holiday, ==, holiday);
		g_free (holiday);
	}
	lunar_date_free(date);
}

static void test_custom_holiday(void)
{
	LunarDate *date;
	GError *error = NULL;
	gchar *holiday;

	date = lunar_date_new();
	lunar_date_set_solar_date(date, 2016, 5, 8, 3, &error);
	if (error != NULL)
		g_error_free(error);

	// 设置公历5月8日为"自定义节目", 简称“自定义”，
	lunar_date_set_solar_holiday  (date, 5, 8, "自定义|自定义节日");
	holiday = lunar_date_get_holiday(date, ";");
	g_assert_cmpstr (holiday, ==, "自定义节日;母亲节");
	g_free (holiday);

	// 设置农历5月8日为"这是一个自定义农历节日", 简称“这一天”，
	lunar_date_set_lunar_holiday  (date, 4, 2, "这一天|这是一个自定义农历节日");
	holiday = lunar_date_get_holiday(date, "\n");
	g_assert_cmpstr (holiday, ==, "这是一个自定义农历节日\n自定义节日\n母亲节");
	g_free (holiday);

	// 设置5月第2个星期日为"它是第二个星期日", 简称“星期日”，注意“母亲节"被覆盖了。
	lunar_date_set_week_holiday  (date, 5, 2, 0, "星期日|它是第二个星期日");
	holiday = lunar_date_get_holiday(date, "@");
	g_assert_cmpstr (holiday, ==, "这是一个自定义农历节日@自定义节日@它是第二个星期日");
	g_free (holiday);

	lunar_date_free(date);
}

int main (int argc, char* argv[])
{
	/* Test in chinese locale env */
	g_setenv("LC_ALL", "zh_CN.utf8", TRUE);
	setlocale (LC_ALL, "");
	g_test_init (&argc, &argv, NULL);
	g_test_add_func ("/date/solar", test_solar_date);
	g_test_add_func ("/date/lunar", test_lunar_date);
	if (is_installed()) {
		g_test_add_func ("/date/holiday", test_holiday_date);
		g_test_add_func ("/date/holiday/custom", test_custom_holiday);
	}
	return g_test_run ();
}
