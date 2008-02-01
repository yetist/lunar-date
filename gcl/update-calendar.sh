#!/bin/sh

sed_it () {
	sed \
	-e 's/gtkcalendar/gclcalendar/g' \
	-e 's/gtk_calendar/gcl_calendar/g' \
	-e 's/GtkCalendar/GclCalendar/g' \
	-e 's/GTK_CALENDAR/GCL_CALENDAR/g' \
	-e 's/GTK_TYPE_CALENDAR/GCL_TYPE_CALENDAR/g' \
	-e 's/GTK_IS_CALENDAR/GCL_IS_CALENDAR/g' \
	$1
}

sed_it ../../gtk+/gtk/gtkcalendar.h > gcl-calendar.h
sed_it ../../gtk+/gtk/gtkcalendar.c > gcl-calendar.c
cat calendar.patch |patch -p1
