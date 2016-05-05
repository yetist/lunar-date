/*
 * compilation:
 * valac  --vapidir=. --pkg config --pkg glib-2.0 --pkg lunar-date-3.0 -C example.vala
 * gcc -DHAVE_CONFIG_H -I.. -include config.h -o example  example.c `pkg-config --cflags --libs gio-2.0 lunar-date-3.0`
 *
 * configure:
 * $ ./configure --enable-vala-bindings ...
 *
 */

using Lunar;

static int main (string[] args) {
	Intl.setlocale();

	var t = new Lunar.Date();
	try {
		t.set_solar_date(2009, DateMonth.JULY, 1, 8 );
	} catch (Error e) {
		return 1;
	}
	stdout.printf("%s\n", t.strftime(_("%(YEAR)年%(YUE)月")));
	return 0;
}
