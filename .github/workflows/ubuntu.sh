#!/usr/bin/bash

# Use grouped output messages
infobegin() {
  echo "::group::${1}"
}
infoend() {
  echo "::endgroup::"
}

# Required packages on Ubuntu
# https://git.launchpad.net/ubuntu/+source/lunar-date/tree/debian/control
requires=(
  ccache
  gettext
  gi-docgen
  gir1.2-glib-2.0-dev
  git
  gobject-introspection
  gtk-doc-tools
  intltool
  libgirepository1.0-dev
  libglib2.0-dev
  meson
  valac
)

infobegin "Update system"
apt-get update -y
infoend

infobegin "Install dependency packages"
env DEBIAN_FRONTEND=noninteractive \
  apt-get install --assume-yes ${requires[@]}
infoend
