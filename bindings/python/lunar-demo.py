#!/usr/bin/python
import gtk
import lunar

def main_quit(object, *args):
	gtk.main_quit()

if __name__ == '__main__':
	window = gtk.Window()
	window.connect("delete-event", main_quit)

        cal = lunar.Calendar()
        cal.set_display_options(lunar.CALENDAR_SHOW_LUNAR
                |lunar.CALENDAR_SHOW_HEADING
                |lunar.CALENDAR_SHOW_DAY_NAMES
                |lunar.CALENDAR_SHOW_WEEK_NUMBERS
                )
        window.add(cal)
	window.show_all()
	gtk.main()
