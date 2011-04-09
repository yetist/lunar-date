/* vi: set sw=4 ts=4: */
/*
 * lunar-main.h
 *
 * This file is part of liblunar.
 *
 * Copyright (C) 2007-2011 yetist <yetist@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 * */

#ifndef __LUNAR_MAIN_H__
#define __LUNAR_MAIN_H__  1

#include	<glib.h>
#include	<lunar-date/lunar-config.h>

G_BEGIN_DECLS

void lunar_init (int *argc, char ***argv);

const gchar * lunar_check_version (guint required_major,
		guint required_minor,
		guint required_micro);

#define LUNAR_CHECK_VERSION(major,minor,micro)    \
	(LUNAR_MAJOR_VERSION > (major) || \
	 (LUNAR_MAJOR_VERSION == (major) && LUNAR_MINOR_VERSION > (minor)) || \
	 (LUNAR_MAJOR_VERSION == (major) && LUNAR_MINOR_VERSION == (minor) && \
	  LUNAR_MICRO_VERSION >= (micro)))


G_END_DECLS

#endif /*__LUNAR_MAIN_H__ */
