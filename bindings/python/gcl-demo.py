#!/usr/bin/python
import gtk
import gclcalendar

def main_quit(object, *args):
	gtk.main_quit()

if __name__ == '__main__':
	window = gtk.Window()
	window.connect("delete-event", main_quit)

        cal = gclcalendar.Calendar()
        cal.set_display_options(gclcalendar.CALENDAR_SHOW_LUNAR
                |gclcalendar.CALENDAR_SHOW_HEADING
                |gclcalendar.CALENDAR_SHOW_DAY_NAMES
                |gclcalendar.CALENDAR_SHOW_WEEK_NUMBERS
                )
        window.add(cal)
	window.show_all()
	gtk.main()
