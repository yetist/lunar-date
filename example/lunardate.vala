/*
 * This is a vala example file.
 * use the follow command to enable vala support.
 * $ ./configure --enable-vala-bindings ...
 * build this file with:
 * $ valac --pkg liblunar-date-2.0 lunardate.vala
 *
 */

using LunarDate;

static int main (string[] args) {
	LunarDate.init(ref args);
	var t = new LunarDate.Date();
	try {
		t.set_solar_date(2009, DateMonth.JULY, 1, 8 );
	} catch (Error e) {
		return 1;
	}

	stdout.printf("%s\n", t.strftime("%(YEAR)年%(YUE)月"));
	return 0;
}
