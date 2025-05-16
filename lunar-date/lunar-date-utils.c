/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (c) 2007-2025 yetist <yetist@gmail.com>
 */

#include <lunar-date/lunar-date.h>
#include "lunar-date-utils.h"
#include <glib/gprintf.h>
#include <glib/gi18n-lib.h>

typedef struct  _CLDate        CLDate;

static char *hanzi_num[] = {
    /* 从○ 到十: "líng" "yī" "èr" "sān" "sì" "wǔ" "liù" "qī" "bā" "jiǔ" "shí" */
    N_("l\303\255ng"), N_("y\304\253"), N_("\303\250r"), N_("s\304\201n"), N_("s\303\254"),
    N_("w\307\224"), N_("li\303\271"), N_("q\304\253"), N_("b\304\201"), N_("ji\307\224"),
    N_("sh\303\255")
};

/* 1901-2100年二十节气最小数序列 向量压缩法 */
gint enc_vector_list[24] = {
4, 19, 3, 18, 4, 19, 4, 19, 4, 20, 4, 20,
6, 22, 6, 22, 6, 22, 7, 22, 6, 21, 6, 21
};

/*
 * 1901-2100年二十节气数据 每个元素的存储格式如下：
 * https://www.hko.gov.hk/sc/gts/time/conversion.htm
 * 1-24
 * 节气所在天（减去节气最小数）
 * 1901-2100年香港天文台公布二十四节气按年存储16进制，1个16进制为4个2进制
 */
guint64 solar_terms_data_list[] = {
    0x6aaaa6aa9a5a, 0xaaaaaabaaa6a, 0xaaabbabbafaa, 0x5aa665a65aab, 0x6aaaa6aa9a5a,  // 1901 ~ 1905
    0xaaaaaaaaaa6a, 0xaaabbabbafaa, 0x5aa665a65aab, 0x6aaaa6aa9a5a, 0xaaaaaaaaaa6a,
    0xaaabbabbafaa, 0x5aa665a65aab, 0x6aaaa6aa9a56, 0xaaaaaaaa9a5a, 0xaaabaabaaeaa,
    0x569665a65aaa, 0x5aa6a6a69a56, 0x6aaaaaaa9a5a, 0xaaabaabaaeaa, 0x569665a65aaa,
    0x5aa6a6a65a56, 0x6aaaaaaa9a5a, 0xaaabaabaaa6a, 0x569665a65aaa, 0x5aa6a6a65a56,
    0x6aaaa6aa9a5a, 0xaaaaaabaaa6a, 0x555665665aaa, 0x5aa665a65a56, 0x6aaaa6aa9a5a,
    0xaaaaaabaaa6a, 0x555665665aaa, 0x5aa665a65a56, 0x6aaaa6aa9a5a, 0xaaaaaaaaaa6a,
    0x555665665aaa, 0x5aa665a65a56, 0x6aaaa6aa9a5a, 0xaaaaaaaaaa6a, 0x555665665aaa,
    0x5aa665a65a56, 0x6aaaa6aa9a5a, 0xaaaaaaaaaa6a, 0x555665655aaa, 0x569665a65a56,
    0x6aa6a6aa9a56, 0xaaaaaaaa9a5a, 0x5556556559aa, 0x569665a65a55, 0x6aa6a6a65a56,
    0xaaaaaaaa9a5a, 0x5556556559aa, 0x569665a65a55, 0x5aa6a6a65a56, 0x6aaaa6aa9a5a,
    0x5556556555aa, 0x569665a65a55, 0x5aa665a65a56, 0x6aaaa6aa9a5a, 0x55555565556a,
    0x555665665a55, 0x5aa665a65a56, 0x6aaaa6aa9a5a, 0x55555565556a, 0x555665665a55,
    0x5aa665a65a56, 0x6aaaa6aa9a5a, 0x55555555556a, 0x555665665a55, 0x5aa665a65a56,
    0x6aaaa6aa9a5a, 0x55555555556a, 0x555665655a55, 0x5aa665a65a56, 0x6aa6a6aa9a5a,
    0x55555555456a, 0x555655655a55, 0x5a9665a65a56, 0x6aa6a6a69a5a, 0x55555555456a,
    0x555655655a55, 0x569665a65a56, 0x6aa6a6a65a56, 0x55555155455a, 0x555655655955,
    0x569665a65a55, 0x5aa6a5a65a56, 0x15555155455a, 0x555555655555, 0x569665665a55,
    0x5aa665a65a56, 0x15555155455a, 0x555555655515, 0x555665665a55, 0x5aa665a65a56,
    0x15555155455a, 0x555555555515, 0x555665665a55, 0x5aa665a65a56, 0x15555155455a,
    0x555555555515, 0x555665665a55, 0x5aa665a65a56, 0x15555155455a, 0x555555555515,
    0x555655655a55, 0x5aa665a65a56, 0x15515155455a, 0x555555554515, 0x555655655a55,
    0x5a9665a65a56, 0x15515151455a, 0x555551554515, 0x555655655a55, 0x569665a65a56,
    0x155151510556, 0x555551554505, 0x555655655955, 0x569665665a55, 0x155110510556,
    0x155551554505, 0x555555655555, 0x569665665a55, 0x55110510556, 0x155551554505,
    0x555555555515, 0x555665665a55, 0x55110510556, 0x155551554505, 0x555555555515,
    0x555665665a55, 0x55110510556, 0x155551554505, 0x555555555515, 0x555655655a55,
    0x55110510556, 0x155551554505, 0x555555555515, 0x555655655a55, 0x55110510556,
    0x155151514505, 0x555555554515, 0x555655655a55, 0x54110510556, 0x155151510505,
    0x555551554515, 0x555655655a55, 0x14110110556, 0x155110510501, 0x555551554505,
    0x555555655555, 0x14110110555, 0x155110510501, 0x555551554505, 0x555555555555,
    0x14110110555, 0x55110510501, 0x155551554505, 0x555555555555, 0x110110555,
    0x55110510501, 0x155551554505, 0x555555555515, 0x110110555, 0x55110510501,
    0x155551554505, 0x555555555515, 0x100100555, 0x55110510501, 0x155151514505,
    0x555555555515, 0x100100555, 0x54110510501, 0x155151514505, 0x555551554515,
    0x100100555, 0x54110510501, 0x155150510505, 0x555551554515, 0x100100555,
    0x14110110501, 0x155110510505, 0x555551554505, 0x100055, 0x14110110500,
    0x155110510501, 0x555551554505, 0x55, 0x14110110500, 0x55110510501,
    0x155551554505, 0x55, 0x110110500, 0x55110510501, 0x155551554505,
    0x15, 0x100110500, 0x55110510501, 0x155551554505, 0x555555555515
};

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
    guint64 x, data;
    gint day;
    static const char* solar_term_name[] = {
        /*
         * "Xiǎohán" "Dàhán" "Lìchūn" "Yǔshuǐ"
         * "Jīngzhé" "Chūnfēn" "Qīngmíng" "Gǔyǔ"
         * "Lìxià" "Xiǎomǎn" "Mángzhòng" "Xiàzhì"
         * "Xiǎoshǔ" "Dàshǔ" "Lìqīu" "Chùshǔ"
         * "Báilòu" "Qīufēn" "Hánlòu" "Shuāngjiàng"
         * "Lìdōng" "Xiǎoxuě" "Dàxuě" "Dōngzhì"
         */
        N_("Xi\307\216oh\303\241n"), N_("D\303\240h\303\241n"), N_("L\303\254ch\305\253n"), N_("Y\307\224shu\307\220"),
        N_("J\304\253ngzh\303\251"), N_("Ch\305\253nf\304\223n"), N_("Q\304\253ngm\303\255ng"), N_("G\307\224y\307\224"),
        N_("L\303\254xi\303\240"), N_("Xi\307\216om\307\216n"), N_("M\303\241ngzh\303\262ng"), N_("Xi\303\240zh\303\254"),
        N_("Xi\307\216osh\307\224"), N_("D\303\240sh\307\224"), N_("L\303\254q\304\253u"), N_("Ch\303\271sh\307\224"),
        N_("B\303\241il\303\271"), N_("Q\304\253uf\304\223n"), N_("H\303\241nl\303\262u"), N_("Shu\304\201ngji\303\240ng"),
        N_("L\303\254d\305\215ng"), N_("Xi\307\216oxu\304\233"), N_("D\303\240xu\304\233"), N_("D\305\215ngzh\303\254")
    };

    data = solar_terms_data_list[year-1901];
    x = data >> (2*n);
    day = enc_vector_list[n] +  (x % 4);
    g_sprintf(result, "%04d%02d%02d %s", year, (n+2)/2, day, _(solar_term_name[n]));
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

/* 1982/34 -> 一九八二/三四 */
void num_2_hanzi(int n, char* hanzi, gulong len)
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

    g_snprintf(hanzi, len, "%s", str->str);
    g_string_free(str, TRUE);
}

void mday_2_hanzi(int n, char* hanzi, gulong len)
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

    g_snprintf(hanzi, len, "%s", str->str);
    g_string_free(str, TRUE);
}

char* str_replace(const gchar* string, const gchar* old, const gchar* new)
{
    GRegex* regex;
    char* str;

    regex = g_regex_new (old, 0, 0, NULL);
    str = g_regex_replace(regex, string, -1, 0, new, 0, NULL);
    g_regex_unref (regex);
    return str;
}
