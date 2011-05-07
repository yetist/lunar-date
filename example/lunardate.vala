/*
 * compilation:
 * valac --pkg lunar-date-2.0 lunardate.vala
 *
 * configure:
 * $ ./configure --enable-vala-bindings ...
 *
 */

using Lunar;

static int main (string[] args) {
	var t = new Lunar.Date();
	try {
		t.set_solar_date(2009, DateMonth.JULY, 1, 8 );
	} catch (Error e) {
		return 1;
	}
	stdout.printf("%s\n", t.strftime("%(YEAR)年%(YUE)月"));
	return 0;
}
