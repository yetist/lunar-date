/* vi: set sw=4 ts=4: */
/*
 * test.c
 *
 * This file is part of liblunar.
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

#include <lunar-date/lunar-date.h>
#include <stdlib.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>

static void display (LunarDate *date)
{
	gchar *holiday;
	g_print("format:%%(YEAR)年%%(MONTH)月%%(DAY)日%%(HOUR)时 ->%s\n", lunar_date_strftime(date, "%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时"));
	g_print("format:%%(year)年%%(month)月%%(day)日%%(hour)时 ->%s\n", lunar_date_strftime(date, "%(year)年%(month)月%(day)日%(hour)时"));
	g_print("format:%%(NIAN)年%%(YUE)月%%(RI)日%%(SHI)时     ->%s\n", lunar_date_strftime(date, "%(NIAN)年%(YUE)月%(RI)日%(SHI)时"));
	g_print("format:%%(nian)年%%(yue)月%%(ri)日%%(shi)时     ->%s\n", lunar_date_strftime(date, "%(nian)年%(yue)月%(ri)日%(shi)时"));
	g_print("format:%%(Y60)年%%(M60)月%%(D60)日%%(H60)时     ->%s\n", lunar_date_strftime(date, "%(Y60)年%(M60)月%(D60)日%(H60)时"));
	g_print("format:%%(Y8)年%%(M8)月%%(D8)日%%(H8)时         ->%s\n", lunar_date_strftime(date, "%(Y8)年%(M8)月%(D8)日%(H8)时"));
	g_print("format:%%(shengxiao)                         ->%s\n", lunar_date_strftime(date, "%(shengxiao)"));
	if ((holiday = lunar_date_get_holiday(date, "  ", TRUE)) != NULL) {
		g_print("format:%%(holiday)                           ->%s[%s]\n", lunar_date_strftime(date, "%(holiday)"), holiday);
		g_free(holiday);
	}
}

void set_lunar_date(LunarDate *date, GDateYear year, GDateMonth month, GDateDay day , guint8 hour, gboolean isleap)
{
	GError *error = NULL;

	g_printf("\nTest Lunar Date: <%u-%u-%u %u>\n", year, month, day, hour);
	lunar_date_set_lunar_date(date, year, month, day , hour, isleap, &error);
	if (error != NULL)
	{
		g_printf("Error: %s \n", error->message);
		return;
	}
	lunar_date_add_week_holiday (date, 5, 2, 0, "中国未来时间");
	display(date);

	g_printf("\nTest Solar Date: <%u-%u-%u %u>\n", year, month, day, hour);
	lunar_date_set_solar_date(date, year, month, day , hour, &error);
	if (error != NULL)
	{
		g_printf("Error: %s \n", error->message);
		return;
	}
	display(date);
	if (error != NULL)
		g_error_free(error);
}

/*
gchar* lunar_date_strftime (LunarDate *date, const char *format);
 * %(YEAR)年%(MONTH)月%(DAY)日		公历：大写
 * %(year)年%(month)月%(day)日		公历：小写
 * %(NIAN)年%(YUE)月%(RI)日%(SHI)时 阴历：大写
 * %(nian)年%(yue)月%(ri)日%(shi)时 阴历：小写
 *
 * %(Y60)年%(M60)月%(D60)日%(H60)时 干支:大写
 * %(y60)年%(m60)月%(d60)日%(h60)时 干支:小写 not use
 * %(Y8)年%(M8)月%(D8)日%(H8)时		八字:大写
 * %(y8)年%(m8)月%(d8)日%(h8)时		八字:小写 not use
 * %(shengxiao)%(jieri)				生肖和节日
 */

void random_test(void)
{
	GRand *rand = NULL;
	LunarDate *date;

	date = lunar_date_new();
	rand = g_rand_new ();

	GDateYear year = g_rand_int_range(rand, 1900, 2049);
	GDateMonth month = g_rand_int_range(rand, 1, 12);
	GDateDay day = g_rand_int_range(rand, 1, 31);
	gboolean isleap = g_rand_int_range(rand, 0, 1);
	guint8 hour = g_rand_int_range(rand, 0, 23);

	set_lunar_date(date, year, month, day , hour, isleap);

	g_rand_free(rand);
	lunar_date_free(date);
}

void test(gchar* argv[])
{
	GRand *rand = NULL;
	LunarDate *date;

	date = lunar_date_new();
	rand = g_rand_new ();

	GDateYear year = atoi(argv[1]);
	GDateMonth month =	atoi(argv[2]);
	GDateDay day = atoi(argv[3]);

	gboolean isleap = g_rand_int_range(rand, 0, 1);
	guint8 hour = g_rand_int_range(rand, 0, 23);

	set_lunar_date(date, year, month, day, hour, isleap);

	g_rand_free(rand);
	lunar_date_free(date);
}

int main (int argc, char* argv[])
{
	setlocale (LC_ALL, "");
#ifndef GLIB_VERSION_2_36
	g_type_init();
#endif
	if (argc == 4)
	{
		test(argv);
	}
	else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
	{
		g_printf("usage: test-date [year month day]\n");
		return 0;
	}
	else 
	{
		int i = 10;
		while (i-- >=0)
		{
			g_printf("--------------------------------------\n");
			random_test();
			g_usleep(20000);
		}
	}
	return 0;
}
