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
#include <lunar/lunar-main.h>

/**
 * lunar_init:
 * @argc: Address of the <parameter>argc</parameter> parameter of your main() function. Changed if any arguments were handled.
 * @argv: Address of the <parameter>argv</parameter> parameter of main(). Any parameters understood by lunar_init() are stripped before return.
 *
 * Call this function before using any other liblunar functions in your applications. It will initialize liblunar.
 *
 */

void	lunar_init				 (int	 *argc, char ***argv)
{
	setlocale(LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, LIBLUNAR_LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
}

const guint liblunar_major_version = LIBLUNAR_MAJOR_VERSION;
const guint liblunar_minor_version = LIBLUNAR_MINOR_VERSION;
const guint liblunar_micro_version = LIBLUNAR_MICRO_VERSION;

/**
 * lunar_check_version:
 * @required_major: the required major version.
 * @required_minor: the required minor version.
 * @required_micro: the required micro version.
 *
 * Checks that the Liblunar library in use is compatible with the
 * given version. Generally you would pass in the constants
 * #LIBLUNAR_MAJOR_VERSION, #LIBLUNAR_MINOR_VERSION, #LIBLUNAR_MICRO_VERSION
 * as the three arguments to this function; that produces
 * a check that the library in use is compatible with
 * the version of Liblunar the application or module was compiled
 * against.
 *
 * Compatibility is defined by two things: first the version
 * of the running library is newer than the version
 * @required_major.required_minor.@required_micro. Second
 * the running library must be binary compatible with the
 * version @required_major.required_minor.@required_micro
 * (same major version.)
 *
 * Return value: %NULL if the Liblunar library is compatible with the
 *   given version, or a string describing the version mismatch.
 *   The returned string is owned by Liblunar and must not be modified
 *   or freed.
 *
 **/
const gchar *
lunar_check_version (guint required_major,
                    guint required_minor,
                    guint required_micro)
{
  gint liblunar_effective_micro = 100 * LIBLUNAR_MINOR_VERSION + LIBLUNAR_MICRO_VERSION;
  gint required_effective_micro = 100 * required_minor + required_micro;

  if (required_major > LIBLUNAR_MAJOR_VERSION)
    return "Liblunar version too old (major mismatch)";
  if (required_major < LIBLUNAR_MAJOR_VERSION)
    return "Liblunar version too new (major mismatch)";
  if (required_effective_micro < liblunar_effective_micro - LIBLUNAR_BINARY_AGE)
    return "Liblunar version too new (micro mismatch)";
  if (required_effective_micro > liblunar_effective_micro)
    return "Liblunar version too old (micro mismatch)";
  return NULL;
}
