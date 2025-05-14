/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (C) 2007-2025 yetist <yetist@gmail.com>
 */

#ifndef __DATE_DAEMON_H__
#define __DATE_DAEMON_H__  1

#include <glib-object.h>

G_BEGIN_DECLS

#define DATE_TYPE_DAEMON           (date_daemon_get_type ())

G_DECLARE_FINAL_TYPE (DateDaemon, date_daemon, DATE, DAEMON, GObject);

DateDaemon*     date_daemon_new    (GMainLoop *loop, gboolean replace);

G_END_DECLS

#endif /* __DATE_DAEMON_H__ */
