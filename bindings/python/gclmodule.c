/* -*- Mode: C; c-basic-offset: 4 -*-
 * pygtk- Python bindings for the GTK toolkit.
 * Copyright (C) 1998-2003  James Henstridge
 *
 *   gtkmodule.c: module wrapping the GTK library
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
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <Python.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <pyerrors.h>
#include "../../gcl/gcl.h"

extern void pycalendar_register_classes(PyObject *d);
extern void pycalendar_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pycalendar_functions[];
extern DL_EXPORT(void) initgclcalendar(void);
extern PyTypeObject PyGclCalendar_Type;

DL_EXPORT(void)
initgclcalendar(void)
{
    PyObject *m, *d;

    /* initialise pygobject */
    init_pygobject();
    init_pygtk();
    
    m = Py_InitModule("gclcalendar", pycalendar_functions);
    d = PyModule_GetDict(m);

    pycalendar_register_classes(d);
    pycalendar_add_constants(m, "GCL_");

    if (PyErr_Occurred()) {
        Py_FatalError("can't initialise module gclcalendar");
    }

}
