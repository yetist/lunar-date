/* vi: set sw=4 ts=4 wrap ai: */
/*
 * date-daemon.h: This file is part of lunar-date.
 *
 * Copyright (C) 2016-2021 yetist <yetist@gmail.com>
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 * */

#ifndef __DATE_DAEMON_H__
#define __DATE_DAEMON_H__  1

#include <glib-object.h>

G_BEGIN_DECLS

#define DATE_TYPE_DAEMON           (date_daemon_get_type ())

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DECLARE_FINAL_TYPE (DateDaemon, date_daemon, DATE, DAEMON, GObject);
#else
#define DATE_DAEMON(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), DATE_TYPE_DAEMON, DateDaemon))
#define DATE_DAEMON_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), DATE_TYPE_DAEMON, DateDaemonClass))
#define DATE_IS_DAEMON(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DATE_TYPE_DAEMON))
#define DATE_IS_DAEMON_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), DATE_TYPE_DAEMON))
#define DATE_DAEMON_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), DATE_TYPE_DAEMON, DateDaemonClass))

typedef struct _DateDaemon                DateDaemon;
typedef struct _DateDaemonClass           DateDaemonClass;
typedef struct _DateDaemonPrivate         DateDaemonPrivate;

struct _DateDaemonClass
{
       GObjectClass     parent_class;
};
GType     date_daemon_get_type                   (void) G_GNUC_CONST;
#endif

DateDaemon*     date_daemon_new    (GMainLoop *loop, gboolean replace);

G_END_DECLS

#endif /* __DATE_DAEMON_H__ */
