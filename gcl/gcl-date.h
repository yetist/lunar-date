/* vi: set sw=4 ts=4: */
/*
 * gcl-date.h
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

#ifndef __GCL_DATE_H__
#define __GCL_DATE_H__  1

#include    <glib.h>

G_BEGIN_DECLS

typedef guint8  GDateHour;
typedef struct  _CLDate              CLDate;
typedef struct  _GCLDate             GCLDate;

#define GCL_DATE_ERROR	(gcl_date_error_quark ())

typedef enum
{
    GCL_DATE_ERROR_INTERNAL,
    GCL_DATE_ERROR_DAY,
    GCL_DATE_ERROR_YEAR,
    GCL_DATE_ERROR_LEAP
} GCLDateError;

GQuark gcl_date_error_quark (void);

struct _CLDate
{
    guint year   : 16;
    guint month  : 4;
    guint day    : 6;
    guint hour   : 4;
    gboolean    isleap; /* the lunar month is a leap month */
};

GCLDate*    gcl_date_new                (void);
void        gcl_date_set_solar_date     (GCLDate *date,
                                            GDateYear year,
                                            GDateMonth month,
                                            GDateDay day,
                                            GDateHour hour,
                                            GError  **error);
void        gcl_date_set_lunar_date     (GCLDate *date,
                                            GDateYear year,
                                            GDateMonth month, 
                                            GDateDay day,
                                            GDateHour hour,
                                            gboolean isleap,
                                            GError **error);
CLDate*     gcl_date_get_solar_date     (GCLDate *date, 
                                            gchar* year, 
                                            gchar* month, 
                                            gchar* day, 
                                            gchar* hour);
CLDate*     gcl_date_get_lunar_date     (GCLDate *date, 
                                            gchar* year, 
                                            gchar* month, 
                                            gchar* day, 
                                            gchar* hour);

gchar*      gcl_date_get_shengxiao      (GCLDate *date);
gchar*      gcl_date_get_ganzhi         (GCLDate *date, 
                                            gchar* year, 
                                            gchar* month,
                                            gchar* day,
                                            gchar* hour);
gchar*      gcl_date_get_bazi           (GCLDate *date, 
                                            gchar* year, 
                                            gchar* month, 
                                            gchar* day, 
                                            gchar* hour);
void        gcl_date_free               (GCLDate *date);

G_END_DECLS

#endif /*__GCL_DATE_H__ */
