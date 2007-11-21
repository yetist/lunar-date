/* vi: set sw=4 ts=4: */
/*
 * gcl-date.c
 *
 * This file is part of ________.
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
#include "gcl-main.h"
#include "gcl-date.h"

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

struct _GCLDate
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
    N_("Jia3"),	N_("Yi3"),	 N_("Bing3"), N_("Ding1"), N_("Wu4"),
    N_("Ji3"),	N_("Geng1"), N_("Xin1"),  N_("Ren2"),  N_("Gui3")
};

static	char	*zhi_list[] = {
    N_("Zi3"),	N_("Chou3"),  N_("Yin2"),  N_("Mao3"),  N_("Chen2"), N_("Si4"),
    N_("Wu3"),	N_("Wei4"),	 N_("Shen1"), N_("You3"),  N_("Xu1"),   N_("Hai4")
};

static	char   *shengxiao_list[] = {
    N_("Mouse"), N_("Ox"), N_("Tiger"), N_("Rabbit"), N_("Dragon"), N_("Snake"),
    N_("Horse"), N_("Goat"), N_("Monkey"), N_("Rooster"), N_("Dog"), N_("Pig")
};
static	char   *lunar_month_list[] = {
    N_("Zheng1Yue4"), N_("Er4Yue4"), N_("San1Yue4"), N_("Si4Yue4"), N_("Wu3Yue4"), N_("liu6Yue4"),
    N_("Qi1Yue4"), N_("Ba1Yue4"), N_("Jiu3Yue4"), N_("Shi2Yue4"), N_("Shi2Yi1Yue4"), N_("Shi2Er4Yue4")
};
static	char   *lunar_day_list[] = {
    N_("Chu1Yi1"), N_("Chu1Er4"), N_("Chu1San1"), N_("Chu1Si4"), N_("Chu1Wu3"), 
    N_("Chu1liu6"), N_("Chu1Qi1"), N_("Chu1Ba1"), N_("Chu1Jiu3"), N_("Chu1Shi2"), 
    N_("Shi2Yi1"), N_("Shi2Er4"), N_("Shi1San1"), N_("Shi1Si4"), N_("Shi1Wu3"),
    N_("Shi1liu6"), N_("Shi1Qi1"), N_("Shi1Ba1"), N_("Shi1Jiu3"), N_("Er4Shi2"), 
    N_("Er4Yi1"), N_("Er4Er4"), N_("Er4San1"), N_("Er4Si4"), N_("Er4Wu3"), 
    N_("Er4liu6"), N_("Er4Qi1"), N_("Er4Ba1"), N_("Er4Jiu3"), N_("San1Shi2")
};

static gchar *solar_jieri[] ={
"0101 元旦节",
"0214 情人节",
"0308 妇女节",
"0312 植树节",
"0315 消费者",
"0401 愚人节",
"0422 地球日",
"0501 劳动节",
"0504 青年节",
"0601 儿童节",
"0801 建军节",
"0910 教师节",
"0918 九一八",
"1001 国庆节",
"1006 老人节",
"1224 平安夜",
"1225 圣诞节"
};

static gchar *lunar_jieri[] ={
"0101*春节",
"0115 元宵节",
"0505 端午节",
"0624 火把节",
"0625 火把节",
"0626 火把节",
"0707 七夕节",
"0715 中元节",
"0815 中秋节",
"0909 重阳节",
"1208 腊八节",
"1223 小年",
"0100 除夕"
};

static gchar *week_jieri[] ={
"0520 母亲节",
"0630 父亲节",
"1144 感恩节"
};

static void _cl_date_calc_lunar(GCLDate *date, GError **error);
static void _cl_date_calc_solar(GCLDate *date, GError **error);
static gint _cl_date_make_lunar_month_days(GCLDate *date, gint year);
static void _cl_date_make_all_lunar_data(GCLDate *date);
static void _cl_date_days_to_lunar (GCLDate *date, GError **error);
static void _cl_date_days_to_solar(GCLDate *date, GError **error);
static void _cl_date_calc_ganzhi(GCLDate *date);
static void _cl_date_calc_bazi(GCLDate *date);
static gint _cl_date_get_bazi_lunar (GCLDate *date);
static glong _date_calc_days_since_reference_year (CLDate *d, GError **error);
static void _date_calc_days_since_lunar_year (GCLDate *date, GError **error);

GQuark gcl_date_error_quark (void)
{
    static GQuark quark = 0;

    if (G_UNLIKELY (quark == 0))
        quark = g_quark_from_static_string ("gcl-date-error-quark");

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
 * gcl_date_new:
 *
 * Allocates a #GCLDate and initializes it. Free the return value with gcl_date_free().
 *
 * Return value: a newly-allocated #GCLDate
 **/
GCLDate*     gcl_date_new                (void)
{
    GCLDate *date = g_new0 (GCLDate, 1);
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
 * gcl_date_set_solar_date:
 * @date: a #GCLDate.
 * @year: year to set.
 * @month: month to set.
 * @day: day to set.
 * @hour: hour to set.
 * @error: location to store the error occuring, or NULL to ignore errors.
 *
 * Sets the solar year, month, day and the hour for a #GCLDate.
 **/
void            gcl_date_set_solar_date     (GCLDate *date,
        GDateYear year,
        GDateMonth month,
        GDateDay day,
        GDateHour hour,
        GError **error)
{
    GError *calc_error = NULL;

    if (!(year>=BEGIN_YEAR && year< BEGIN_YEAR+NUM_OF_YEARS))
    {
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_YEAR,
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
 * gcl_date_set_lunar_date:
 * @date: a #GCLDate.
 * @year: year to set.
 * @month: month to set.
 * @day: day to set.
 * @hour: hour to set.
 * @isleap: indicate whether the month is a leap month.
 * @error: location to store the error occuring, or #NULL to ignore errors.
 *
 * Sets the lunar year, month, day and the hour for a #GCLDate. If the month is a leap month, you should set the isleap to TRUE.
 **/
void            gcl_date_set_lunar_date     (GCLDate *date,
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
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_YEAR,
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
 * gcl_date_get_solar_date:
 * @date: a #GCLDate.
 * @year: the solar year strings stored(e.g.,1995nian2).
 * @month: the solar month strings stored(e.g.,8yue4).
 * @day: the solar day strings stored(e.g., 23ri4).
 * @hour: the solar hour strings stored(e.g., 2shi2).
 *
 * Returns the solar date according the date. The date must be valid.
 *
 * Return value: a #CLDate stored the solar date.
 **/
CLDate*     gcl_date_get_solar_date     (GCLDate *date, gchar* year, gchar* month, gchar* day, gchar* hour)
{
    g_sprintf(year, "%u%s", date->solar->year, _("nian2"));
    g_sprintf(month, "%u%s", date->solar->month, _("yue4"));
    g_sprintf(day, "%u%s", date->solar->day, _("ri4"));
    g_sprintf(hour, "%u%s", date->solar->hour, _("shi2"));

    return date->solar;
}

/**
 * gcl_date_get_lunar_date:
 * @date: a #GCLDate.
 * @year: the lunar year strings stored(e.g.,1995nian2).
 * @month: the lunar month strings stored(e.g., Ba1yue4).
 * @day: the lunar day strings stored(e.g., Shi2Si4).
 * @hour: the lunar hour strings stored(e.g., Liu4shi2).
 *
 * Returns the lunar date according the date. The date must be valid.
 *
 * Return value: a #CLDate stored the lunar date.
 **/
CLDate*     gcl_date_get_lunar_date     (GCLDate *date, gchar* year, gchar* month, gchar* day, gchar* hour)
{

    g_sprintf(year, "%u%s", date->lunar->year, _("nian2"));
    if (date->lunar->isleap)
        g_sprintf(month, "%s%s", _("run4"), _(lunar_month_list[date->lunar->month-1]));
    else
        g_sprintf(month, "%s", _(lunar_month_list[date->lunar->month-1]));
    g_sprintf(day, "%s", _(lunar_day_list[date->lunar->day-1]));
    g_sprintf(hour, "%u%s", date->lunar->hour, _("shi2"));
    return date->lunar;
}

/**
 * gcl_date_get_shengxiao:
 * @date: a #GCLDate.
 *
 * Returns the Shengxiao of the date. The date must be valid.
 *
 * Return value: the new-allocated string stored Shengxiao.
 **/
gchar*          gcl_date_get_shengxiao          (GCLDate *date)
{
    gchar* shengxiao;
    shengxiao = g_strdup(_(shengxiao_list[date->zhi->year]));
    return shengxiao;
}


/**
 * gcl_date_get_ganzhi:
 * @date: a #GCLDate
 * @year: the Ganzhi year strings stored(e.g., Gui3Si4).
 * @month: the Ganzhi month strings stored(e.g., Jia3Zi3).
 * @day: the Ganzhi day strings stored(e.g., Ding1Mao3).
 * @hour: the Ganzhi hour strings stored(e.g.,Gui3Mao3).
 *
 * Returns the Ganzhi of the date. The date must be valid.
 *
 * Return value:  a newly-allocated Ganzhi string of the date(e.g., Gui3Si4nian2Jia3Zi3Ding1Mao3Gui3Mao3shi2).
 **/
gchar*          gcl_date_get_ganzhi (GCLDate *date, gchar* year, gchar* month, gchar* day, gchar* hour)
{
    g_sprintf(year, "%s%s", _(gan_list[date->gan->year]), _(zhi_list[date->zhi->year]));
    g_sprintf(month, "%s%s", _(gan_list[date->gan->month]), _(zhi_list[date->zhi->month]));
    g_sprintf(day, "%s%s", _(gan_list[date->gan->day]), _(zhi_list[date->zhi->day]));
    g_sprintf(hour, "%s%s", _(gan_list[date->gan->hour]), _(zhi_list[date->zhi->hour]));

    gchar* ganzhi = g_strdup_printf("%s%s%s%s%s%s%s%s%s%s", 
	   _(gan_list[date->gan->year]), _(zhi_list[date->zhi->year]), _("nian2"),
	   _(gan_list[date->gan->month]),_(zhi_list[date->zhi->month]),
	   _(gan_list[date->gan->day]),  _(zhi_list[date->zhi->day]),
	   _(gan_list[date->gan->hour]), _(zhi_list[date->zhi->hour]), _("shi2"));

    return ganzhi;
}

/**
 * gcl_date_get_bazi:
 * @date: a #GCLDate.
 * @year: the Bazi year strings stored(e.g., Gui3Si4).
 * @month: the Bazi month strings stored(e.g., Jia3Zi3).
 * @day: the Bazi day strings stored(e.g., Ding1Mao3).
 * @hour: the Bazi hour strings stored(e.g.,Gui3Mao3).
 *
 * Returns the Bazi of the date. The date must be valid.
 *
 * Return value:  a newly-allocated Bazi string of the date(e.g., Gui3Si4nian2Jia3Zi3Ding1Mao3Gui3Mao3shi2).
 **/
gchar*          gcl_date_get_bazi (GCLDate *date, gchar* year, gchar* month, gchar* day, gchar* hour)
{
    g_sprintf(year, "%s%s", _(gan_list[date->gan2->year]),  _(zhi_list[date->zhi2->year]));
    g_sprintf(month, "%s%s",_(gan_list[date->gan2->month]), _(zhi_list[date->zhi2->month]));
    g_sprintf(day, "%s%s",  _(gan_list[date->gan2->day]),   _(zhi_list[date->zhi2->day]));
    g_sprintf(hour, "%s%s", _(gan_list[date->gan2->hour]),  _(zhi_list[date->zhi2->hour]));

    gchar* bazi = g_strdup_printf("%s%s%s%s%s%s%s%s%s%s%s%s\n", 
	   _(gan_list[date->gan2->year]), _(zhi_list[date->zhi2->year]), _("nian2"),
	   _(gan_list[date->gan2->month]),_(zhi_list[date->zhi2->month]), _("yue4"),
	   _(gan_list[date->gan2->day]),  _(zhi_list[date->zhi2->day]), _("ri4"),
	   _(gan_list[date->gan2->hour]), _(zhi_list[date->zhi2->hour]), _("shi2"));

    return bazi;

}

/**
 * gcl_date_free:
 * @date: a #GCLDate
 *
 * Frees a #GCLDate returned from gcl_date_new().
 **/
void            gcl_date_free                   (GCLDate *date)
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

static void _cl_date_calc_lunar(GCLDate *date, GError **error)
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

static void _cl_date_calc_solar(GCLDate *date, GError **error)
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
static void _cl_date_calc_ganzhi(GCLDate *date)
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

static void _cl_date_calc_bazi(GCLDate *date)
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
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_INTERNAL,
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
            g_set_error(error, GCL_DATE_ERROR,
                    GCL_DATE_ERROR_DAY,
                    _("Day out of range: \"%d\""),
                    d->day);
            return 0;
        }
    }
    else if (d->day > days_in_solar_month[d->month]) 
    {
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_DAY,
                _("Day out of range: \"%d\""),
                d->day);
        return 0;
    }
    return days;
}
/* Compute offset days of a lunar date from the beginning of the table */
static void _date_calc_days_since_lunar_year (GCLDate *date, GError **error)
{
    int year, i, m, leap_month;

    date->days = 0;
    year = date->lunar->year - first_lunar_date.year;
    for (i=0; i<year; i++)
        date->days += date->lunar_year_days[i];

    leap_month = _cl_date_make_lunar_month_days(date, year);
    if ((date->lunar->isleap) && (leap_month!=date->lunar->month))
    {
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_LEAP,
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
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_DAY,
                _("Day out of range: \"%d\""),
                date->lunar->day);
        return;
    }
}

static void _cl_date_days_to_lunar (GCLDate *date, GError **error)
{
    int i, m, nYear, leap_month;

    glong offset = date->days;
    for (i=0; i<NUM_OF_YEARS && offset > 0; i++)
        offset -= date->lunar_year_days[i];
    if (offset <0)
        offset += date->lunar_year_days[--i];
    if (i==NUM_OF_YEARS) 
    {
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_DAY,
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

static void _cl_date_days_to_solar(GCLDate *date, GError **error)
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
        g_set_error(error, GCL_DATE_ERROR,
                GCL_DATE_ERROR_DAY,
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

static void _cl_date_make_all_lunar_data(GCLDate *date)
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
static gint _cl_date_make_lunar_month_days(GCLDate *date, gint year)
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
static gint _cl_date_get_bazi_lunar (GCLDate *date)
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

/*

gchar* get_jieri(JieRi type, int month, int day, gchar** jieri)
{
    int i;
    if (type == JIERI_SOLAR)
    {
        for (i=0; i < G_N_ELEMENTS(solar_jieri); i++)
        {
            gchar** b;
            b = g_strsplit(solar_jieri[i], " ", 2);
            gchar* str_day = g_strdup_printf("%02d%02d", month, day);
            if (g_ascii_strcasecmp(b[0], str_day) == 0)
            {
                *jieri = g_strdup(b[1]);
                g_strfreev(b);
                return (*jieri);
            }
            g_strfreev(b);
        }
    }
    else if (type == JIERI_LUNAR)
    {
        for (i=0; i < G_N_ELEMENTS(lunar_jieri); i++)
        {
            gchar** b;
            b = g_strsplit(lunar_jieri[i], " ", 2);
            gchar* str_day = g_strdup_printf("%02d%02d", month, day);
            if (g_ascii_strcasecmp(b[0], str_day) == 0)
            {
                *jieri = g_strdup(b[1]);
                g_strfreev(b);
                return (*jieri);
            }
            g_strfreev(b);
        }
    }
    return NULL;

}

gchar* get_week_jieri(int month, int weekth, int weekday, gchar** jieri)
{
    int i;
    for (i=0; i < G_N_ELEMENTS(week_jieri); i++)
    {
        gchar** b;
        b = g_strsplit(week_jieri[i], " ", 2);
        gchar* str_day = g_strdup_printf("%02d%01d%01d", month, weekth, weekday);
        if (g_ascii_strcasecmp(b[0], str_day) == 0)
        {
            *jieri = g_strdup(b[1]);
            g_strfreev(b);
            return (*jieri);
        }
        g_strfreev(b);
    }
    return NULL;

}
*/
