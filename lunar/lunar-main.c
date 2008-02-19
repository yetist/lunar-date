/* vi: set sw=4 ts=4: */
/*
 * lunar-main.c
 *
 * This file is part of liblunar.
 *
 * Copyright (C) 2007 - yetist <yetist@gmail.com>.
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

#include <config.h>
#include <glib/gi18n-lib.h>
#include "lunar-main.h"

/**
 * lunar_init:
 * @argc: Address of the <parameter>argc</parameter> parameter of your main() function. Changed if any arguments were handled.
 * @argv: Address of the <parameter>argv</parameter> parameter of main(). Any parameters understood by lunar_init() are stripped before return.
 *
 * Call this function before using any other liblunar functions in your applications. It will initialize liblunar.
 *
 */

void    lunar_init               (int    *argc, char ***argv)
{
    bindtextdomain (GETTEXT_PACKAGE, LIBLUNAR_LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
}
