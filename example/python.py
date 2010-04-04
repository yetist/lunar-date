#! /usr/bin/env python
# -*- encoding:utf-8 -*-
# FileName: test.py

"This file is part of ____"
 
__author__   = "yetist"
__copyright__= "Copyright (C) 2010 yetist <yetist@gmail.com>"
__license__  = """
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from gi.repository import Gtk as gtk
from gi.repository import LunarCalendar as liblunar

def main_quit(object, *args):
    gtk.main_quit()

if __name__ == '__main__':
    window = gtk.Window()
    window.connect("delete-event", main_quit)

    cal = liblunar.Calendar()
#        cal.set_display_options(gtk.CALENDAR_SHOW_HEADING
#                |gtk.CALENDAR_SHOW_DAY_NAMES
#                |gtk.CALENDAR_SHOW_WEEK_NUMBERS
#                |gtk.CALENDAR_SHOW_DETAILS
#                )
    window.add(cal)
    window.show_all()
    gtk.main()
