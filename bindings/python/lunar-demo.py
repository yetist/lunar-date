#!/usr/bin/python
import gtk
import liblunar

def main_quit(object, *args):
	gtk.main_quit()

if __name__ == '__main__':
	window = gtk.Window()
	window.connect("delete-event", main_quit)

        cal = liblunar.Calendar()
        cal.set_display_options(gtk.CALENDAR_SHOW_HEADING
                |gtk.CALENDAR_SHOW_DAY_NAMES
                |gtk.CALENDAR_SHOW_WEEK_NUMBERS
                |gtk.CALENDAR_SHOW_DETAILS
                )
        window.add(cal)
	window.show_all()
	gtk.main()
