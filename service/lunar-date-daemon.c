/* vi: set sw=4 ts=4 wrap ai expandtab: */
/*
 * lunar-date-daemon.c: This file is part of lunar-date.
 *
 * Copyright (C) 2016-2019 yetist <yetist@gmail.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <stdlib.h>

#include "date-daemon.h"

static gboolean debug = FALSE;
static gboolean replace = FALSE;

static GOptionEntry entries[] =
{
    {
        "debug", 0, G_OPTION_FLAG_NONE,
        G_OPTION_ARG_NONE, &debug,
        N_("Enable debugging code"),
        NULL
    },
    {
        "replace", 'r', G_OPTION_FLAG_NONE,
        G_OPTION_ARG_NONE, &replace,
        N_("Replace a currently running application"),
        NULL
    },
    {
        NULL
    }
};

static gboolean parse_arguments (int    *argc, char ***argv)
{
    GOptionContext *context;
    GError *error;

    context = g_option_context_new (NULL);

    g_option_context_add_main_entries (context, entries, NULL);

    error = NULL;
    if (g_option_context_parse (context, argc, argv, &error) == FALSE)
    {
        g_warning ("Failed to parse command line arguments: %s", error->message);
        g_error_free (error);

        return FALSE;
    }

    if (debug)
        g_setenv ("G_MESSAGES_DEBUG", "all", FALSE);

    return TRUE;
}

int main (int argc, char *argv[])
{
    GMainLoop *loop;
    DateDaemon *daemon;

    bindtextdomain (GETTEXT_PACKAGE, LUNAR_DATE_LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
    setlocale(LC_ALL, "");

#if !GLIB_CHECK_VERSION(2, 36, 0)
    g_type_init();
#endif

    if (!parse_arguments (&argc, &argv))
        return EXIT_FAILURE;

    loop = g_main_loop_new (NULL, FALSE);

    daemon = date_daemon_new (loop, replace);

    g_main_loop_run (loop);

    g_main_loop_unref (loop);
    g_object_unref (daemon);

    return EXIT_SUCCESS;
}
