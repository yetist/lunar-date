const Gtk = imports.gi.Gtk
const LunarCalendar    = imports.gi.LunarCalendar

function main_quit(o, name, time)
{
	Gtk.main_quit();
}

Gtk.init(0, null);
let win = new Gtk.Window();
win.connect("delete-event", main_quit);
let cal = new LunarCalendar.Calendar();
win.add(cal);
win.show_all();
Gtk.main();
