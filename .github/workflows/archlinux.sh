#!/usr/bin/bash

# Use grouped output messages
infobegin() {
	echo "::group::${1}"
}
infoend() {
	echo "::endgroup::"
}

# Required packages on Archlinux
requires=(
	ccache
	clang
	file
	gcc
	git
	glib2
	glib2-devel
	gobject-introspection
	intltool
	itstool
	meson
	pkgconf
	vala
	gi-docgen
)

infobegin "Update system"
pacman --noconfirm -Syu
infoend

infobegin "Install dependency packages"
pacman --noconfirm -S ${requires[@]}
infoend
