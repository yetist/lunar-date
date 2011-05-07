/*
 * compilation:
 * valac --pkg lunar-calendar-3.0 calendar.vala
 *
 * configure:
 * $ ./configure --enable-vala-bindings ...
 *
 */

using Gtk;
using LunarCalendar;

static int main (string[] args) {
	Gtk.init (ref args);

	var window = new Window (WindowType.TOPLEVEL);
	window.title = "Lunar Calendar";
	window.position = WindowPosition.CENTER;
	window.destroy.connect (Gtk.main_quit);

	var calendar = new LunarCalendar.Calendar ();
	window.add (calendar);
	window.show_all ();

	Gtk.main ();
	return 0;
}
