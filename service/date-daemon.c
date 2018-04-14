/* vi: set sw=4 ts=4 wrap ai: */
/*
 * date-daemon.c: This file is part of ____
 *
 * Copyright (C) 2016 yetist <yetist@gmail.com>
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */


#include <stdio.h>

#include <gio/gio.h>
#include <glib/gi18n.h>
#include <lunar-date.h>
#include "date-dbus-generated.h"
#include "date-daemon.h"

#define DATE_DBUS_NAME "org.chinese.Lunar.Date"
#define DATE_DBUS_PATH "/org/chinese/Lunar/Date"

struct _DateDaemon
{
	GObject            parent;
	LunarDate          *date;
	BusLunarDate       *skeleton;
	guint              bus_name_id;
	GMainLoop          *loop;
	gboolean           replace;
};

enum {
	PROP_0,
	PROP_LOOP,
	PROP_REPLACE,
	LAST_PROP
};

gboolean date_get_holiday (BusLunarDate *object,
		GDBusMethodInvocation *invocation,
		gint year,
		gint month,
		gint day,
		gint hour,
		const gchar *delimiter,
		gpointer user_data);
gboolean date_get_strftime (BusLunarDate *object,
		GDBusMethodInvocation *invocation,
		gint year,
		gint month,
		gint day,
		gint hour,
		const gchar *format,
		gpointer user_data);
gboolean date_get_calendar(
		BusLunarDate *object,
		GDBusMethodInvocation *invocation,
		gint year,
		gint month,
		gint day,
		gint hour,
		gint max,
		gpointer user_data);
static GParamSpec *properties[LAST_PROP] = { NULL };

G_DEFINE_TYPE (DateDaemon, date_daemon, G_TYPE_OBJECT)

gboolean date_get_holiday (BusLunarDate *object,
		GDBusMethodInvocation *invocation,
		gint year,
		gint month,
		gint day,
		gint hour,
		const gchar *delimiter,
		gpointer user_data)
{
	DateDaemon *daemon;
	gchar *holiday;
	GError *error = NULL;

	daemon = DATE_DAEMON (user_data);
	lunar_date_set_solar_date(daemon->date, year, month, day , hour, &error);
	if (error != NULL ) {
		g_dbus_method_invocation_return_error (invocation, g_quark_from_static_string(DATE_DBUS_NAME), 1, "%s", error->message);
		return FALSE;
	}
	holiday = lunar_date_get_holiday(daemon->date, delimiter);
	bus_lunar_date_complete_holiday (object, invocation, holiday);
	g_free(holiday);
	return TRUE;
}

gboolean date_get_strftime (
		BusLunarDate *object,
		GDBusMethodInvocation *invocation,
		gint year,
		gint month,
		gint day,
		gint hour,
		const gchar *format,
		gpointer user_data)
{
	DateDaemon *daemon;
	gchar* result;
	GError *error = NULL;

	daemon = DATE_DAEMON (user_data);
	lunar_date_set_solar_date(daemon->date, year, month, day , hour, &error);
	if (error != NULL ) {
		g_dbus_method_invocation_return_error (invocation, g_quark_from_static_string(DATE_DBUS_NAME), 1, "%s", error->message);
		return FALSE;
	}
	result = lunar_date_strftime(daemon->date, format);
	bus_lunar_date_complete_strftime (object, invocation, result);
	g_free(result);
	return TRUE;
}

gboolean date_get_calendar(
		BusLunarDate *object,
		GDBusMethodInvocation *invocation,
		gint year,
		gint month,
		gint day,
		gint hour,
		gint max,
		gpointer user_data)
{
	DateDaemon *daemon;
	gchar* result;
	GError *error = NULL;

	daemon = DATE_DAEMON (user_data);
	lunar_date_set_solar_date(daemon->date, year, month, day , hour, &error);
	if (error != NULL ) {
		g_dbus_method_invocation_return_error (invocation, g_quark_from_static_string(DATE_DBUS_NAME), 1, "%s", error->message);
		return FALSE;
	}
	result = lunar_date_get_calendar(daemon->date, max);
	bus_lunar_date_complete_calendar(object, invocation, result);
	g_free(result);
	return TRUE;
}

static void bus_acquired_handler_cb (GDBusConnection *connection,
		const gchar     *name,
		gpointer         user_data)
{
	DateDaemon *daemon;

	GError *error = NULL;
	gboolean exported;

	daemon = DATE_DAEMON (user_data);

	g_signal_connect (daemon->skeleton, "handle-holiday", G_CALLBACK (date_get_holiday), daemon);
	g_signal_connect (daemon->skeleton, "handle-strftime", G_CALLBACK (date_get_strftime), daemon);
	g_signal_connect (daemon->skeleton, "handle-calendar", G_CALLBACK (date_get_calendar), daemon);

	exported = g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (daemon->skeleton),
			connection, DATE_DBUS_PATH, &error);

	if (!exported)
	{
		g_warning ("Failed to export interface: %s", error->message);
		g_error_free (error);

		g_main_loop_quit (daemon->loop);
	}
}

static void name_lost_handler_cb (GDBusConnection *connection,
		const gchar     *name,
		gpointer         user_data)
{
	DateDaemon *daemon;

	daemon = DATE_DAEMON (user_data);
	g_debug("bus name lost\n");

	g_main_loop_quit (daemon->loop);
}

static void date_daemon_constructed (GObject *object)
{
	DateDaemon *daemon;

	GBusNameOwnerFlags flags;

	daemon = DATE_DAEMON (object);

	G_OBJECT_CLASS (date_daemon_parent_class)->constructed (object);

	flags = G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT;
	if (daemon->replace)
		flags |= G_BUS_NAME_OWNER_FLAGS_REPLACE;

	daemon->bus_name_id = g_bus_own_name (G_BUS_TYPE_SESSION,
			DATE_DBUS_NAME, flags,
			bus_acquired_handler_cb, NULL,
			name_lost_handler_cb, daemon, NULL);

}

static void date_daemon_dispose (GObject *object)
{
	DateDaemon *daemon;

	daemon = DATE_DAEMON (object);

	if (daemon->date != NULL){
		lunar_date_free(daemon->date);
	}

	if (daemon->skeleton != NULL)
	{
		GDBusInterfaceSkeleton *skeleton;

		skeleton = G_DBUS_INTERFACE_SKELETON (daemon->skeleton);
		g_dbus_interface_skeleton_unexport (skeleton);

		g_clear_object (&daemon->skeleton);
	}

	if (daemon->bus_name_id > 0)
	{
		g_bus_unown_name (daemon->bus_name_id);
		daemon->bus_name_id = 0;
	}
	G_OBJECT_CLASS (date_daemon_parent_class)->dispose (object);
}

static void date_daemon_set_property (GObject      *object,
		guint         prop_id,
		const GValue *value,
		GParamSpec   *pspec)
{
	DateDaemon *daemon;

	daemon = DATE_DAEMON (object);

	switch (prop_id)
	{
		case PROP_LOOP:
			daemon->loop = g_value_get_pointer(value);
			break;
		case PROP_REPLACE:
			daemon->replace = g_value_get_boolean (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void date_daemon_class_init (DateDaemonClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);

	gobject_class->set_property = date_daemon_set_property;

	gobject_class->constructed = date_daemon_constructed;
	gobject_class->dispose = date_daemon_dispose;

	properties[PROP_LOOP] =
		g_param_spec_pointer("loop", "loop", "loop",
				G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);
	properties[PROP_REPLACE] =
		g_param_spec_boolean ("replace", "replace", "replace", FALSE,
				G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (gobject_class, LAST_PROP, properties);
}

static void date_daemon_init (DateDaemon *daemon)
{
	daemon->skeleton = bus_lunar_date_skeleton_new();
	daemon->date = lunar_date_new();
}

DateDaemon* date_daemon_new (GMainLoop *loop, gboolean replace)
{
	return g_object_new (DATE_TYPE_DAEMON, "loop", loop, NULL);
}
