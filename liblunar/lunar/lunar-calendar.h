/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GTK Calendar Widget
 * Copyright (C) 1998 Cesar Miquel and Shawn T. Amundson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __LUNAR_CALENDAR_H__
#define __LUNAR_CALENDAR_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LUNAR_TYPE_CALENDAR                  (lunar_calendar_get_type ())
#define LUNAR_CALENDAR(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), LUNAR_TYPE_CALENDAR, LunarCalendar))
#define LUNAR_CALENDAR_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), LUNAR_TYPE_CALENDAR, LunarCalendarClass))
#define LUNAR_IS_CALENDAR(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LUNAR_TYPE_CALENDAR))
#define LUNAR_IS_CALENDAR_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), LUNAR_TYPE_CALENDAR))
#define LUNAR_CALENDAR_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), LUNAR_TYPE_CALENDAR, LunarCalendarClass))


typedef struct _LunarCalendar	       LunarCalendar;
typedef struct _LunarCalendarClass       LunarCalendarClass;

typedef struct _LunarCalendarPrivate     LunarCalendarPrivate;

typedef enum
{
  LUNAR_CALENDAR_SHOW_HEADING		= 1 << 0,
  LUNAR_CALENDAR_SHOW_DAY_NAMES		= 1 << 1,
  LUNAR_CALENDAR_NO_MONTH_CHANGE		= 1 << 2,
  LUNAR_CALENDAR_SHOW_WEEK_NUMBERS	= 1 << 3,
  LUNAR_CALENDAR_WEEK_START_MONDAY	= 1 << 4,
  LUNAR_CALENDAR_SHOW_LUNAR		= 1 << 5
} LunarCalendarDisplayOptions;

struct _LunarCalendar
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
  LunarCalendarDisplayOptions  display_flags;
  GdkColor marked_date_color[31];
  
  GdkGC *gc;			/* unused */
  GdkGC *xor_gc;		/* unused */

  gint focus_row;
  gint focus_col;

  gint highlight_row;
  gint highlight_col;
  
  LunarCalendarPrivate *priv;
  gchar grow_space [32];

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

struct _LunarCalendarClass
{
  GtkWidgetClass parent_class;
  
  /* Signal handlers */
  void (* month_changed)		(LunarCalendar *calendar);
  void (* day_selected)			(LunarCalendar *calendar);
  void (* day_selected_double_click)	(LunarCalendar *calendar);
  void (* prev_month)			(LunarCalendar *calendar);
  void (* next_month)			(LunarCalendar *calendar);
  void (* prev_year)			(LunarCalendar *calendar);
  void (* next_year)			(LunarCalendar *calendar);
  
};


GType	   lunar_calendar_get_type	(void) G_GNUC_CONST;
GtkWidget* lunar_calendar_new		(void);

gboolean   lunar_calendar_select_month	(LunarCalendar *calendar, 
					 guint	      month,
					 guint	      year);
void	   lunar_calendar_select_day	(LunarCalendar *calendar,
					 guint	      day);

gboolean   lunar_calendar_mark_day	(LunarCalendar *calendar,
					 guint	      day);
gboolean   lunar_calendar_unmark_day	(LunarCalendar *calendar,
					 guint	      day);
void	   lunar_calendar_clear_marks	(LunarCalendar *calendar);


void	   lunar_calendar_set_display_options (LunarCalendar    	      *calendar,
					     LunarCalendarDisplayOptions flags);
LunarCalendarDisplayOptions
           lunar_calendar_get_display_options (LunarCalendar   	      *calendar);
#ifndef GTK_DISABLE_DEPRECATED
void	   lunar_calendar_display_options (LunarCalendar		  *calendar,
					 LunarCalendarDisplayOptions flags);
#endif

void	   lunar_calendar_get_date	(LunarCalendar *calendar, 
					 guint	     *year,
					 guint	     *month,
					 guint	     *day);
#ifndef GTK_DISABLE_DEPRECATED
void	   lunar_calendar_freeze		(LunarCalendar *calendar);
void	   lunar_calendar_thaw		(LunarCalendar *calendar);
#endif

G_END_DECLS

#endif /* __LUNAR_CALENDAR_H__ */
