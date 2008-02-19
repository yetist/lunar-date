#!/usr/bin/python
import gtk
import lunarcalendar

def main_quit(object, *args):
	gtk.main_quit()

if __name__ == '__main__':
	window = gtk.Window()
	window.connect("delete-event", main_quit)

        cal = lunarcalendar.Calendar()
        cal.set_display_options(lunarcalendar.CALENDAR_SHOW_LUNAR
                |lunarcalendar.CALENDAR_SHOW_HEADING
                |lunarcalendar.CALENDAR_SHOW_DAY_NAMES
                |lunarcalendar.CALENDAR_SHOW_WEEK_NUMBERS
                )
        window.add(cal)
	window.show_all()
	gtk.main()
