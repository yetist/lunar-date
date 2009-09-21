#! /usr/bin/env python
# -*- encoding:utf-8 -*-
# FileName: __init__.py

"This file is part of liblunar"
 
__author__   = "yetist"
__copyright__= "Copyright (C) 2009 yetist <yetist@gmail.com>"
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

from _lunardate import *
del _lunardate

try:
    from _lunarcalendar import *
except ImportError:
    pass
else:
    del _lunarcalendar
