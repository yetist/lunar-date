/* vi: set sw=4 ts=4: */
/*
 * lunar-date.c
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

#if HAVE_CONFIG_H
    #include <config.h>
#endif
#include <glib/gi18n-lib.h>
#include <string.h>
#include "lunar-main.h"
#include "lunar-date.h"

#define REFERENCE_YEAR  1201
#define BEGIN_YEAR  1900	/* Note that LC1900.1.1 is SC1900.1.31 */
#define NUM_OF_YEARS 150
#define NUM_OF_MONTHS 13

static CLDate first_solar_date  = {1900, 1, 31, 0, FALSE }; /* 1900年1月31日 */
static CLDate first_lunar_date  = {1900, 1, 1, 0, FALSE };  /* 1900年元月初一 */
static CLDate first_gan_date    = {6, 4, 0, 0, FALSE };     /* 庚年午月甲日甲时 */
static CLDate first_zhi_date    = {0, 2, 4, 0, FALSE};     /* 子年寅月辰日子时 */


static long years_info[NUM_OF_YEARS] = {
    /* encoding:
		b bbbbbbbbbbbb bbbb
       bit#    	1 111111000000 0000
	        6 543210987654 3210
    		. ............ ....
       month#	  000000000111
	        M 123456789012   L
				
    b_j = 1 for long month, b_j = 0 for short month
    L is the leap month of the year if 1<=L<=12; NO leap month if L = 0.
    The leap month (if exists) is long one iff M = 1.
    */
    					                0x04bd8,	/* 1900 */
    0x04ae0, 0x0a570, 0x054d5, 0x0d260, 0x0d950,	/* 1905 */
    0x16554, 0x056a0, 0x09ad0, 0x055d2, 0x04ae0,	/* 1910 */
    0x0a5b6, 0x0a4d0, 0x0d250, 0x1d255, 0x0b540,	/* 1915 */
    0x0d6a0, 0x0ada2, 0x095b0, 0x14977, 0x04970,	/* 1920 */
    0x0a4b0, 0x0b4b5, 0x06a50, 0x06d40, 0x1ab54,	/* 1925 */
    0x02b60, 0x09570, 0x052f2, 0x04970, 0x06566,	/* 1930 */
    0x0d4a0, 0x0ea50, 0x06e95, 0x05ad0, 0x02b60,	/* 1935 */
    0x186e3, 0x092e0, 0x1c8d7, 0x0c950, 0x0d4a0,	/* 1940 */
    0x1d8a6, 0x0b550, 0x056a0, 0x1a5b4, 0x025d0,	/* 1945 */
    0x092d0, 0x0d2b2, 0x0a950, 0x0b557, 0x06ca0,	/* 1950 */
    0x0b550, 0x15355, 0x04da0, 0x0a5d0, 0x14573,	/* 1955 */
    0x052d0, 0x0a9a8, 0x0e950, 0x06aa0, 0x0aea6,	/* 1960 */
    0x0ab50, 0x04b60, 0x0aae4, 0x0a570, 0x05260,	/* 1965 */
    0x0f263, 0x0d950, 0x05b57, 0x056a0, 0x096d0,	/* 1970 */
    0x04dd5, 0x04ad0, 0x0a4d0, 0x0d4d4, 0x0d250,	/* 1975 */
    0x0d558, 0x0b540, 0x0b5a0, 0x195a6, 0x095b0,	/* 1980 */
    0x049b0, 0x0a974, 0x0a4b0, 0x0b27a, 0x06a50,	/* 1985 */
    0x06d40, 0x0af46, 0x0ab60, 0x09570, 0x04af5,	/* 1990 */
    0x04970, 0x064b0, 0x074a3, 0x0ea50, 0x06b58,	/* 1995 */
    0x05ac0, 0x0ab60, 0x096d5, 0x092e0, 0x0c960,	/* 2000 */
    0x0d954, 0x0d4a0, 0x0da50, 0x07552, 0x056a0,	/* 2005 */
    0x0abb7, 0x025d0, 0x092d0, 0x0cab5, 0x0a950,	/* 2010 */
    0x0b4a0, 0x0baa4, 0x0ad50, 0x055d9, 0x04ba0,	/* 2015 */
    0x0a5b0, 0x15176, 0x052b0, 0x0a930, 0x07954,	/* 2020 */
    0x06aa0, 0x0ad50, 0x05b52, 0x04b60, 0x0a6e6,	/* 2025 */
    0x0a4e0, 0x0d260, 0x0ea65, 0x0d530, 0x05aa0,	/* 2030 */
    0x076a3, 0x096d0, 0x04bd7, 0x04ad0, 0x0a4d0,	/* 2035 */
    0x1d0b6, 0x0d250, 0x0d520, 0x0dd45, 0x0b5a0,	/* 2040 */
    0x056d0, 0x055b2, 0x049b0, 0x0a577, 0x0a4b0,	/* 2045 */
    0x0aa50, 0x1b255, 0x06d20, 0x0ada0			/* 2049 */
};

/*
  In "4-column" calculation, a "mingli" (fortune-telling) calculation,
  the beginning of a month is not the first day of the month as in
  the Lunar Calendar; it is instead governed by "jie2" (festival).
  Interestingly, in the Solar calendar, a jie always comes around certain
  day. For example, the jie "li4chun1" (beginning of spring) always comes
  near Feburary 4 of the Solar Calendar. 

  Meaning of array fest:
  Each element, fest[i][j] stores the jie day (in term of the following Solar
  month) of the lunar i-th year, j-th month.
  For example, in 1992, fest[92][0] is 4, that means the jie "li4chun1"
  (beginning of spring) is on Feb. 4, 1992; fest[92][11] is 5, that means
  the jie of the 12th lunar month is on Jan. 5, 1993.
*/

gchar fest[NUM_OF_YEARS][12] = {
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1900 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1901 */
{5, 6, 6, 6, 7, 8, 8, 8, 9, 8, 8, 6},	/* 1902 */
{5, 7, 6, 7, 7, 8, 9, 9, 9, 8, 8, 7},	/* 1903 */
{5, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1904 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1905 */
{5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1906 */
{5, 7, 6, 7, 7, 8, 9, 9, 9, 8, 8, 7},	/* 1907 */
{5, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1908 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1909 */
{5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1910 */
{5, 7, 6, 7, 7, 8, 9, 9, 9, 8, 8, 7},	/* 1911 */
{5, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1912 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1913 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1914 */
{5, 6, 6, 6, 7, 8, 8, 9, 9, 8, 8, 6},	/* 1915 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1916 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 7, 6},	/* 1917 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1918 */
{5, 6, 6, 6, 7, 8, 8, 9, 9, 8, 8, 6},	/* 1919 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1920 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 9, 7, 6},	/* 1921 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1922 */
{5, 6, 6, 6, 7, 8, 8, 9, 9, 8, 8, 6},	/* 1923 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1924 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 7, 6},	/* 1925 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1926 */
{5, 6, 6, 6, 7, 8, 8, 8, 9, 8, 8, 6},	/* 1927 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1928 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1929 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1930 */
{5, 6, 6, 6, 7, 8, 8, 8, 9, 8, 8, 6},	/* 1931 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1932 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1933 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1934 */
{5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1935 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1936 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1937 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1938 */
{5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1939 */
{5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1940 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1941 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1942 */
{5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1943 */
{5, 6, 5, 5, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1944 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1945 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1946 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1947 */
{5, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5},	/* 1948 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1949 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1950 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1951 */
{5, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5},	/* 1952 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1953 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 7, 6},	/* 1954 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1955 */
{5, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5},	/* 1956 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1957 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1958 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1959 */
{5, 5, 5, 5, 6, 7, 7, 7, 8, 7, 7, 5},	/* 1960 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1961 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1962 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1963 */
{5, 5, 5, 5, 6, 7, 7, 7, 8, 7, 7, 5},	/* 1964 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1965 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1966 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1967 */
{5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1968 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1969 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1970 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1971 */
{5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1972 */
{4, 6, 5, 5, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1973 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1974 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1975 */
{5, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1976 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 1977 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1978 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1979 */
{5, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1980 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 1981 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1982 */
{4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6},	/* 1983 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1984 */
{5, 5, 5, 5, 5, 8, 7, 7, 8, 7, 7, 5},	/* 1985 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1986 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1987 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1988 */
{5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1989 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 1990 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1991 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1992 */
{5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1993 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1994 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1995 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1996 */
{5, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 1997 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 1998 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 1999 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2000 */
{4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2001 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 2002 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 2003 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2004 */
{4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2005 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2006 */
{4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6},	/* 2007 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2008 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2009 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2010 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 2011 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2012 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2013 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2014 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 2015 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2016 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2017 */
{4, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5},	/* 2018 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 2019 */
{4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 7, 5},	/* 2020 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2021 */
{4, 5, 5, 5, 6, 7, 7, 7, 8, 7, 7, 5},	/* 2022 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6},	/* 2023 */
{4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5},	/* 2024 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2025 */
{4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2026 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 2027 */
{4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5},	/* 2028 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2029 */
{4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2030 */
{4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6},	/* 2031 */
{4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5},	/* 2032 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2033 */
{4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2034 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2035 */
{4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5},	/* 2036 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2037 */
{4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2038 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2039 */
{4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5},	/* 2040 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2041 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2042 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2043 */
{4, 5, 4, 5, 5, 6, 7, 7, 7, 7, 6, 5},	/* 2044 */
{3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2045 */
{4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5},	/* 2046 */
{4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6},	/* 2047 */
{4, 5, 4, 5, 5, 6, 7, 7, 7, 7, 6, 5},	/* 2048 */
{3, 5, 4, 5, 5, 6, 7, 7, 8, 7, 7, 5}	/* 2049 */
};

struct _LUNARDate
{
    CLDate *solar;
    CLDate *lunar;
    CLDate *lunar2;
    CLDate *gan;
    CLDate *zhi;
    CLDate *gan2;
    CLDate *zhi2;
    glong       days;
    guint   lunar_year_months[NUM_OF_YEARS];
    guint   lunar_year_days[NUM_OF_YEARS];
    guint   lunar_month_days[NUM_OF_MONTHS +1];
};

static int days_in_solar_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int days_in_lunar_month[2]  = {29,30}; 


static	char	*gan_list[] = {
    N_("Ji\307\216"),	N_("Y\307\220"),	 N_("B\307\220ng"), N_("D\304\253ng"), N_("W\303\271"),
    N_("J\307\220"),	N_("G\304\223ng"), N_("X\304\253n"),  N_("R\303\251n"),  N_("Gu\307\220")
};

static	char	*zhi_list[] = {
    N_("Z\307\220"),	N_("Ch\307\222u"),  N_("Y\303\255n"),  N_("M\307\216o"),  N_("Ch\303\251n"), N_("S\303\254"),
    N_("W\307\224"),	N_("W\303\250i"),	 N_("Sh\304\223n"), N_("Y\307\222u"),  N_("X\305\253"),   N_("H\303\240i")
};

static	char   *shengxiao_list[] = {
    N_("Mouse"), N_("Ox"), N_("Tiger"), N_("Rabbit"), N_("Dragon"), N_("Snake"),
    N_("Horse"), N_("Goat"), N_("Monkey"), N_("Rooster"), N_("Dog"), N_("Pig")
};
static	char   *lunar_month_list[] = {
    N_("Jan"), N_("Feb"), N_("Mar"), N_("Apr"), N_("May"), N_("Jun"),
    N_("Jul"), N_("Aug"), N_("Sep"), N_("Oct"), N_("Nov"), N_("Dec")
};
static	char   *lunar_day_list[] = {
    N_("Ch\305\253y\304\253"), N_("Ch\305\253\303\250r"), N_("Ch\305\253s\304\201n"), N_("Ch\305\253s\303\254"), N_("Ch\305\253w\307\224"), 
    N_("Ch\305\253li\303\271"), N_("Ch\305\253q\304\253"), N_("Ch\305\253b\304\201"), N_("Ch\305\253ji\307\224"), N_("Ch\305\253sh\303\255"), 
    N_("Sh\303\255y\304\253"), N_("Sh\303\255\303\250r"), N_("Sh\303\255s\304\201n"), N_("Sh\303\255s\303\254"), N_("Sh\303\255w\307\224"),
    N_("Sh\303\255li\303\271"), N_("Sh\303\255q\304\253"), N_("Sh\303\255b\304\201"), N_("Sh\303\255ji\307\224"), N_("\303\210rsh\303\255"), 
    N_("\303\210ry\304\253"), N_("\303\210r\303\250r"), N_("\303\210rs\304\201n"), N_("\303\210rs\303\254"), N_("\303\210rw\307\224"), 
    N_("\303\210rli\303\271"), N_("\303\210rq\304\253"), N_("\303\210rb\304\201"), N_("\303\210rji\307\224"), N_("S\304\201nsh\303\255")
};

static char *hanzi_num[] = {
    N_("l\303\255ng"), N_("y\304\253"), N_("\303\250r"), N_("s\304\201n"), N_("s\303\254"),
    N_("w\307\224"), N_("li\303\271"), N_("q\304\253"), N_("b\304\201"), N_("ji\307\224"),
    N_("sh\303\255")
};

static void _cl_date_calc_lunar(LUNARDate *date, GError **error);
static void _cl_date_calc_solar(LUNARDate *date, GError **error);
static gint _cl_date_make_lunar_month_days(LUNARDate *date, gint year);
static void _cl_date_make_all_lunar_data(LUNARDate *date);
static void _cl_date_days_to_lunar (LUNARDate *date, GError **error);
static void _cl_date_days_to_solar(LUNARDate *date, GError **error);
static void _cl_date_calc_ganzhi(LUNARDate *date);
static void _cl_date_calc_bazi(LUNARDate *date);
static gint _cl_date_get_bazi_lunar (LUNARDate *date);
static glong _date_calc_days_since_reference_year (CLDate *d, GError **error);
static void _date_calc_days_since_lunar_year (LUNARDate *date, GError **error);
static gint get_day_of_week (gint year, gint month, gint day);
static gint get_weekth_of_month (gint day);
static int mymemfind(const char *mem, int len, const char *pat, int pat_len);
static int mymemcnt(const char *mem, int len, const char *pat, int pat_len);
static GString* g_string_replace (GString *string, const gchar* old, const gchar* new, int count);
char* num_2_hanzi(int n);
char* mday_2_hanzi(int n);

GQuark lunar_date_error_quark (void)
{
    static GQuark quark = 0;

    if (G_UNLIKELY (quark == 0))
        quark = g_quark_from_static_string ("lunar-date-error-quark");

    return quark;
}

static gboolean leap (guint year)
{
    return((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0));
} 
static gint _cmp_date (gint month1, gint day1, gint month2, gint day2)
{
    if (month1!=month2) return(month1-month2);
    if (day1!=day2) return(day1-day2);
    return(0);
}

/**
 * lunar_date_new:
 *
 * Allocates a #LUNARDate and initializes it. Free the return value with lunar_date_free().
 *
 * Return value: a newly-allocated #LUNARDate
 **/
LUNARDate*     lunar_date_new                (void)
{
    LUNARDate *date = g_new0 (LUNARDate, 1);
    date->solar = g_new0 (CLDate, 1);
    date->lunar = g_new0 (CLDate, 1);
    date->lunar2 = g_new0 (CLDate, 1);
    date->gan   = g_new0 (CLDate, 1);
    date->zhi   = g_new0 (CLDate, 1);
    date->gan2   = g_new0 (CLDate, 1);
    date->zhi2   = g_new0 (CLDate, 1);
    _cl_date_make_all_lunar_data(date);
    return date;
}

/**
 * lunar_date_set_solar_date:
 * @date: a #LUNARDate.
 * @year: year to set.
 * @month: month to set.
 * @day: day to set.
 * @hour: hour to set.
 * @error: location to store the error occuring, or NULL to ignore errors.
 *
 * Sets the solar year, month, day and the hour for a #LUNARDate.
 **/
void            lunar_date_set_solar_date     (LUNARDate *date,
        GDateYear year,
        GDateMonth month,
        GDateDay day,
        GDateHour hour,
        GError **error)
{
    GError *calc_error = NULL;

    if (!(year>=BEGIN_YEAR && year< BEGIN_YEAR+NUM_OF_YEARS))
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_YEAR,
                _("Year out of range."));
        return;
    }

    date->solar->year = year;
    date->solar->month = month;
    date->solar->day = day;
    date->solar->hour = hour;
    /* 计算农历 */
    _cl_date_calc_lunar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    _cl_date_calc_ganzhi(date);
    _cl_date_calc_bazi(date);

}

/**
 * lunar_date_set_lunar_date:
 * @date: a #LUNARDate.
 * @year: year to set.
 * @month: month to set.
 * @day: day to set.
 * @hour: hour to set.
 * @isleap: indicate whether the month is a leap month.
 * @error: location to store the error occuring, or #NULL to ignore errors.
 *
 * Sets the lunar year, month, day and the hour for a #LUNARDate. If the month is a leap month, you should set the isleap to TRUE.
 **/
void            lunar_date_set_lunar_date     (LUNARDate *date,
        GDateYear year,
        GDateMonth month, 
        GDateDay day,
        GDateHour hour,
        gboolean isleap,
        GError **error)
{
    GError *calc_error = NULL;

    if (!(year>=BEGIN_YEAR && year< BEGIN_YEAR+NUM_OF_YEARS))
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_YEAR,
                _("Year out of range."));
        return;
    }

    date->lunar->year = year;
    date->lunar->month = month;
    date->lunar->day = day;
    date->lunar->hour = hour;
    date->lunar->isleap = isleap;
    /* 计算公历 */
    _cl_date_calc_solar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    _cl_date_calc_ganzhi(date);
    _cl_date_calc_bazi(date);
}

/**
 * lunar_date_get_jieri:
 * @date: a #LUNARDate
 *
 * Returns the holiday of the date. The date must be valid.
 *
 * Return value:  a newly-allocated holiday string of the date.
 * This can be changed in $(datadir)/liblunar/holiday.dat file.
 **/
gchar*      lunar_date_get_jieri          (LUNARDate *date)
{
    GString* jieri;
    jieri=g_string_new("");

    gchar* cfgfile;
    GKeyFile* keyfile;
    gint weekday, weekth;
    gchar* str_day;

    keyfile = g_key_file_new();

    cfgfile = g_build_filename(LUNAR_HOLIDAYDIR, "holiday.dat", NULL);
#ifdef RUN_IN_SOURCE_TREE
    if (!g_file_test(cfgfile, G_FILE_TEST_IS_REGULAR))
    {
        cfgfile = g_build_filename("data", "holiday.dat", NULL);
        if (!g_file_test(cfgfile, G_FILE_TEST_IS_REGULAR))
        {
            cfgfile = g_build_filename(".", "holiday.dat", NULL);
            if (!g_file_test(cfgfile, G_FILE_TEST_IS_REGULAR))
            {
                cfgfile = g_build_filename("..", "holiday.dat", NULL);
            }
        }
    }
#endif

    if (!g_key_file_load_from_file(keyfile, cfgfile, G_KEY_FILE_KEEP_COMMENTS, NULL))
    {
        ;
    }

    if (g_key_file_has_group(keyfile, "LUNAR"))
    {
        str_day = g_strdup_printf("%02d%02d", date->lunar->month, date->lunar->day);
        if (g_key_file_has_key (keyfile, "LUNAR", str_day, NULL))
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, g_key_file_get_value (keyfile, "LUNAR", str_day, NULL));
        }
    }

    if (g_key_file_has_group(keyfile, "SOLAR"))
    {
        str_day = g_strdup_printf("%02d%02d", date->solar->month, date->solar->day);
        if (g_key_file_has_key (keyfile, "SOLAR", str_day, NULL))
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, g_key_file_get_value (keyfile, "SOLAR", str_day, NULL));
        }
    }

    weekday = get_day_of_week ( date->solar->year, date->solar->month, date->solar->day);
    weekth = get_weekth_of_month ( date->solar->day);
    if (g_key_file_has_group(keyfile, "WEEK"))
                                                                  {
        str_day = g_strdup_printf("%02d%01d%01d", date->solar->month, weekth, weekday);
        if (g_key_file_has_key (keyfile, "WEEK", str_day, NULL))
        {
            jieri=g_string_append(jieri, " ");
            jieri=g_string_append(jieri, g_key_file_get_value (keyfile, "WEEK", str_day, NULL));
        }
    }
    gchar* oo = g_strdup(g_strstrip(jieri->str));
    g_string_free(jieri, TRUE);
    g_free(str_day);
    return oo;
}

/**
 * lunar_date_strftime:
 * @date: a #LUNARDate
 * @format: specify the output format. this
 *
 * 使用给定的格式来输出字符串。类似于strftime的用法。可使用的格式及输出如下：
 *
 * %(YEAR)年%(MONTH)月%(DAY)日      公历：大写->二OO八年一月二十一日
 *
 * %(year)年%(month)月%(day)日      公历：小写->2008年1月21日
 *
 * %(NIAN)年%(YUE)月%(RI)日%(SHI)时 阴历：大写->丁亥年腊月十四日
 *
 * %(nian)年%(yue)月%(ri)日%(shi)时 阴历：小写->2007年12月14日
 *
 * %(Y60)年%(M60)月%(D60)日%(H60)时 干支：大写->丁亥年癸丑月庚申日
 *
 * %(y60)年%(m60)月%(d60)日%(h60)时 干支：小写 not use
 *
 * %(Y8)年%(M8)月%(D8)日%(H8)时     八字：大写->丁亥年癸丑月庚申日
 *
 * %(y8)年%(m8)月%(d8)日%(h8)时     八字：小写 not use
 *
 * %(shengxiao)%(jieri)             生肖和节日->猪
 *
 * 使用%(jieri)时，如果此日没有节日，那么将为空。
 * 节日可以自定义，只要按照格式修改$(prefix)/share/liblunar/hodiday.dat即可。
 *
 * Return value: a newly-allocated output string, nul-terminated
 **/
gchar* lunar_date_strftime (LUNARDate *date, const char *format)
{
    gchar *s, *tmp;
    GString *str = g_string_new(format);

    //solar-upper case
    if (strstr(format, "%(YEAR)") != NULL)
    {
        tmp = num_2_hanzi(date->solar->year);
        str = g_string_replace(str, "%(YEAR)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(MONTH)") != NULL)
    {
        tmp = mday_2_hanzi(date->solar->month);
        str = g_string_replace(str, "%(MONTH)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(DAY)") != NULL)
    {
        tmp = mday_2_hanzi(date->solar->day);
        str = g_string_replace(str, "%(DAY)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(HOUR)") != NULL)
    {
        tmp = mday_2_hanzi(date->solar->hour);
        str = g_string_replace(str, "%(HOUR)", tmp, -1);
        g_free(tmp);
    }

    //solar-lower case
    if (strstr(format, "%(year)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->solar->year);
        str = g_string_replace(str, "%(year)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(month)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->solar->month);
        str = g_string_replace(str, "%(month)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(day)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->solar->day);
        str = g_string_replace(str, "%(day)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(hour)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->solar->hour);
        str = g_string_replace(str, "%(hour)", tmp, -1);
        g_free(tmp);
    }

    //lunar-upper case
    if (strstr(format, "%(NIAN)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->year]), _(zhi_list[date->zhi->year]));
        str = g_string_replace(str, "%(NIAN)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(YUE)") != NULL)
    {
        if (date->lunar->isleap)
            tmp = g_strdup_printf("%s%s", _("R\303\271n"), _(lunar_month_list[date->lunar->month-1]));
        else
            tmp = g_strdup_printf("%s", _(lunar_month_list[date->lunar->month-1]));
        str = g_string_replace(str, "%(YUE)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(RI)") != NULL)
    {
        str = g_string_replace(str, "%(RI)", _(lunar_day_list[date->lunar->day-1]), -1);
    }
    if (strstr(format, "%(SHI)") != NULL)
    {
        str = g_string_replace(str, "%(SHI)", _(zhi_list[date->lunar->hour/2]), -1);
    }

    //lunar-lower case
    if (strstr(format, "%(nian)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->lunar->year);
        str = g_string_replace(str, "%(nian)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(yue)") != NULL)
    {
        if (date->lunar->isleap)
            tmp = g_strdup_printf("*%d", date->lunar->month);
        else
            tmp = g_strdup_printf("%d", date->lunar->month);
        str = g_string_replace(str, "%(yue)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(ri)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->lunar->day);
        str = g_string_replace(str, "%(ri)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(shi)") != NULL)
    {
        tmp = g_strdup_printf("%d", date->lunar->hour);
        str = g_string_replace(str, "%(shi)", tmp, -1);
        g_free(tmp);
    }

    //ganzhi
    if (strstr(format, "%(Y60)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->year]), _(zhi_list[date->zhi->year]));
        str = g_string_replace(str, "%(Y60)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(M60)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->month]), _(zhi_list[date->zhi->month]));
        str = g_string_replace(str, "%(M60)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(D60)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan->day]), _(zhi_list[date->zhi->day]));
        str = g_string_replace(str, "%(D60)", tmp, -1);
        g_free(tmp);
    }

    //bazi
    if (strstr(format, "%(Y8)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->year]), _(zhi_list[date->zhi2->year]));
        str = g_string_replace(str, "%(Y8)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(M8)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->month]), _(zhi_list[date->zhi2->month]));
        str = g_string_replace(str, "%(M8)", tmp, -1);
        g_free(tmp);
    }
    if (strstr(format, "%(D8)") != NULL)
    {
        tmp = g_strdup_printf("%s%s", _(gan_list[date->gan2->day]), _(zhi_list[date->zhi2->day]));
        str = g_string_replace(str, "%(D8)", tmp, -1);
        g_free(tmp);
    }

    //shengxiao
    if (strstr(format, "%(shengxiao)") != NULL)
    {
        str = g_string_replace(str, "%(shengxiao)", _(shengxiao_list[date->zhi->year]), -1);
    }

    //jieri
    if (strstr(format, "%(jieri)") != NULL)
    {
        if (strstr(lunar_date_get_jieri(date), " " ) != NULL)
        {
            char** buf = g_strsplit(lunar_date_get_jieri(date), " ", -1);
            tmp = strdup(*buf);
            g_strfreev(buf);
        }
        else
            tmp = strdup(lunar_date_get_jieri(date));
        str = g_string_replace(str, "%(jieri)", tmp, -1);
        g_free(tmp);
    }

    s = strdup(str->str);
    g_string_free(str, TRUE);
    return s;
}

/**
 * lunar_date_free:
 * @date: a #LUNARDate
 *
 * Frees a #LUNARDate returned from lunar_date_new().
 **/
void            lunar_date_free                   (LUNARDate *date)
{
    g_return_if_fail (date != NULL);

    g_free(date->solar);
    g_free(date->lunar);
    g_free(date->lunar2);
    g_free(date->gan);
    g_free(date->zhi);
    g_free(date->gan2);
    g_free(date->zhi2);
    g_free(date);

}

static void _cl_date_calc_lunar(LUNARDate *date, GError **error)
{
    glong days;
    GError *calc_error = NULL;

    CLDate *d;
    date->days = _date_calc_days_since_reference_year(date->solar, &calc_error) ;
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    date->days -=  _date_calc_days_since_reference_year(&first_solar_date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    /* A lunar day begins at 11 p.m. */
    if (date->solar->hour == 23)
        date->days ++;

    _cl_date_days_to_lunar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    date->lunar->hour = date->solar->hour;
}

static void _cl_date_calc_solar(LUNARDate *date, GError **error)
{
    GError *calc_error = NULL;
    _date_calc_days_since_lunar_year(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    _cl_date_days_to_solar(date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    date->solar->hour = date->lunar->hour;
}

//TODO
static void _cl_date_calc_ganzhi(LUNARDate *date)
{
    int	year, month;
    year = date->lunar->year - first_lunar_date.year;
    month = year * 12 + date->lunar->month - 1;   /* leap months do not count */

    date->gan->year = (first_gan_date.year + year) % 10;
    date->zhi->year = (first_zhi_date.year + year) % 12;
    date->gan->month = (first_gan_date.month + month) % 10;
    date->zhi->month = (first_zhi_date.month + month) % 12;
    date->gan->day = (first_gan_date.day + date->days) % 10;
    date->zhi->day = (first_zhi_date.day + date->days) % 12;
    date->zhi->hour = ((date->lunar->hour + 1) / 2) % 12;
    date->gan->hour = (date->gan->day * 12 + date->zhi->hour) % 10;
}

static void _cl_date_calc_bazi(LUNARDate *date)
{
    int	year, month;

    _cl_date_get_bazi_lunar(date);

    year = date->lunar2->year - first_lunar_date.year;
    month = year * 12 + date->lunar2->month - 1;   /* leap months do not count */

    date->gan2->year = (first_gan_date.year + year) % 10;
    date->zhi2->year = (first_zhi_date.year + year) % 12;
    date->gan2->month = (first_gan_date.month + month) % 10;
    date->zhi2->month = (first_zhi_date.month + month) % 12;
    date->gan2->day = (first_gan_date.day + date->days) % 10;
    date->zhi2->day = (first_zhi_date.day + date->days) % 12;
    date->zhi2->hour = ((date->lunar2->hour + 1) / 2) % 12;
    date->gan2->hour = (date->gan2->day * 12 + date->zhi2->hour) % 10;
}

/* Compute the number of days from the Solar date BYEAR.1.1 */
/* 返回从阳历年1201.1.1日经过的天数 */
//long Solar2Day1(CLDate *d)
static glong _date_calc_days_since_reference_year (CLDate *d, GError **error)
{
    glong days, delta;
    int i;

    delta = d->year - REFERENCE_YEAR;
    if (delta<0) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_INTERNAL,
                _("Internal error: pick a larger constant for BYEAR."));
        return 0;
    }
    days = delta * 365 + delta / 4 - delta / 100 + delta / 400;
    for (i=1; i< d->month; i++)
        days += days_in_solar_month[i];
    if ((d->month > 2) && leap(d->year))
        days++;
    days += d->day - 1;

    if ((d->month == 2) && leap(d->year))
    {
        if (d->day > 29) 
        {
            g_set_error(error, LUNAR_DATE_ERROR,
                    LUNAR_DATE_ERROR_DAY,
                    _("Day out of range: \"%d\""),
                    d->day);
            return 0;
        }
    }
    else if (d->day > days_in_solar_month[d->month]) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_DAY,
                _("Day out of range: \"%d\""),
                d->day);
        return 0;
    }
    return days;
}
/* Compute offset days of a lunar date from the beginning of the table */
static void _date_calc_days_since_lunar_year (LUNARDate *date, GError **error)
{
    int year, i, m, leap_month;

    date->days = 0;
    year = date->lunar->year - first_lunar_date.year;
    for (i=0; i<year; i++)
        date->days += date->lunar_year_days[i];

    leap_month = _cl_date_make_lunar_month_days(date, year);
    if ((date->lunar->isleap) && (leap_month!=date->lunar->month))
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_LEAP,
                _("%d is not a leap month in year %d.\n"), 
                date->lunar->month, date->lunar->year);
        return;
    }
    for (m=1; m<date->lunar->month; m++)
        date->days+= date->lunar_month_days[m];
    if (leap_month 
            && ((date->lunar->month>leap_month) 
                || (date->lunar->isleap && (date->lunar->month==leap_month))
               ))
        date->days += date->lunar_month_days[m++];
    date->days += date->lunar->day - 1;

    if (date->lunar->day > date->lunar_month_days[m]) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_DAY,
                _("Day out of range: \"%d\""),
                date->lunar->day);
        return;
    }
}

static void _cl_date_days_to_lunar (LUNARDate *date, GError **error)
{
    int i, m, nYear, leap_month;

    glong offset = date->days;
    for (i=0; i<NUM_OF_YEARS && offset > 0; i++)
        offset -= date->lunar_year_days[i];
    if (offset <0)
        offset += date->lunar_year_days[--i];
    if (i==NUM_OF_YEARS) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_DAY,
                _("Year out of range. \"%d\""),
                date->solar->year);
        return;
    }
    date->lunar->year = i + first_lunar_date.year;

    leap_month = _cl_date_make_lunar_month_days(date, i);
    for (m=1; m<=NUM_OF_MONTHS && offset >0; m++)
        offset -= date->lunar_month_days[m];
    if (offset <0)
        offset += date->lunar_month_days[--m];

    date->lunar->isleap = FALSE;	/* don't know leap or not yet */

    if (leap_month>0)	/* has leap month */
    {
        /* if preceeding month number is the leap month,
           this month is the actual extra leap month */
        date->lunar->isleap = (gboolean)(leap_month == (m - 1));

        /* month > leap_month is off by 1, so adjust it */
        if (m > leap_month) --m;
    }

    date->lunar->month = m;
    date->lunar->day = offset + 1;
}

static void _cl_date_days_to_solar(LUNARDate *date, GError **error)
{
    GError *calc_error = NULL;
    gint	adj, i, m, days;
    glong offset;

    adj = (date->lunar->hour == 23)? -1 : 0;
    offset = date->days;
    offset += adj;
    offset -= _date_calc_days_since_reference_year(&first_lunar_date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    offset += _date_calc_days_since_reference_year(&first_solar_date, &calc_error);
    if (calc_error)
    {
        g_propagate_error (error, calc_error);
        return;
    }
    /* offset is now the number of days from SolarFirstDate.year.1.1 */

    for (i=first_solar_date.year;
            (i<first_solar_date.year+NUM_OF_YEARS) && (offset > 0);	 i++) 
        offset -= 365 + leap(i);
    if (offset<0)
    {
        --i; 	/* LeapYear is a macro */
        offset += 365 + leap(i);
    }
    if (i==(first_solar_date.year + NUM_OF_YEARS)) 
    {
        g_set_error(error, LUNAR_DATE_ERROR,
                LUNAR_DATE_ERROR_DAY,
                _("Year out of range. \"%d\""),
                i);
        return;
    }
    date->solar->year = i;

    /* assert(offset<(365+LeapYear(i))); */
    for (m=1; m<=12; m++)
    {
        days = days_in_solar_month[m];
        if ((m==2) && leap(i))	/* leap February */
            days++;
        if (offset<days)
        {
            date->solar->month = m;
            date->solar->day = offset + 1;
            return;
        }
        offset -= days;
    }
}

static void _cl_date_make_all_lunar_data(LUNARDate *date)
{
    gint year, i, leap;
    long code;

    for (year = 0; year < NUM_OF_YEARS; year++)
    {
        code = years_info[year];
        leap = code & 0xf;
        date->lunar_year_days[year] = 0;
        if (leap != 0)
        {
            i = (code >> 16) & 0x1;
            date->lunar_year_days[year] += days_in_lunar_month[i];
        }
        code >>= 4;
        for (i = 0; i < NUM_OF_MONTHS-1; i++)
        {
            date->lunar_year_days[year] += days_in_lunar_month[code&0x1];
            code >>= 1;
        }
        date->lunar_year_months[year] = 12;
        if (leap != 0) 
            date->lunar_year_months[year]++;
    }
}

/* Compute the days of each month in the given lunar year */
static gint _cl_date_make_lunar_month_days(LUNARDate *date, gint year)
{
    int i, leap_month;
    long code;

    code = years_info[year];
    leap_month = code & 0xf;
    code >>= 4;
    if (leap_month == 0)
    {
        date->lunar_month_days[NUM_OF_MONTHS] = 0;
        for (i = NUM_OF_MONTHS-1; i >= 1; i--)
        {
            date->lunar_month_days[i] = days_in_lunar_month[code&0x1];
            code >>= 1;
        }
    }
    else
    {
        /* 
           There is a leap month (run4 yue4) L in this year.
           mday[1] contains the number of days in the 1-st month;
           mday[L] contains the number of days in the L-th month;
           mday[L+1] contains the number of days in the L-th leap month;
           mday[L+2] contains the number of days in the L+1 month, etc.

           cf. years_info[]: info about the leap month is encoded differently.
           */
        i = (years_info[year] >> 16) & 0x1;
        date->lunar_month_days[leap_month+1] = days_in_lunar_month[i];
        for (i = NUM_OF_MONTHS; i >= 1; i--)
        {
            if (i == leap_month+1) 
                i--;
            date->lunar_month_days[i] = days_in_lunar_month[code&0x1];
            code >>= 1;
        }
    }
    return leap_month;
}

/* Compare two dates and return <,=,> 0 if the 1st is <,=,> the 2nd */
static gint _cl_date_get_bazi_lunar (LUNARDate *date)
{
    int m, flag;

    if (date->solar->month==1)
    {
        flag = _cmp_date(date->solar->month, date->solar->day,
                1, fest[date->solar->year - first_solar_date.year - 1][11]);
        if (flag<0) 
            date->lunar2->month = 11;
        else if (flag>0) 
            date->lunar2->month = 12;
        date->lunar2->year = date->solar->year - 1;
        return(flag==0);
    }
    for (m=2; m<=12; m++)
    {
        flag = _cmp_date(date->solar->month, date->solar->day,
                m, fest[date->solar->year - first_solar_date.year][m-2]);
        if (flag==0) m++;
        if (flag<=0) break;
    }
    date->lunar2->month = (m-2) % 12;
    date->lunar2->year = date->solar->year;
    if ((date->lunar2->month)==0)
    {
        date->lunar2->year = date->solar->year - 1;
        date->lunar2->month = 12;
    }
    return(flag==0);
}

/**
 * get_day_of_week:
 * @year: year
 * @month: month of year
 * @day: day of month
 *
 * calc weekday by year, month, day.
 *
 * Return value: week day.
 **/
static gint get_day_of_week (gint year, gint month, gint day)
{
    int val;
    if ((month == 1) || (month == 2))
    {
        month += 12;
        year--;
    }

    val = (day + 2*month + 3*(month+1)/5 + year + year/4 - year/100 + year/400 +1 ) % 7;
    return val;
}

/**
 * get_weekth_of_month:
 * @day: 日。
 *
 * 计算是本月第几个(星期几)
 * 
 * Return value: which.
 **/
static gint get_weekth_of_month (gint day)
{
    gint a=1;
    while(day -7 >0)
    {
        day = day -7;
        a++;
    }
    return a;
}

static int
mymemfind(const char *mem, int len, const char *pat, int pat_len)
{
	register int ii;

	len -= pat_len;

	for (ii = 0; ii <= len; ii++) {
		if (mem[ii] == pat[0] && memcmp(&mem[ii], pat, pat_len) == 0) {
			return ii;
		}
	}
	return -1;
}


static int
mymemcnt(const char *mem, int len, const char *pat, int pat_len)
{
	register int offset = 0;
	int nfound = 0;

	while (len >= 0) {
		offset = mymemfind(mem, len, pat, pat_len);
		if (offset == -1)
			break;
		mem += offset + pat_len;
		len -= offset + pat_len;
		nfound++;
	}
	return nfound;
}

static GString* g_string_replace (GString *string, const gchar* old, const gchar* new, int count)
{
    int nfound, offset;

    int old_len = strlen(old);
    int new_len = strlen(new);

    if (string->len == 0 || (old_len == 0 && new_len == 0) || old_len > string->len)
        goto return_same;

    nfound = (old_len > 0) ? mymemcnt(string->str, string->len, old, old_len) : string->len + 1;
    if (count < 0)
        count = INT_MAX;
    else if (nfound > count)
        nfound = count;
    if (nfound == 0)
        goto return_same;

    GString *new_str = g_string_new("");

    if (old_len > 0) {
        for (; nfound > 0; --nfound) {
            offset = mymemfind(string->str, string->len, old, old_len);
            if (offset == -1)
                break;

            new_str = g_string_append_len(new_str, string->str, offset);
            string->str += offset + old_len;
            string->len -= offset + old_len;

            new_str = g_string_append(new_str, new);
        }
        if (string->len > 0)
            new_str = g_string_append_len(new_str, string->str, string->len);
    }
    else {
        for (;;++string->str, --string->len) {
            g_string_append(new_str, new);
            if (--nfound <= 0) {
                g_string_append_len(new_str, string->str, string->len);
                break;
            }
            g_string_append_c(new_str, *string->str);
        }
    }
    g_string_free(string, FALSE);
    return new_str;

return_same:
    return string;
}

char* num_2_hanzi(int n)
{
    GString *str = g_string_new("");
    int d;
    while (n > 10)
    {
        d = n % 10;
        n = n/10;
        str = g_string_prepend(str, _(hanzi_num[d]));
    }
    str = g_string_prepend(str, _(hanzi_num[n]));

    gchar* ret = g_strdup(g_strstrip(str->str));
    g_string_free(str, TRUE);
    return ret;
}

char* mday_2_hanzi(int n)
{
    GString *str = g_string_new("");
    int d;

    if ((n % 10) == 0)
    {
        n /= 10;
        str = g_string_append(str, _(hanzi_num[n]));
        str = g_string_append(str, _(hanzi_num[10]));
    }
    else if ((n / 10) == 1)
    {
        n = n % 10;
        str = g_string_append(str, _(hanzi_num[10]));
        str = g_string_append(str, _(hanzi_num[n]));
    }
    else if (n > 10)
    {
        d = n % 10;
        n = n/10;
        str = g_string_append(str, _(hanzi_num[n]));
        str = g_string_append(str, _(hanzi_num[10]));
        str = g_string_append(str, _(hanzi_num[d]));
    }
    else
        str = g_string_append(str, _(hanzi_num[n]));

    gchar* ret = g_strdup(g_strstrip(str->str));
    g_string_free(str, TRUE);
    return ret;
}
