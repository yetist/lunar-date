#!/bin/sh

ECHO_C=
ECHO_N=
case `echo -n x` in
-n*)
  case `echo 'x\c'` in
  *c*) ;;
  *)   ECHO_C='\c';;
  esac;;
*)
  ECHO_N='-n';;
esac

# some terminal codes ...
boldface="`tput bold 2>/dev/null`"
normal="`tput sgr0 2>/dev/null`"
printbold() {
    echo $ECHO_N "$boldface" $ECHO_C
    echo "$@"
    echo $ECHO_N "$normal" $ECHO_C
}    

printbold Running libtoolize...
libtoolize --force --copy
printbold Running glib-gettextize...
glib-gettextize --force --copy
printbold Running intltoolize...
intltoolize --force --copy --automake
printbold Running gtkdocize...
gtkdocize --copy
printbold Running aclocal...
aclocal -I m4
printbold Running autoconf...
autoconf
printbold Running autoheader...
autoheader

if [ -f COPYING ]; then
	cp -pf COPYING COPYING.autogen_bak
fi
if [ -f INSTALL ]; then
	cp -pf INSTALL INSTALL.autogen_bak
fi

printbold Running automake...
automake --gnu --add-missing --force --copy

if [ -f COPYING.autogen_bak ]; then
	cmp COPYING COPYING.autogen_bak > /dev/null || cp -pf COPYING.autogen_bak COPYING
	rm -f COPYING.autogen_bak
fi
if [ -f INSTALL.autogen_bak ]; then
	cmp INSTALL INSTALL.autogen_bak > /dev/null || cp -pf INSTALL.autogen_bak INSTALL
	rm -f INSTALL.autogen_bak
fi

conf_flags="--prefix=/usr --enable-maintainer-mode --enable-gtk-doc"

printbold Running ./configure $conf_flags "$@" ...
./configure $conf_flags "$@" \
&& echo Now type \`make\' to compile lunar-date
