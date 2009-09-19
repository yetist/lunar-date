#!/usr/bin/python
# coding=utf-8
import sys
import os
import time
sys.path.insert(1, os.path.join(os.getcwd(), ".libs"));

import liblunar
print liblunar.__file__
#print dir (liblunar)

import locale
locale.setlocale(locale.LC_ALL, "")

def test_date(y, m, d, t):
    l=liblunar.Date()
    l.set_solar_date(y, m, d, t)
    format={"%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时":"%(YEAR)年%(MONTH)月%(DAY)日%(HOUR)时",
            "%(year)年%(month)月%(day)日%(hour)时": "%(year)年%(month)月%(day)日%(hour)时",
            "%(NIAN)年%(YUE)月%(RI)日%(SHI)时": "%(NIAN)年%(YUE)月%(RI)日%(SHI)时",
            "%(nian)年%(yue)月%(ri)日%(shi)时": "%(nian)年%(yue)月%(ri)日%(shi)时",
            "%(Y60)年%(M60)月%(D60)日%(H60)时": "%(Y60)年%(M60)月%(D60)日%(H60)时",
            "%(Y8)年%(M8)月%(D8)日%(H8)时": "%(Y8)年%(M8)月%(D8)日%(H8)时",
            "生肖属%(shengxiao)": "生肖属%(shengxiao)"}
    for i in format.keys():
        print i,"\t"*2, l.strftime(format[i])
    l.free()

t = time.localtime()
test_date(t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour)
