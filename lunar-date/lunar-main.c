/* vi: set sw=4 ts=4: */
/*
 * lunar-main.c
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
#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif
#include <glib/gi18n-lib.h>
#include <lunar-date/lunar-main.h>

/**
 * lunar_init:
 * @argc: (inout): The number of arguments in @argv
 * @argv: (array length=argc) (inout) (allow-none): A pointer to an array
 * of arguments.
 *
 * Call this function before using any other lunar functions in your applications. It will initialize lunar.
 *
 */

void lunar_init (int *argc, char ***argv)
{
	setlocale(LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, LUNAR_DATE_LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
}

const guint lunar_major_version = LUNAR_DATE_MAJOR_VERSION;
const guint lunar_minor_version = LUNAR_DATE_MINOR_VERSION;
const guint lunar_micro_version = LUNAR_DATE_MICRO_VERSION;

/**
 * lunar_date_check_version:
 * @required_major: the required major version.
 * @required_minor: the required minor version.
 * @required_micro: the required micro version.
 *
 * Checks that the LunarDate library in use is compatible with the
 * given version. Generally you would pass in the constants
 * #LUNAR_DATE_MAJOR_VERSION, #LUNAR_DATE_MINOR_VERSION, #LUNAR_DATE_MICRO_VERSION
 * as the three arguments to this function; that produces
 * a check that the library in use is compatible with
 * the version of LunarDate the application or module was compiled
 * against.
 *
 * Compatibility is defined by two things: first the version
 * of the running library is newer than the version
 * @required_major.required_minor.@required_micro. Second
 * the running library must be binary compatible with the
 * version @required_major.required_minor.@required_micro
 * (same major version.)
 *
 * Return value: %NULL if the LunarDate library is compatible with the
 *   given version, or a string describing the version mismatch.
 *   The returned string is owned by LunarDate and must not be modified
 *   or freed.
 *
 **/
const gchar *
lunar_check_version (guint required_major,
                    guint required_minor,
                    guint required_micro)
{
  gint lunar_effective_micro = 100 * LUNAR_DATE_MINOR_VERSION + LUNAR_DATE_MICRO_VERSION;
  gint required_effective_micro = 100 * required_minor + required_micro;

  if (required_major > LUNAR_DATE_MAJOR_VERSION)
    return "LunarDate version too old (major mismatch)";
  if (required_major < LUNAR_DATE_MAJOR_VERSION)
    return "LunarDate version too new (major mismatch)";
  if (required_effective_micro < lunar_effective_micro - LUNAR_DATE_BINARY_AGE)
    return "LunarDate version too new (micro mismatch)";
  if (required_effective_micro > lunar_effective_micro)
    return "LunarDate version too old (micro mismatch)";
  return NULL;
}
