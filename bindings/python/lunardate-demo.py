#!/usr/bin/python
# coding=utf-8
import sys
import os
sys.path.insert(1, os.path.join(os.getcwd(), ".libs"));

import liblunar
print liblunar.__file__

print dir (liblunar)

import locale
locale.setlocale(locale.LC_ALL, "")

def test_date(y, m, d, t):
    l=liblunar.Date()
    l.set_solar_date(y, m, d, t)
    format=(
            "%(YEAR)年%(MONTH)月%(DAY)",
            "%(year)年%(month)月%(day)日",
            "%(NIAN)年%(YUE)月%(RI)日%(SHI)时",
            "%(nian)年%(yue)月%(ri)日%(shi)时",
            "%(Y60)年%(M60)月%(D60)日%(H60)时",
            "%(Y8)年%(M8)月%(D8)日%(H8)时",
            "%(shengxiao)")
    for i in format:
        print l.strftime(i)
    l.free()

test_date(2009, 3, 18, 20)
