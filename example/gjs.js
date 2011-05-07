#!/usr/bin/env gjs-console
const LunarDate = imports.gi.LunarDate;
const Gettext = imports.gettext.domain('lunar-date');

let l = new LunarDate.Date();
l.set_solar_date(2010,4,2,18);
//let out = l.strftime("%(NIAN)年%(YUE)月%(RI)日%(SHI)时");
let out = l.strftime("%(NIAN)nian%(YUE)yue%(RI)ri%(SHI)shi");
print(out);
l.free();
