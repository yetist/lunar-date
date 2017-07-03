/*
 * compilation:
 * valac --pkg lunar-date-3.0 example.vala
 */

using Lunar;

static int main (string[] args) {
	//Environment.set_variable ("LC_ALL", "zh_CN.utf8", true);
	Intl.setlocale(LocaleCategory.ALL, "");
	var t = new Lunar.Date();
	try {
		t.set_solar_date(2009, DateMonth.JULY, 1, 8 );
	} catch (Error e) {
		return 1;
	}
	print("%s\n", t.strftime("%(YEAR)年%(YUE)月%(RI)日"));
	return 0;
}
