/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (c) 2009-2025 yetist <yetist@gmail.com>
 */

#ifndef __LUNAR_DATE_UTILS_H__
#define __LUNAR_DATE_UTILS_H__  1

#if HAVE_CONFIG_H
#include <config.h>
#endif
#include <glib.h>
#include <glib/gi18n-lib.h>

G_BEGIN_DECLS

#define REFERENCE_YEAR	1201

typedef struct	_CLDate				 CLDate;

struct _CLDate
{
    guint year	 : 16;
    guint month  : 4;
    guint day	 : 6;
    guint hour	 : 5;
    gboolean	isleap; /* the lunar month is a leap month */
};

static int days_in_solar_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

gboolean leap (guint year);
gint	_cmp_date (gint month1, gint day1, gint month2, gint day2);
glong   _date_calc_days_since_reference_year (CLDate *d, GError **error);
void	year_jieqi(int year, int n, char* result);
gint	get_day_of_week (gint year, gint month, gint day);
gint    get_weekth_of_month (gint day);
void    num_2_hanzi(int n, char* hanzi, gulong len);
void    mday_2_hanzi(int n, char* hanzi, gulong len);
char*   str_replace(const gchar* string, const gchar* old, const gchar* new);

G_END_DECLS

#endif /* __LUNAR_DATE_UTILS_H__ */
