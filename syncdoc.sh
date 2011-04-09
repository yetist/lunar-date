#!/bin/sh
set -x

CWD=`pwd`
TMPDIR=`mktemp -d`

MODULE="lunar-calendar"
SVNROOT="https://liblunar.googlecode.com/svn"
CONFIGARGS="--enable-gtk-doc"

check_module()
{
	if svn list $SVNROOT/docs/ | grep $MODULE; then
		echo "$MODULE exists";
	else
		svn mkdir -m "mkdir $MODULE" $SVNROOT/docs/$MODULE;
	fi
}

build_html()
{
	cd $TMPDIR;
	svn checkout $SVNROOT/docs/$MODULE
	mkdir $TMPDIR/tmp
	#svn checkout $SVNROOT/trunk/$MODULE $TMPDIR/tmp/$MODULE
	svn checkout $SVNROOT/trunk/liblunar-gtk $TMPDIR/tmp/$MODULE
	cd $TMPDIR/tmp/$MODULE
	./autogen.sh $CONFIGARGS
	make
	cp docs/reference/html/* $TMPDIR/$MODULE/
	cd $TMPDIR/$MODULE
	for i in $TMPDIR/$MODULE/*.html
	do
		svn propset "svn:mime-type" "text/html" $i
	done
}

echo $TMPDIR
#svn checkout https://liblunar.googlecode.com/svn/docs/$MODULE

check_module
build_html
