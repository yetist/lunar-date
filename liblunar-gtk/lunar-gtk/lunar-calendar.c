/* vi: set sw=4 ts=4: */
/*
 * lunar-calendar.c: This file is part of liblunar-gtk.
 *
 * Copyright (C) 2009 yetist <yetist@gmail.com>
 *
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

#include <gdk/gdk.h>
#include <lunar/lunar.h>
#include <string.h>
#include "lunar-calendar.h"

enum {
	LAST_SIGNAL
};

enum {
	PROP_0,
};

#define LUNAR_CALENDAR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), LUNAR_TYPE_CALENDAR, LunarCalendarPrivate))

struct _LunarCalendarPrivate
{
	LunarDate   *date;
	GdkColor	*color;
};

static void lunar_calendar_set_property  (GObject          *object,
                                         guint             prop_id,
                                         const GValue     *value,
                                         GParamSpec       *pspec);
static void lunar_calendar_get_property  (GObject          *object,
                                         guint             prop_id,
                                         GValue           *value,
                                         GParamSpec       *pspec);

static void lunar_calendar_month_changed (GtkCalendar *calendar, gpointer     user_data);
void  lunar_calendar_day_selected(GtkCalendar *calendar);

static gchar*
calendar_detail_cb (GtkCalendar *gcalendar,
                    guint        year,
                    guint        month,
                    guint        day,
                    gpointer     data);

G_DEFINE_TYPE (LunarCalendar, lunar_calendar, GTK_TYPE_CALENDAR);

static void
lunar_calendar_class_init (LunarCalendarClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (class);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	GtkCalendarClass *gcalendar_class = GTK_CALENDAR_CLASS (class);

    gobject_class->set_property = lunar_calendar_set_property;
    gobject_class->get_property = lunar_calendar_get_property;
	gcalendar_class->day_selected = lunar_calendar_day_selected;

    g_type_class_add_private (class, sizeof (LunarCalendarPrivate));
}

static void
lunar_calendar_init (LunarCalendar *calendar)
{
    LunarCalendarPrivate *priv;

    priv = LUNAR_CALENDAR_GET_PRIVATE (calendar);
	priv->date = lunar_date_new();
	priv->color = g_new0(GdkColor, 1);
	priv->color->red = 0x0;
	priv->color->green = 0x0;
	priv->color->blue = 0xffff;

	if (gtk_calendar_get_display_options(GTK_CALENDAR(calendar)) & GTK_CALENDAR_SHOW_DETAILS)
    gtk_calendar_set_detail_func (GTK_CALENDAR (calendar), calendar_detail_cb, calendar, NULL);
}

static void
lunar_calendar_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    LunarCalendar *calendar;

    calendar = LUNAR_CALENDAR (object);

    switch (prop_id)
    {

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
lunar_calendar_get_property (GObject      *object,
                            guint         prop_id,
                            GValue       *value,
                            GParamSpec   *pspec)
{
    LunarCalendar *calendar;

    calendar = LUNAR_CALENDAR (object);

    switch (prop_id)
    {

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

/**
 * lunar_calendar_new:
 * 
 * Creates a new lunar calendar, with the current date being selected. 
 * 
 * Return value: a newly #LunarCalendar widget
 **/
GtkWidget*
lunar_calendar_new (void)
{
    return g_object_new (LUNAR_TYPE_CALENDAR, NULL);
}

/**
 * lunar_calendar_set_jieri_color:
 * @lunar: a #LunarCalendar
 * @color: the holiday color.
 *
 * Setup the holiday(jieri) color.
 *
 **/
void		lunar_calendar_set_jieri_color		(LunarCalendar *lunar, const GdkColor *color)
{
  LunarCalendarPrivate *priv = LUNAR_CALENDAR_GET_PRIVATE (lunar);
  if (gdk_color_equal(priv->color, color))
		  return;
  priv->color = gdk_color_copy(color);
  gtk_widget_queue_draw(GTK_WIDGET(lunar));
}

void  lunar_calendar_day_selected(GtkCalendar *calendar)
{
	guint year, month, day;
	LunarDate *lunar;
	GError *error = NULL;

	LunarCalendarPrivate *priv = LUNAR_CALENDAR_GET_PRIVATE (calendar);
	gtk_calendar_get_date(calendar, &year, &month, &day);
	lunar_date_set_solar_date(priv->date, year, month + 1, day, 0, &error);
	char *format = g_strdup_printf("%(year)年%(month)月%(day)日\n"
	"农历 %(YUE)月%(RI)日\n"
	"干支：%(Y60)年%(M60)月%(D60)日\n"
	"八字：%(Y8)年%(M8)月%(D8)日\n"
	"生肖：%(shengxiao)\n"
	"<span foreground=\"blue\" size=\"x-small\">%s</span>\n", lunar_date_get_jieri(priv->date, "\n"));
	gtk_widget_set_tooltip_markup(GTK_WIDGET(calendar), lunar_date_strftime(priv->date, format));
}

static gchar*
calendar_detail_cb (GtkCalendar *gcalendar,
                    guint        year,
                    guint        month,
                    guint        day,
                    gpointer     data)
{
  GError *error = NULL;
  LunarCalendar *calendar = LUNAR_CALENDAR(data);
  LunarCalendarPrivate *priv = LUNAR_CALENDAR_GET_PRIVATE (calendar);
  gboolean show_detail;
  g_object_get (calendar, "show-details", &show_detail, NULL);
  if (! show_detail)
	  return NULL;

  lunar_date_set_solar_date(priv->date, year, month + 1, day, 0, &error);
  if (error)
  {
	  return NULL;
  }

  char* buf;

  if (strlen(buf = lunar_date_strftime(priv->date, "%(jieri)")) > 0)
  {
		return g_strdup_printf("<span foreground=\"%s\">%s</span>", gdk_color_to_string(priv->color), buf);
  }
  if (strcmp(lunar_date_strftime(priv->date, "%(ri)"), "1") == 0)
      return g_strdup(lunar_date_strftime(priv->date, "%(YUE)月"));
  else
      return g_strdup(lunar_date_strftime(priv->date, "%(RI)"));
}

/*
vi:ts=4:wrap:ai:
*/
