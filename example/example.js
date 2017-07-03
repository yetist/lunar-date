#!/usr/bin/env gjs-console
const Gettext = imports.gettext;
const LunarDate = imports.gi.LunarDate;
Gettext.setlocale(Gettext.LocaleCategory.ALL, null);

var l = new LunarDate.Date();
l.set_solar_date(2010,4,2,18);
var out = l.strftime("%(NIAN)年%(YUE)月%(RI)日%(SHI)时");
print(out);
l.free();
