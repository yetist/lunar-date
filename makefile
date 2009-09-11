all:
	gcc -g -o test `pkg-config --cflags --libs gtk+-2.0` test.c lunar-calendar.c -llunar-1 -I /usr/include/liblunar-1/lunar/
	gcc -g -o testcalendar `pkg-config --cflags --libs gtk+-2.0` testcalendar.c lunar-calendar.c -llunar-1 -I /usr/include/liblunar-1/lunar/
