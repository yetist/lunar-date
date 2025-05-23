/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (C) 2007-2025 yetist <yetist@gmail.com>
 */

#ifndef __LUNAR_DATE_H__
#define __LUNAR_DATE_H__  1

#include    <glib-object.h>

G_BEGIN_DECLS

#define LUNAR_TYPE_DATE          (lunar_date_get_type ())
#define LUNAR_DATE_ERROR         (lunar_date_error_quark ())
G_DECLARE_FINAL_TYPE (LunarDate, lunar_date, LUNAR, DATE, GObject)

typedef enum
{
    LUNAR_DATE_ERROR_INTERNAL,
    LUNAR_DATE_ERROR_YEAR,
    LUNAR_DATE_ERROR_MONTH,
    LUNAR_DATE_ERROR_DAY,
    LUNAR_DATE_ERROR_HOUR,
    LUNAR_DATE_ERROR_LEAP
} LunarDateError;

GQuark lunar_date_error_quark (void);

LunarDate*  lunar_date_new                (void);
void        lunar_date_set_solar_date     (LunarDate *date, GDateYear year, GDateMonth month, GDateDay day, guint8 hour, GError **error);
void        lunar_date_set_lunar_date     (LunarDate *date, GDateYear year, GDateMonth month, GDateDay day, guint8 hour, gboolean isleap, GError **error);
void        lunar_date_set_solar_holiday  (LunarDate *date, GDateMonth month, GDateDay day, const gchar *holiday);
void        lunar_date_set_lunar_holiday  (LunarDate *date, GDateMonth month, GDateDay day, const gchar *holiday);
void        lunar_date_set_week_holiday   (LunarDate *date, GDateMonth month, gint week_of_month, gint day_of_week, const gchar *holiday);
gchar*      lunar_date_get_holiday        (LunarDate *date, const gchar *delimiter);
gchar*      lunar_date_get_calendar       (LunarDate *date, gint max);
gchar*      lunar_date_strftime           (LunarDate *date, const gchar *format);
gchar*      lunar_date_get_constellation  (LunarDate *date);
void        lunar_date_free               (LunarDate *date);

GLIB_DEPRECATED_FOR(lunar_date_get_holiday)
gchar*      lunar_date_get_jieri          (LunarDate *date, const gchar *delimiter);

G_END_DECLS

#endif /*__LUNAR_DATE_H__ */
