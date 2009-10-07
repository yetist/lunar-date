using Gtk;
using Lunar;

static int main (string[] args) {
	Gtk.init (ref args);

	var window = new Window (WindowType.TOPLEVEL);
	window.title = "Lunar Calendar";
	window.position = WindowPosition.CENTER;
	window.destroy.connect (Gtk.main_quit);

	var calendar = new Lunar.Calendar ();
	window.add (calendar);
	window.show_all ();

	Gtk.main ();
	return 0;
}
