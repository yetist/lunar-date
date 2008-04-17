#!/bin/sh
GTK_VERSION=2.12.8

sed_it () {
	sed \
	-e 's/gtkcalendar/lunarcalendar/g' \
	-e 's/gtk_calendar/lunar_calendar/g' \
	-e 's/GtkCalendar/LunarCalendar/g' \
	-e 's/GTK_CALENDAR/LUNAR_CALENDAR/g' \
	-e 's/GTK_TYPE_CALENDAR/LUNAR_TYPE_CALENDAR/g' \
	-e 's/GTK_IS_CALENDAR/LUNAR_IS_CALENDAR/g' \
	$1
}

sed_it ../../gtk+-${GTK_VERSION}/gtk/gtkcalendar.h > lunar-calendar.h
sed_it ../../gtk+-${GTK_VERSION}/gtk/gtkcalendar.c > lunar-calendar.c
cat calendar.patch |patch -p1
