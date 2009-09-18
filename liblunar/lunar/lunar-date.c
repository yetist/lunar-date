/* vi: set sw=4 ts=4: */
/*
 * lunar-date.c
 *
 * This file is part of liblunar.
 *
 * Copyright (C) 2007-2009 - yetist <yetist@gmail.com>.
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
 * Fung F. Lee   <lee@umunhum.stanford.edu>
 * Ricky Yeung   <cryeung@hotmail.com>
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
#include "lunar-main.h"
#include "lunar-date.h"
#include "lunar-date-private.h"

enum {
    LAST_SIGNAL
};

enum {
    PROP_0,
};

#define LUNAR_DATE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), LUNAR_TYPE_DATE, LunarDatePrivate))

struct _LunarDatePrivate
{
    CLDate *solar;
    CLDate *lunar;
    CLDate *lunar2;
    CLDate *gan;
    CLDate *zhi;
    CLDate *gan2;
    CLDate *zhi2;
    glong   days;
    guint   lunar_year_months[NUM_OF_YEARS];
    guint   lunar_year_days[NUM_OF_YEARS];
    guint   lunar_month_days[NUM_OF_MONTHS +1];
};

static void lunar_date_set_property  (GObject          *object,
                                         guint             prop_id,
                                         const GValue     *value,
                                         GParamSpec       *pspec);
static void lunar_date_get_property  (GObject          *object,
                                         guint             prop_id,
                                         GValue           *value,
                                         GParamSpec       *pspec);
static void _cl_date_make_all_lunar_data(LunarDate *date);

G_DEFINE_TYPE (LunarDate, lunar_date, G_TYPE_OBJECT);

static void
lunar_date_class_init (LunarDateClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (class);

    gobject_class->set_property = lunar_date_set_property;
    gobject_class->get_property = lunar_date_get_property;

    g_type_class_add_private (class, sizeof (LunarDatePrivate));
}

static void
lunar_date_init (LunarDate *date)
{
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);
    priv->solar = g_new0 (CLDate, 1);
    priv->lunar = g_new0 (CLDate, 1);
    priv->lunar2 = g_new0 (CLDate, 1);
    priv->gan   = g_new0 (CLDate, 1);
    priv->zhi   = g_new0 (CLDate, 1);
    priv->gan2   = g_new0 (CLDate, 1);
    priv->zhi2   = g_new0 (CLDate, 1);
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
lunar_date_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
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
lunar_date_get_property (GObject      *object,
                            guint         prop_id,
                            GValue       *value,
                            GParamSpec   *pspec)
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

/*
LunarDate*     lunar_date_new                (void)
{
    LunarDate *date = g_new0 (LunarDate, 1);
    date->solar = g_new0 (CLDate, 1);
    date->lunar = g_new0 (CLDate, 1);
    date->lunar2 = g_new0 (CLDate, 1);
    date->gan   = g_new0 (CLDate, 1);
    date->zhi   = g_new0 (CLDate, 1);
    date->gan2   = g_new0 (CLDate, 1);
    date->zhi2   = g_new0 (CLDate, 1);
    _cl_date_make_all_lunar_data(date);
    return date;
}
*/

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
void            lunar_date_set_solar_date     (LunarDate *date,
        GDateYear year,
        GDateMonth month,
        GDateDay day,
        GDateHour hour,
        GError **error)
{
    LunarDatePrivate *priv;
    GError *calc_error = NULL;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    if (year < BEGIN_YEAR || year > BEGIN_YEAR+NUM_OF_YEARS || (year == BEGIN_YEAR && month == 1))
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_YEAR,
                _("Year out of range."));
        return;
    }

    priv->solar->year = year;
    priv->solar->month = month;
    priv->solar->day = day;
    priv->solar->hour = hour;
    /* 计算农历 */
    _cl_date_calc_lunar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
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
void            lunar_date_set_lunar_date     (LunarDate *date,
        GDateYear year,
        GDateMonth month, 
        GDateDay day,
        GDateHour hour,
        gboolean isleap,
        GError **error)
{
    GError *calc_error = NULL;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    if (!(year>=BEGIN_YEAR && year< BEGIN_YEAR+NUM_OF_YEARS))
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_YEAR,
                _("Year out of range."));
        return;
    }

    priv->lunar->year = year;
    priv->lunar->month = month;
    priv->lunar->day = day;
    priv->lunar->hour = hour;
    priv->lunar->isleap = isleap;
    /* 计算公历 */
    _cl_date_calc_solar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    _cl_date_calc_ganzhi(date);
    _cl_date_calc_bazi(date);
}

/**
 * lunar_date_get_jieri:
 * @date: a #LunarDate
 *
 * Returns the holiday of the date. The date must be valid.
 *
 * Return value:  a newly-allocated holiday string of the date.
 * This can be changed in  <ulink url="http://www.freedesktop.org/wiki/Specifications/basedir-spec">$XDG_CONFIG_HOME</ulink>/liblunar/hodiday.dat file.
 *
 **/
gchar*      lunar_date_get_jieri          (LunarDate *date)
{
    LunarDatePrivate *priv;
    GString* jieri;

    gchar* cfgfile;
    GKeyFile* keyfile;
    gint weekday, weekth;
    gchar* str_day;

    jieri=g_string_new("");
    priv = LUNAR_DATE_GET_PRIVATE (date);
    keyfile = g_key_file_new();

#ifdef RUN_IN_SOURCE_TREE
    if (!g_file_test(cfgfile, G_FILE_TEST_IS_REGULAR))
    {
        cfgfile = g_build_filename("data", "holiday.dat", NULL);
        if (!g_file_test(cfgfile, G_FILE_TEST_IS_REGULAR))
        {
            cfgfile = g_build_filename(".", "holiday.dat", NULL);
            if (!g_file_test(cfgfile, G_FILE_TEST_IS_REGULAR))
            {
                cfgfile = g_build_filename("..", "holiday.dat", NULL);
            }
        }
    }
#else
    cfgfile = g_build_filename( g_get_user_config_dir() , "liblunar", "holiday.dat", NULL);
	if ( ! g_file_test(cfgfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
	{
		g_free(cfgfile);
    	cfgfile = g_build_filename(LUNAR_HOLIDAYDIR, "holiday.dat", NULL);
	}
#endif

    if (!g_key_file_load_from_file(keyfile, cfgfile, G_KEY_FILE_KEEP_COMMENTS, NULL))
    {
        ;
    }

    if (g_key_file_has_group(keyfile, "LUNAR"))
    {
        str_day = g_strdup_printf("%02d%02d", priv->lunar->month, priv->lunar->day);
        if (g_key_file_has_key (keyfile, "LUNAR", str_day, NULL))
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, g_key_file_get_value (keyfile, "LUNAR", str_day, NULL));
        }
    }

    if (g_key_file_has_group(keyfile, "SOLAR"))
    {
        str_day = g_strdup_printf("%02d%02d", priv->solar->month, priv->solar->day);
        if (g_key_file_has_key (keyfile, "SOLAR", str_day, NULL))
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, g_key_file_get_value (keyfile, "SOLAR", str_day, NULL));
        }
    }

    weekday = get_day_of_week ( priv->solar->year, priv->solar->month, priv->solar->day);
    weekth = get_weekth_of_month ( priv->solar->day);
    if (g_key_file_has_group(keyfile, "WEEK"))
    {
        str_day = g_strdup_printf("%02d%01d%01d", priv->solar->month, weekth, weekday);
        if (g_key_file_has_key (keyfile, "WEEK", str_day, NULL))
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, g_key_file_get_value (keyfile, "WEEK", str_day, NULL));
        }
    }

    //jie2qi4
    static char str_jq[24][20] = {'9'};
    int i;
    char yc[5] = {str_jq[0][0], str_jq[0][1], str_jq[0][2], str_jq[0][3], '\0'};
    if(atoi(yc) != priv->solar->year)
    {
        for(i=0; i<24; i++)
        {
            year_jieqi(priv->solar->year, i, str_jq[i]);
        }
    }
    str_day = g_strdup_printf("%04d%02d%02d", priv->solar->year, priv->solar->month, priv->solar->day);
    gchar** jq_day;
    for (i=0; i<24; i++)
    {
        jq_day = g_strsplit(str_jq[i], " ", 2);
        if (g_ascii_strcasecmp(jq_day[0], str_day) == 0)
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, jq_day[1]);
        }
    }
    g_strfreev(jq_day);

    gchar* oo = g_strdup(g_strstrip(jieri->str));
    g_string_free(jieri, TRUE);
    g_free(str_day);
    return oo;
}

/**
 * lunar_date_strftime:
 * @date: a #LunarDate
 * @format: specify the output format. this
 *
 * 使用给定的格式来输出字符串。类似于strftime的用法。可使用的格式及输出如下：
 *
 * %(YEAR)年%(MONTH)月%(DAY)日      公历：大写->二OO八年一月二十一日
 *
 * %(year)年%(month)月%(day)日      公历：小写->2008年1月21日
 *
 * %(NIAN)年%(YUE)月%(RI)日%(SHI)时 阴历：大写->丁亥年腊月十四日
 *
 * %(nian)年%(yue)月%(ri)日%(shi)时 阴历：小写->2007年12月14日
 *
 * %(Y60)年%(M60)月%(D60)日%(H60)时 干支：大写->丁亥年癸丑月庚申日
 *
 * %(y60)年%(m60)月%(d60)日%(h60)时 干支：小写 not use
 *
 * %(Y8)年%(M8)月%(D8)日%(H8)时     八字：大写->丁亥年癸丑月庚申日
 *
 * %(y8)年%(m8)月%(d8)日%(h8)时     八字：小写 not use
 *
 * %(shengxiao)                     生肖：猪
 * %(jieri)                         节日(节日、纪念日、节气等)：立春
 *
 * 使用%(jieri)时，如果此日没有节日或节气，那么将为空。
 * 支持自定义节日，只要按照格式修改 <ulink url="http://www.freedesktop.org/wiki/Specifications/basedir-spec">$XDG_CONFIG_HOME</ulink>/liblunar/hodiday.dat 文件即可。
 *
 * Return value: a newly-allocated output string, nul-terminated
 **/
gchar* lunar_date_strftime (LunarDate *date, const char *format)
{
    gchar *s, *tmp;
    LunarDatePrivate *priv;

    GString *str = g_string_new(format);
    priv = LUNAR_DATE_GET_PRIVATE (date);

    //solar-upper case
    if (strstr(format, "%(YEAR)") != NULL)
    {
        tmp = num_2_hanzi(priv->solar->year);
        str = g_string_replace(str, "%(YEAR)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(MONTH)") != NULL)
    {
        tmp = mday_2_hanzi(priv->solar->month);
        str = g_string_replace(str, "%(MONTH)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(DAY)") != NULL)
    {
        tmp = mday_2_hanzi(priv->solar->day);
        str = g_string_replace(str, "%(DAY)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(HOUR)") != NULL)
    {
        tmp = mday_2_hanzi(priv->solar->hour);
        str = g_string_replace(str, "%(HOUR)", tmp, -1);
        g_free(tmp);
    }

    //solar-lower case
    if (strstr(format, "%(year)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->solar->year);
        str = g_string_replace(str, "%(year)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(month)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->solar->month);
        str = g_string_replace(str, "%(month)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(day)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->solar->day);
        str = g_string_replace(str, "%(day)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(hour)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->solar->hour);
        str = g_string_replace(str, "%(hour)", tmp, -1);
        g_free(tmp);
    }

    //lunar-upper case
    if (strstr(format, "%(NIAN)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan->year]), _(zhi_list[priv->zhi->year]));
        str = g_string_replace(str, "%(NIAN)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(YUE)") != NULL)
    {
        if (priv->lunar->isleap)
            tmp = g_strdup_printf("%s%s", _("R\303\271n"), _(lunar_month_list[priv->lunar->month-1]));
        else
            tmp = g_strdup_printf("%s", _(lunar_month_list[priv->lunar->month-1]));
        str = g_string_replace(str, "%(YUE)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(RI)") != NULL)
    {
        str = g_string_replace(str, "%(RI)", _(lunar_day_list[priv->lunar->day-1]), -1);
    }
    if (strstr(format, "%(SHI)") != NULL)
    {
        str = g_string_replace(str, "%(SHI)", _(zhi_list[priv->lunar->hour/2]), -1);
    }

    //lunar-lower case
    if (strstr(format, "%(nian)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->lunar->year);
        str = g_string_replace(str, "%(nian)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(yue)") != NULL)
    {
        if (priv->lunar->isleap)
            tmp = g_strdup_printf("*%d", priv->lunar->month);
        else
            tmp = g_strdup_printf("%d", priv->lunar->month);
        str = g_string_replace(str, "%(yue)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(ri)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->lunar->day);
        str = g_string_replace(str, "%(ri)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(shi)") != NULL)
    {
        tmp = g_strdup_printf("%d", priv->lunar->hour);
        str = g_string_replace(str, "%(shi)", tmp, -1);
        g_free(tmp);
    }

    //ganzhi
    if (strstr(format, "%(Y60)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan->year]), _(zhi_list[priv->zhi->year]));
        str = g_string_replace(str, "%(Y60)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(M60)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan->month]), _(zhi_list[priv->zhi->month]));
        str = g_string_replace(str, "%(M60)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(D60)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan->day]), _(zhi_list[priv->zhi->day]));
        str = g_string_replace(str, "%(D60)", tmp, -1);
        g_free(tmp);
    }

    //bazi
    if (strstr(format, "%(Y8)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan2->year]), _(zhi_list[priv->zhi2->year]));
        str = g_string_replace(str, "%(Y8)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(M8)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan2->month]), _(zhi_list[priv->zhi2->month]));
        str = g_string_replace(str, "%(M8)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(D8)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[priv->gan2->day]), _(zhi_list[priv->zhi2->day]));
        str = g_string_replace(str, "%(D8)", tmp, -1);
        g_free(tmp);
    }

    //shengxiao
    if (strstr(format, "%(shengxiao)") != NULL)
    {
        str = g_string_replace(str, "%(shengxiao)", _(shengxiao_list[priv->zhi->year]), -1);
    }

    //jieri
    if (strstr(format, "%(jieri)") != NULL)
    {
        if (strstr(lunar_date_get_jieri(date), " " ) != NULL)
        {
            char** buf = g_strsplit(lunar_date_get_jieri(date), " ", -1);
            tmp = strdup(*buf);
            g_strfreev(buf);
        }
        else
            tmp = strdup(lunar_date_get_jieri(date));
        str = g_string_replace(str, "%(jieri)", tmp, -1);
        g_free(tmp);
    }

    s = strdup(str->str);
    g_string_free(str, TRUE);
    return s;
}

/**
 * lunar_date_free:
 * @date: a #LunarDate
 *
 * Frees a #LunarDate returned from lunar_date_new().
 **/
void            lunar_date_free                   (LunarDate *date)
{
    g_return_if_fail (date != NULL);
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    g_free(priv->solar);
    g_free(priv->lunar);
    g_free(priv->lunar2);
    g_free(priv->gan);
    g_free(priv->zhi);
    g_free(priv->gan2);
    g_free(priv->zhi2);
}

static void _cl_date_calc_lunar(LunarDate *date, GError **error)
{
    glong days;
    GError *calc_error = NULL;
    LunarDatePrivate *priv;
    CLDate *d;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    priv->days = _date_calc_days_since_reference_year(priv->solar, &calc_error) ;
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    priv->days -=  _date_calc_days_since_reference_year(&first_solar_date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    /* A lunar day begins at 11 p.m. */
    if (priv->solar->hour == 23)
        priv->days ++;

    _cl_date_days_to_lunar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    priv->lunar->hour = priv->solar->hour;
}

static void _cl_date_calc_solar(LunarDate *date, GError **error)
{
    GError *calc_error = NULL;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);
    _date_calc_days_since_lunar_year(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    _cl_date_days_to_solar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    priv->solar->hour = priv->lunar->hour;
}

//TODO
static void _cl_date_calc_ganzhi(LunarDate *date)
{
    int	year, month;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);
    year = priv->lunar->year - first_lunar_date.year;
    month = year * 12 + priv->lunar->month - 1;   /* leap months do not count */

    priv->gan->year = (first_gan_date.year + year) % 10;
    priv->zhi->year = (first_zhi_date.year + year) % 12;
    priv->gan->month = (first_gan_date.month + month) % 10;
    priv->zhi->month = (first_zhi_date.month + month) % 12;
    priv->gan->day = (first_gan_date.day + priv->days) % 10;
    priv->zhi->day = (first_zhi_date.day + priv->days) % 12;
    priv->zhi->hour = ((priv->lunar->hour + 1) / 2) % 12;
    priv->gan->hour = (priv->gan->day * 12 + priv->zhi->hour) % 10;
}

static void _cl_date_calc_bazi(LunarDate *date)
{
    int	year, month;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    _cl_date_get_bazi_lunar(date);

    year = priv->lunar2->year - first_lunar_date.year;
    month = year * 12 + priv->lunar2->month - 1;   /* leap months do not count */

    priv->gan2->year = (first_gan_date.year + year) % 10;
    priv->zhi2->year = (first_zhi_date.year + year) % 12;
    priv->gan2->month = (first_gan_date.month + month) % 10;
    priv->zhi2->month = (first_zhi_date.month + month) % 12;
    priv->gan2->day = (first_gan_date.day + priv->days) % 10;
    priv->zhi2->day = (first_zhi_date.day + priv->days) % 12;
    priv->zhi2->hour = ((priv->lunar2->hour + 1) / 2) % 12;
    priv->gan2->hour = (priv->gan2->day * 12 + priv->zhi2->hour) % 10;
}

/* Compute offset days of a lunar date from the beginning of the table */
static void _date_calc_days_since_lunar_year (LunarDate *date, GError **error)
{
    int year, i, m, leap_month;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    priv->days = 0;
    year = priv->lunar->year - first_lunar_date.year;
    for (i=0; i<year; i++)
        priv->days += priv->lunar_year_days[i];

    leap_month = _cl_date_make_lunar_month_days(date, year);
    if ((priv->lunar->isleap) && (leap_month!=priv->lunar->month))
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_LEAP,
                _("%d is not a leap month in year %d.\n"), 
                priv->lunar->month, priv->lunar->year);
        return;
    }
    for (m=1; m<priv->lunar->month; m++)
        priv->days+= priv->lunar_month_days[m];
    if (leap_month 
            && ((priv->lunar->month>leap_month) 
                || (priv->lunar->isleap && (priv->lunar->month==leap_month))
               ))
        priv->days += priv->lunar_month_days[m++];
    priv->days += priv->lunar->day - 1;

    if (priv->lunar->day > priv->lunar_month_days[m]) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_DAY,
                _("Day out of range: \"%d\""),
                priv->lunar->day);
        return;
    }
}

static void _cl_date_days_to_lunar (LunarDate *date, GError **error)
{
    int i, m, nYear, leap_month;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    glong offset = priv->days;
    for (i=0; i<NUM_OF_YEARS && offset > 0; i++)
        offset -= priv->lunar_year_days[i];
    if (offset <0)
        offset += priv->lunar_year_days[--i];
    if (i==NUM_OF_YEARS) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_DAY,
                _("Year out of range. \"%d\""),
                priv->solar->year);
        return;
    }
    priv->lunar->year = i + first_lunar_date.year;

    leap_month = _cl_date_make_lunar_month_days(date, i);
    for (m=1; m<=NUM_OF_MONTHS && offset >0; m++)
        offset -= priv->lunar_month_days[m];
    if (offset <0)
        offset += priv->lunar_month_days[--m];

    priv->lunar->isleap = FALSE;	/* don't know leap or not yet */

    if (leap_month>0)	/* has leap month */
    {
        /* if preceeding month number is the leap month,
           this month is the actual extra leap month */
        priv->lunar->isleap = (gboolean)(leap_month == (m - 1));

        /* month > leap_month is off by 1, so adjust it */
        if (m > leap_month) --m;
    }

    priv->lunar->month = m;
    priv->lunar->day = offset + 1;
}

static void _cl_date_days_to_solar(LunarDate *date, GError **error)
{
    GError *calc_error = NULL;
    gint	adj, i, m, days;
    glong offset;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    adj = (priv->lunar->hour == 23)? -1 : 0;
    offset = priv->days;
    offset += adj;
    offset -= _date_calc_days_since_reference_year(&first_lunar_date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    offset += _date_calc_days_since_reference_year(&first_solar_date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    /* offset is now the number of days from SolarFirstDate.year.1.1 */

    for (i=first_solar_date.year;
            (i<first_solar_date.year+NUM_OF_YEARS) && (offset > 0);	 i++) 
        offset -= 365 + leap(i);
    if (offset<0)
    {
        --i; 	/* LeapYear is a macro */
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
    priv->solar->year = i;

    /* assert(offset<(365+LeapYear(i))); */
    for (m=1; m<=12; m++)
    {
        days = days_in_solar_month[m];
        if ((m==2) && leap(i))	/* leap February */
            days++;
        if (offset<days)
        {
            priv->solar->month = m;
            priv->solar->day = offset + 1;
            return;
        }
        offset -= days;
    }
}

static void _cl_date_make_all_lunar_data(LunarDate *date)
{
    gint year, i, leap;
    long code;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    for (year = 0; year < NUM_OF_YEARS; year++)
    {
        code = years_info[year];
        leap = code & 0xf;
        priv->lunar_year_days[year] = 0;
        if (leap != 0)
        {
            i = (code >> 16) & 0x1;
            priv->lunar_year_days[year] += days_in_lunar_month[i];
        }
        code >>= 4;
        for (i = 0; i < NUM_OF_MONTHS-1; i++)
        {
            priv->lunar_year_days[year] += days_in_lunar_month[code&0x1];
            code >>= 1;
        }
        priv->lunar_year_months[year] = 12;
        if (leap != 0) 
            priv->lunar_year_months[year]++;
    }
}

/* Compute the days of each month in the given lunar year */
static gint _cl_date_make_lunar_month_days(LunarDate *date, gint year)
{
    int i, leap_month;
    long code;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    code = years_info[year];
    leap_month = code & 0xf;
    code >>= 4;
    if (leap_month == 0)
    {
        priv->lunar_month_days[NUM_OF_MONTHS] = 0;
        for (i = NUM_OF_MONTHS-1; i >= 1; i--)
        {
            priv->lunar_month_days[i] = days_in_lunar_month[code&0x1];
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
        priv->lunar_month_days[leap_month+1] = days_in_lunar_month[i];
        for (i = NUM_OF_MONTHS; i >= 1; i--)
        {
            if (i == leap_month+1) 
                i--;
            priv->lunar_month_days[i] = days_in_lunar_month[code&0x1];
            code >>= 1;
        }
    }
    return leap_month;
}

/* Compare two dates and return <,=,> 0 if the 1st is <,=,> the 2nd */
static gint _cl_date_get_bazi_lunar (LunarDate *date)
{
    int m, flag;
    LunarDatePrivate *priv;

    priv = LUNAR_DATE_GET_PRIVATE (date);

    if (priv->solar->month==1)
    {
        flag = _cmp_date(priv->solar->month, priv->solar->day,
                1, fest[priv->solar->year - first_solar_date.year - 1][11]);
        if (flag<0) 
            priv->lunar2->month = 11;
        else if (flag>0) 
            priv->lunar2->month = 12;
        priv->lunar2->year = priv->solar->year - 1;
        return(flag==0);
    }
    for (m=2; m<=12; m++)
    {
        flag = _cmp_date(priv->solar->month, priv->solar->day,
                m, fest[priv->solar->year - first_solar_date.year][m-2]);
        if (flag==0) m++;
        if (flag<=0) break;
    }
    priv->lunar2->month = (m-2) % 12;
    priv->lunar2->year = priv->solar->year;
    if ((priv->lunar2->month)==0)
    {
        priv->lunar2->year = priv->solar->year - 1;
        priv->lunar2->month = 12;
    }
    return(flag==0);
}
