/* vi: set sw=4 ts=4: */
/*
 * liblunar-gtk-preload.c: This file is part of liblunar-gtk.
 *
 * Copyright (C) 2009 yetist <yetist@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gtk/gtk.h>
#include <lunar-gtk/lunar.h>

GtkWidget* gtk_calendar_new (void)
{
	GtkWidget* calendar;
	calendar = lunar_calendar_new();
	return calendar;
}

void  gtk_calendar_set_display_options (GtkCalendar *calendar, GtkCalendarDisplayOptions flags)
{
	void *handle;
	char *error;
	void  (* _gtk_calendar_set_display_options) (GtkCalendar *calendar, GtkCalendarDisplayOptions flags) = NULL;

	handle = dlopen("/usr/lib/libgtk-x11-2.0.so", RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	dlerror();

	flags |= GTK_CALENDAR_SHOW_DETAILS;
	*(void **) (&_gtk_calendar_set_display_options) = dlsym(handle, "gtk_calendar_display_options");

	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}

	(*_gtk_calendar_set_display_options) (calendar, flags);
	dlclose(handle);
}

void  gtk_calendar_display_options (GtkCalendar *calendar, GtkCalendarDisplayOptions flags)
{
	gtk_calendar_set_display_options (calendar, flags);
}

/*
vi:ts=4:wrap:ai:
*/
