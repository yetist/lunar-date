/* vi: set sw=4 ts=4: */
/*
 * gcl-calendar.h: This file is part of ____
 *
 * Copyright (C) 2007 yetist <yetist@gmail.com>
 *
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

#ifndef __GCL_CALENDAR_H__ 
#define __GCL_CALENDAR_H__  1

G_BEGIN_DECLS

#include <gtk/gtk.h>

#define GCL_TYPE_CALENDAR              (gcl_calendar_get_type ())
#define GCL_CALENDAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GCL_TYPE_CALENDAR, GclCalendar))
#define GCL_CALENDAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GCL_TYPE_CALENDAR, GclCalendarClass))
#define GCL_IS_CALENDAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GCL_TYPE_CALENDAR))
#define GCL_IS_CALENDAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GCL_TYPE_CALENDAR))
#define GCL_CALENDAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GCL_TYPE_CALENDAR, GclCalendarClass))

typedef struct _GclCalendar             GclCalendar;
typedef struct _GclCalendarClass        GclCalendarClass;
typedef struct _GclCalendarPrivate      GclCalendarPrivate;

typedef enum
{
  GCL_CALENDAR_SHOW_HEADING		    = 1 << 0,
  GCL_CALENDAR_SHOW_DAY_NAMES		= 1 << 1,
  GCL_CALENDAR_NO_MONTH_CHANGE		= 1 << 2,
  GCL_CALENDAR_SHOW_WEEK_NUMBERS	= 1 << 3,
  GCL_CALENDAR_WEEK_START_MONDAY	= 1 << 4,
  GCL_CALENDAR_SHOW_LUNAR           = 1 << 5
} GclCalendarDisplayOptions;

struct _GclCalendar
{
  GtkWidget widget;
  
  GtkStyle  *header_style;
  GtkStyle  *label_style;
  
  gint month;
  gint year;
  gint selected_day;
  
  gint day_month[6][7];
  gint day[6][7];
  
  gint num_marked_dates;
  gint marked_date[31];
  GclCalendarDisplayOptions  display_flags;
  GdkColor marked_date_color[31];
  
  GdkGC *gc;			/* unused */
  GdkGC *xor_gc;		/* unused */

  gint focus_row;
  gint focus_col;

  gint highlight_row;
  gint highlight_col;
  
  GclCalendarPrivate *priv;
  gchar grow_space [32];

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

struct _GclCalendarClass
{
  GtkWidgetClass parent_class;
  
  /* Signal handlers */
  void (* month_changed)		        (GclCalendar *calendar);
  void (* day_selected)			        (GclCalendar *calendar);
  void (* day_selected_double_click)	(GclCalendar *calendar);
  void (* prev_month)			        (GclCalendar *calendar);
  void (* next_month)			        (GclCalendar *calendar);
  void (* prev_year)			        (GclCalendar *calendar);
  void (* next_year)			        (GclCalendar *calendar);
  
};

GType	   gcl_calendar_get_type	(void) G_GNUC_CONST;
GtkWidget* gcl_calendar_new		    (void);

gboolean   gcl_calendar_select_month	(GclCalendar *calendar, guint month, guint year);
void	   gcl_calendar_select_day	    (GclCalendar *calendar, guint day);
gboolean   gcl_calendar_mark_day	    (GclCalendar *calendar, guint day);
gboolean   gcl_calendar_unmark_day	    (GclCalendar *calendar, guint day);
void	   gcl_calendar_clear_marks	    (GclCalendar *calendar);


void	   gcl_calendar_set_display_options (GclCalendar *calendar, GclCalendarDisplayOptions flags);
GclCalendarDisplayOptions gcl_calendar_get_display_options (GclCalendar *calendar);
#ifndef GTK_DISABLE_DEPRECATED
void	   gcl_calendar_display_options     (GclCalendar *calendar, GclCalendarDisplayOptions flags);
#endif

void	   gcl_calendar_get_date	(GclCalendar *calendar, guint *year, guint *month, guint *day);
#ifndef GTK_DISABLE_DEPRECATED
void	   gcl_calendar_freeze		(GclCalendar *calendar);
void	   gcl_calendar_thaw		(GclCalendar *calendar);
#endif

G_END_DECLS

#endif /* __GCL_CALENDAR_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
