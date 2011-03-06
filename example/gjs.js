const Gettext = imports.gettext;
const GLib = imports.gi.GLib;
const LunarDate = imports.gi.LunarDate;

Gettext.bindtextdomain("liblunar", "/usr/share/locale");
Gettext.textdomain("liblunar");

LunarDate.init(0, "");
let l = new LunarDate.Date();
l.set_solar_date(2010,4,2,18);
let out = l.strftime("%(NIAN)nian%(YUE)yue%(RI)ri%(SHI)shi");
print(out);
l.free();
