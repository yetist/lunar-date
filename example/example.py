#!/usr/bin/python
# coding=utf-8

import gi
gi.require_version('LunarDate', '3.0')
from gi.repository import LunarDate
import sys
import locale
locale.setlocale(locale.LC_ALL, "")

def test_date():
    l = LunarDate.Date()
    l.set_solar_date(2010, 4, 2, 18)
    format={"%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时":"%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时",
            "%(year)年%(month)月%(day)日%(hour)时": "%(year)年%(month)月%(day)日%(hour)时",
            "%(NIAN)年%(YUE)月%(RI)日%(SHI)时": "%(NIAN)年%(YUE)月%(RI)日%(SHI)时",
            "%(nian)年%(yue)月%(ri)日%(shi)时": "%(nian)年%(yue)月%(ri)日%(shi)时",
            "%(Y60)年%(M60)月%(D60)日%(H60)时": "%(Y60)年%(M60)月%(D60)日%(H60)时",
            "%(Y8)年%(M8)月%(D8)日%(H8)时": "%(Y8)年%(M8)月%(D8)日%(H8)时",
            "生肖属%(shengxiao)": "生肖属%(shengxiao)"}
    for i in format.keys():
        print(i,"\t"*2, l.strftime(format[i]))
    l.free()

test_date()
