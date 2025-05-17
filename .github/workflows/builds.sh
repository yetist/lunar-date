#!/usr/bin/bash

set -e
set -o pipefail

CPUS=$(grep processor /proc/cpuinfo | wc -l)

# Use grouped output messages
infobegin() {
	echo "::group::${1}"
}
infoend() {
	echo "::endgroup::"
}

if [ -f meson.build ]; then

	infobegin "Configure (meson)"
	meson setup _build --prefix=/usr -Ddocs=true -Dintrospection=true -Dvapi=true -Dtests=true
	infoend

	infobegin "Build (meson)"
	meson compile -C _build
	infoend

	infobegin "Test (meson)"
	ninja -C _build test
	infoend

	infobegin "Dist (meson)"
	# Git safedirectory stop ninja dist
	# https://github.com/git/git/commit/8959555cee7ec045958f9b6dd62e541affb7e7d9
	# https://git-scm.com/docs/git-config/2.35.2#Documentation/git-config.txt-safedirectory
	git config --global --add safe.directory ${PWD}
	ninja -C _build dist
	infoend
fi
