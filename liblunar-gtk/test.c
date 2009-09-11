/* vi: set sw=4 ts=4: */
/*
 * test.c: This file is part of ____
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
#include <gtk/gtk.h>
#include "lunar-calendar.h"

int main(int argc, char** argv)
{
	gtk_init(&argc, &argv);
	GtkWidget *window;
	GtkWidget *cal;


    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	cal = lunar_calendar_new();
	gtk_container_add(GTK_CONTAINER(window), cal);
    
    gtk_widget_show_all  (window);
    gtk_main ();

	return 0;
}

/*
vi:ts=4:wrap:ai:
*/
