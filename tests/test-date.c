/* vi: set sw=4 ts=4: */
/*
 * test.c
 *
 * This file is part of ________.
 *
 * Copyright (C) 2007 - yetist <yetist@gmail.com>.
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
 * */

#include <gcl/gcl.h>

void display (GCLDate *date, gboolean islunar)
{
    char a[13][255];
    CLDate *d;
    g_printf("\n");
    if (islunar)
    {
        d = gcl_date_get_solar_date( date, a[0], a[1], a[2], a[3]);
        g_printf("Solar:");
    }
    else
    {
        d = gcl_date_get_lunar_date( date, a[0], a[1], a[2], a[3]);
        if (d->isleap)
            g_printf("Lunar(isleap):");
        else
            g_printf("Lunar:");
    }
    g_printf("<%u-%u-%u %u> <%s-%s-%s %s>\n", d->year, d->month, d->day, d->hour, a[0], a[1], a[2], a[3]);
    g_printf("Shengxiao: %s\n", gcl_date_get_shengxiao(date));
    g_printf("Ganzhi: <%s> <%s-%s-%s-%s>\n", gcl_date_get_ganzhi(date, a[4], a[5],a[6],a[7]), a[4], a[5],a[6],a[7]);
    g_printf("Bazi: <%s> <%s-%s-%s-%s>\n", gcl_date_get_bazi(date, a[8], a[9],a[10],a[11]), a[8], a[9],a[10],a[11]);
    g_printf("Jieqi:<%s>\n\n", gcl_date_get_jieri(date));

}

void random_test(void)
{
    GError *error = NULL;
    GCLDate *date;

    date = gcl_date_new();
    GDateYear year = random() % 150 + 1900;
    GDateMonth month = random() % 12 + 1;
    GDateDay day = random() % 31 + 1;
    gboolean isleap = random() % 1;
    GDateHour hour = random() % 24 + 1;

    g_printf("Test Date: <%u-%u-%u %u>\n", year, month, day, hour);
    gcl_date_set_lunar_date(date, year, month, day , hour, isleap, &error);
    if (error)
    {
        g_printf("Error: %s \n", error->message);
        return;
    }
    display(date, TRUE);

    gcl_date_set_solar_date(date, year, month, day , hour, &error);
    if (error)
    {
        g_printf("Error: %s \n", error->message);
        return;
    }
    display(date, FALSE);

    gcl_date_free(date);
}

void test(gchar* argv[])
{
    GError *error = NULL;
    GCLDate *date;

    date = gcl_date_new();

    GDateYear year = atoi(argv[1]);
    GDateMonth month =  atoi(argv[2]);
    GDateDay day = atoi(argv[3]);

    gboolean isleap = random() % 1;
    GDateHour hour = random() % 24 + 1;

    g_printf("Test Date: <%u-%u-%u %u>\n", year, month, day, hour);
    gcl_date_set_lunar_date(date, year, month, day , hour, isleap, &error);
    if (error)
    {
        g_printf("Error: %s \n", error->message);
        return;
    }
    display(date, TRUE);

    gcl_date_set_solar_date(date, year, month, day , hour, &error);
    if (error)
    {
        g_printf("Error: %s \n", error->message);
        return;
    }
    display(date, FALSE);

    gcl_date_free(date);
}

int main (int argc, char* argv[])
{
    gcl_init(&argc, &argv);
    if (argc == 4)
    {
        test(argv);
    }
    else
    {
        int i = 10;
        while (i-- >=0)
        {
            g_printf("--------------------------------------\n");
            random_test();
            sleep(2);
        }
    }
    return 0;
}
