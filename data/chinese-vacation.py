#! /usr/bin/env python
# -*- encoding:utf-8 -*-
# FileName: c.py

"中国法定假日计算"

__author__   = "yetist"
__copyright__= "Copyright (C) 2019-2021 yetist <yetist@yetibook>"
__license__  = """
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <http://www.gnu.org/licenses/>.
"""
import gi
from gi.repository import GLib

vacations = ['-18122901', '18123003', '19020407', '-19020201', '-19020301', '19040503', '19050104', '-19042801', '-19050501', '19060703', '19091303', '19100107', '-19092901', '-19101201']

vacation_days = []
work_days = []
for i in vacations:
    ymd = abs(int(i))
    year = 2000 + int(ymd / 1000000)
    month = int(ymd % 1000000 / 10000)
    day = int(ymd % 10000 / 100)

    date = GLib.Date.new_dmy(day, month, year)
    count = ymd % 100
    for j in range(count):
        yid = int(date.get_year())%100
        did = date.get_day_of_year()
        out = yid * 1000 + did
        if int(i) < 0:
            work_days.append(out)
        else:
            vacation_days.append(out)
        date.add_days(1)
print('vacation days:', vacation_days)
print('work days:', work_days)
