#!/usr/bin/bash

# Use grouped output messages
infobegin() {
	echo "::group::${1}"
}
infoend() {
	echo "::endgroup::"
}

# Required packages on Debian
requires=(
	ccache
	gettext
	gi-docgen
	gir1.2-glib-2.0-dev
	git
	gobject-introspection
	intltool
	libglib2.0-dev
	meson
	valac
)

infobegin "Update system"
apt-get update -qq
infoend

infobegin "Install dependency packages"
apt-get install --assume-yes --no-install-recommends ${requires[@]}
infoend
