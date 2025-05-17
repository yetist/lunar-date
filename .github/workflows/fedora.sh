#!/usr/bin/bash

# Use grouped output messages
infobegin() {
  echo "::group::${1}"
}
infoend() {
  echo "::endgroup::"
}

# Required packages on Fedora
requires=(
  ccache
  clang
  gcc
  git
  gi-docgen
  gobject-introspection-devel
  gtk-doc
  itstool
  meson
  vala
)

infobegin "Update system"
dnf update -y
infoend

infobegin "Install dependency packages"
dnf install -y ${requires[@]}
infoend
