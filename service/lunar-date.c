/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (C) 2007-2025 yetist <yetist@gmail.com>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <lunar-date/lunar-date.h>
#include "date-dbus-generated.h"

#define MOSES_DBUS_NAME "org.chinese.Lunar.Date"
#define MOSES_DBUS_PATH "/org/chinese/Lunar/Date"

static gchar *format= "";
void show_year_month_cal(BusLunarDate *date, gint year, gint month);
void show_year_cal(BusLunarDate *date, gint year);
void show_day_hour_info(BusLunarDate *date, gint year, gint month, gint day, gint hour);

static GOptionEntry entries[] =
{
    { "format", 'f', 0, G_OPTION_ARG_STRING, &format, N_("Format for date output"), "N" },
    { NULL }
};

void show_year_month_cal(BusLunarDate *date, gint year, gint month)
{
    guint i;
    guint8 days;
    gchar *string = NULL;
    GError *error = NULL;
    GDate *gd;
    gchar* lunar[32] = {0};
    GDateWeekday weekday;
    gint day;
    gboolean num;

    if (!g_date_valid_year(year)|| !g_date_valid_month(month)) {
        g_print(_("Date is not valid\n"));
        return;
    }

    days = g_date_get_days_in_month (month, year);

    for (i=1; i<= days; i++) {
        bus_lunar_date_call_calendar_sync (date, year, month, i, 1, 3, &string, NULL, &error);
        if (error != NULL) {
            g_error_free(error);
            error = NULL;
            lunar[i] = g_strdup(" ");
        } else {
            lunar[i] = string;
        }
    }

    gd = g_date_new_dmy (1, month, year);
    weekday = g_date_get_weekday(gd);
    g_date_free(gd);

    day = 0;
    g_print("====================== %d年%d月 =====================\n", year, month);
    num = TRUE;

    //每月的表头
    g_print("周日\t周一\t周二\t周三\t周四\t周五\t周六\n");

    //打印第一行公历
    for(i=0; i<weekday%7; i++) printf("\t");
    for(i=1; i <= (7 - weekday); i++){
        g_print("%u\t", i);
    }
    if (weekday % 7 != 0) g_print("\n");

    //打印第一行农历
    for(i=0; i<weekday%7; i++) printf("\t");
    for(i=1; i <= (7 - weekday); i++){
        g_print("%s\t", lunar[i]);
    }
    if (weekday % 7 != 0) g_print("\n");

    //打印中间的日历
    for(i=8-weekday; i<= days; i++) {
        if ((weekday + i) % 7 != 0 ) {
            if (num){
                g_print("%u\t", i);
            }else{
                g_print("%s\t", lunar[i]);
            }
        }else{
            if (num) {
                g_print("%u\n", i);
                num = FALSE;
                i -= 7;
            }else{
                g_print("%s\n", lunar[i]);
                num = TRUE;
                day = i;
            }
        }
    }

    // 打印最后一行农历
    if (day < days) {
        g_print("\n");
        for(i=day+1; i<= days; i++) {
            g_print("%s\t", lunar[i]);
        }
        g_print("\n");
    }

    for (i=1; i<= days; i++) {
        g_free(lunar[i]);
    }
}

void show_year_cal(BusLunarDate *date, gint year)
{
    gint i;
    for (i =1; i <=12; i++) {
        show_year_month_cal(date, year, i);
        g_print("\n");
    }
}

void show_day_hour_info(BusLunarDate *date, gint year, gint month, gint day, gint hour)
{
    GError *error = NULL;
    gchar *string, *holiday;

    if (! g_date_valid_dmy (day, month, year)) {
        g_print(_("Date is not valid\n"));
        return;
    }

    g_print("公历：%d年%d月%d日\n", year, month, day);
    if (hour < 0) {
        GDateTime *datetime;
        datetime = g_date_time_new_now_local ();
        hour = g_date_time_get_hour (datetime);
        g_date_time_unref(datetime);
    }

    if (strlen(format) < 1) {
        format = "农历：%(NIAN)年%(YUE)月%(RI)\n"
            "干支：%(Y60)年%(M60)月%(D60)日%(D60)时\n"
            "生肖：%(shengxiao)";
    }

    bus_lunar_date_call_strftime_sync (date,
                                       year,
                                       month,
                                       day,
                                       hour,
                                       format,
                                       &string,
                                       NULL,
                                       &error);
    if (error != NULL) {
        if (error->code == LUNAR_DATE_ERROR_YEAR) {
            g_printerr (_("Unable to get cal: %s\n"), _("Year out of range.(from 1900 to 2100)."));
        } else if (error->code == LUNAR_DATE_ERROR_MONTH) {
            g_printerr (_("Unable to get cal: %s\n"), _("Month out of range.(Start from 1900-02)."));
        } else if (error->code == LUNAR_DATE_ERROR_DAY) {
            g_printerr (_("Unable to get cal: %s\n"), _("Day out of range."));
        } else if (error->code == LUNAR_DATE_ERROR_HOUR) {
            g_printerr (_("Unable to get cal: %s\n"), _("Hour out of range."));
        } else if (error->code == LUNAR_DATE_ERROR_LEAP) {
            g_printerr (_("Unable to get cal: %s\n"), _("Leap month in year error.\n"));
        }
        g_error_free(error);
        return;
    }
    bus_lunar_date_call_holiday_sync (date,
                                      year,
                                      month,
                                      day,
                                      hour,
                                      " ",
                                      &holiday,
                                      NULL,
                                      &error);
    if (error != NULL) {
        g_printerr (_("Unable to get cal: %s\n"), error->message);
        g_error_free(error);
        return;
    }
    g_print("%s\n", string);
    if (g_strcmp0(holiday, " ") != 0) {
        g_print("节日：%s\n", holiday);
    }
    g_free(string);
    g_free(holiday);
}

int main (int argc, char **argv)
{
    BusLunarDate *date;
    GDateTime *now;
    gint i, year, month, day, hour = 3;
    GError *error = NULL;
    GOptionContext *context;

    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, LUNAR_DATE_LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

#if !GLIB_CHECK_VERSION(2, 36, 0)
    g_type_init();
#endif

    date = bus_lunar_date_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                                  G_DBUS_PROXY_FLAGS_NONE,
                                                  MOSES_DBUS_NAME,
                                                  MOSES_DBUS_PATH,
                                                  NULL,
                                                  &error);
    if (error != NULL) {
        g_printerr (_("Unable to contact dbus server: %s\n"), error->message);
        g_error_free(error);
        return EXIT_FAILURE;
    }

    context = g_option_context_new (_("[year [month [day [hour]]]]"));
    g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
    g_option_context_set_summary (context, _("Display a calendar, or some part of it.\nWithout any arguments, display the current month."));

    if (!g_option_context_parse (context, &argc, &argv, &error))
    {
        g_print (_("option parsing failed: %s\n"), error->message);
        exit (1);
    }

    now = g_date_time_new_now_local ();
    g_date_time_get_ymd (now, &year, &month, &day);
    g_date_time_unref(now);

    for (i = 0; argv[i]; i++) {
        if (i == 1) year = atoi(argv[i]);
        if (i == 2) month = atoi(argv[i]);
        if (i == 3) day = atoi(argv[i]);
        if (i == 4) hour = atoi(argv[i]);
    }

    if (i == 2)
        show_year_cal(date, year);
    else if (i == 3)
        show_year_month_cal(date, year, month);
    else {
	if (year == 1900 && month == 1 && day < 31) {
	    g_print(_("Error: %s\n"), _("Day out of range.(Lunar start from 1900-01-31)."));
	    return EXIT_SUCCESS;
	}
        show_day_hour_info(date, year, month, day, hour);
    }
    return EXIT_SUCCESS;
}
