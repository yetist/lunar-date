/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * Copyright (C) 2007-2025 yetist <yetist@gmail.com>
 */

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
