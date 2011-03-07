#!/usr/bin/env gjs-console
const Gettext = imports.gettext;
const LunarDate = imports.gi.LunarDate;

Gettext.bindtextdomain("liblunar", "/usr/share/locale");
Gettext.textdomain("liblunar");

LunarDate.init(0, null);
let l = new LunarDate.Date();
l.set_solar_date(2010,4,2,18);
//var out = l.strftime("%(NIAN)年%(YUE)月%(RI)日%(SHI)时");
let out = l.strftime("%(NIAN)nian%(YUE)yue%(RI)ri%(SHI)shi");
print(out);
l.free();
