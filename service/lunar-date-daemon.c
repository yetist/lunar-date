/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (C) 2007-2025 yetist <yetist@gmail.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <stdlib.h>

#include "date-daemon.h"

static gboolean debug = FALSE;
static gboolean replace = FALSE;

static GOptionEntry entries[] = {
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
        g_warning (_("Failed to parse command line arguments: %s"), error->message);
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
