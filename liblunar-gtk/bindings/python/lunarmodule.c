/* vi: set sw=4 ts=4: */
/*
 * lunarmodule.c: This file is part of liblunar-gtk.
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <Python.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <pyerrors.h>
#include <lunar-gtk/lunar-calendar.h>

extern void pylunarcalendar_register_classes(PyObject *d);
extern void pylunarcalendar_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pylunarcalendar_functions[];
extern DL_EXPORT(void) init_lunarcalendar(void);
extern PyTypeObject PyLunarCalendar_Type;

DL_EXPORT(void)
init_lunarcalendar(void)
{
	PyObject *m, *d;

	/* initialise pygobject */
	init_pygobject();
	init_pygtk();
	
	m = Py_InitModule("liblunar._lunarcalendar", pylunarcalendar_functions);
	d = PyModule_GetDict(m);
	
	init_pygobject_check(2, 15, 2);

	pylunarcalendar_register_classes(d);
	//pylunarcalendar_add_constants(m, "LUNAR_");
	
	if (PyErr_Occurred()) {
	    Py_FatalError("can't initialise module lunar._lunarcalendar");
	}
}

/*
vi:ts=4:wrap:ai:
*/
