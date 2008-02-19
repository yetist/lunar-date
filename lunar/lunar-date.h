/* vi: set sw=4 ts=4: */
/*
 * lunar-date.h
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

#ifndef __LUNAR_DATE_H__
#define __LUNAR_DATE_H__  1

#include    <glib.h>

G_BEGIN_DECLS

typedef guint8  GDateHour;
typedef struct  _CLDate              CLDate;
typedef struct  _LUNARDate             LUNARDate;

#define LUNAR_DATE_ERROR	(lunar_date_error_quark ())

typedef enum
{
    LUNAR_DATE_ERROR_INTERNAL,
    LUNAR_DATE_ERROR_DAY,
    LUNAR_DATE_ERROR_YEAR,
    LUNAR_DATE_ERROR_LEAP
} LUNARDateError;

GQuark lunar_date_error_quark (void);

struct _CLDate
{
    guint year   : 16;
    guint month  : 4;
    guint day    : 6;
    guint hour   : 4;
    gboolean    isleap; /* the lunar month is a leap month */
};

LUNARDate*    lunar_date_new                (void);
void        lunar_date_set_solar_date     (LUNARDate *date,
                                            GDateYear year,
                                            GDateMonth month,
                                            GDateDay day,
                                            GDateHour hour,
                                            GError  **error);
void        lunar_date_set_lunar_date     (LUNARDate *date,
                                            GDateYear year,
                                            GDateMonth month, 
                                            GDateDay day,
                                            GDateHour hour,
                                            gboolean isleap,
                                            GError **error);
gchar*      lunar_date_get_jieri          (LUNARDate *date);
gchar*      lunar_date_strftime           (LUNARDate *date, const char *format);
void        lunar_date_free               (LUNARDate *date);

G_END_DECLS

#endif /*__LUNAR_DATE_H__ */
