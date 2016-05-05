# Contributor: yetist <yetist@gmail.com>

pkgname=mingw32-lunar-date
_pkgname=lunar-date
pkgver=2.9.1
pkgrel=1
pkgdesc="Chinese lunar date library."
arch=("i686" "x86_64")
url="https://github.com/yetist/lunar-date"
license=('GPL2')
depends=(mingw32-glib2)
makedepends=(mingw32-gcc)
source=("http://liblunar.googlecode.com/files/$_pkgname-$pkgver.tar.gz")

build() {
  cd "$srcdir/$_pkgname-$pkgver"

  export CPPFLAGS="-D_REENTRANT"
  export PKG_CONFIG_LIBDIR=/usr/i486-mingw32/lib/pkgconfig
  unset PKG_CONFIG_PATH
  unset LDFLAGS

  ./configure \
    --prefix=/usr/i486-mingw32 \
    --host=i486-mingw32 \
    --build=$CHOST
  make V=1 || return 1
}

package() {
  cd "$srcdir/$_pkgname-$pkgver"

  make DESTDIR="$pkgdir/" install || return 1

  cd ${pkgdir}
  find . -name '*.a' -o -name '*.dll' -o -name '*.exe' \
    | xargs -rtl1 i486-mingw32-strip -g
  rm -rf usr/i486-mingw32/share/gtk-doc
}
