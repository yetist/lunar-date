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

	infobegin "Configure"
	meson setup _build --prefix=/usr -Ddocs=true -Dintrospection=true -Dvapi=true -Dtests=true
	infoend

	infobegin "Build"
	meson compile -C _build
	infoend

	infobegin "Test"
	ninja -C _build test
	infoend
fi
