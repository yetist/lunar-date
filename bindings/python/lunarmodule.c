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
#include <config.h>
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <Python.h>
#include <pyglib.h>
#include <pygobject.h>
#include <pyerrors.h>
#include "../../lunar/lunar.h"
#include "../../lunar/lunar-date.h"
#include "../../lunar/lunar-date-private.h"

extern void pylunardate_register_classes(PyObject *d);
extern void pylunardate_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pylunardate_functions[];
extern DL_EXPORT(void) init_lunardate(void);
extern PyTypeObject PyLunarDate_Type;

DL_EXPORT(void)
init_lunardate(void)
{
    PyObject *m, *d;
    PyObject *tuple;
    PyObject *e;

    /* initialise pygobject */
    init_pygobject();
    
    /* perform any initialisation required by the library here */
    m = Py_InitModule("liblunar._lunardate", pylunardate_functions);
    d = PyModule_GetDict(m);

    init_pygobject_check(2, 15, 2);

    pylunardate_register_classes(d);
    pylunardate_add_constants(m, "LUNAR_");

/*
    PyModule_AddStringConstant(m, "ERROR", g_quark_to_string(LUNAR_DATE_ERROR));
    e = pyglib_register_exception_for_domain("lunar.Error", LUNAR_DATE_ERROR);
    PyDict_SetItemString(d, "Error", e);
    Py_DECREF(e);

    PyModule_AddStringConstant(m, "FILE_ATTRIBUTE_STANDARD_TYPE",
			       G_FILE_ATTRIBUTE_STANDARD_TYPE);
*/

    /* liblunar version */
    tuple = Py_BuildValue ("(iii)",
			   LIBLUNAR_MAJOR_VERSION,
			   LIBLUNAR_MINOR_VERSION,
			   LIBLUNAR_MICRO_VERSION);
    PyDict_SetItemString(d, "liblunar_version", tuple); 
    Py_DECREF(tuple);

    if (PyErr_Occurred()) {
        Py_FatalError("can't initialise module lunar._lunardate");
    }

}
