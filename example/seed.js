#!/usr/bin/env seed
Gettext = imports.gettext;
GLib = imports.gi.GLib;
LunarDate = imports.gi.LunarDate;

Gettext.bindtextdomain("liblunar", "/usr/share/locale");
Gettext.textdomain("liblunar");

LunarDate.init(0, null);
var l = new LunarDate.Date();
l.set_solar_date(2010,4,2,18);
var out = l.strftime("%(NIAN)年%(YUE)月%(RI)日%(SHI)时");
l.free();
print(out);
