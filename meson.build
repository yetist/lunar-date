project('lunar-date', 'c',
  version : '2.9.2',
  license : 'LGPLv2.1+',
  default_options : [
	  'warning_level=1',
	  'buildtype=debugoptimized',
	  ],
  meson_version : '>=0.40.1'
)

# Versionning
lunar_date_version = meson.project_version()
version_arr = lunar_date_version.split('.')
lunar_date_major_version = version_arr[0].to_int()
lunar_date_minor_version = version_arr[1].to_int()
lunar_date_micro_version = version_arr[2].to_int()

apiversion = '1.0'
soversion = 0

if lunar_date_minor_version.is_odd()
  lunar_date_interface_age = 0
else
  lunar_date_interface_age = lunar_date_version_micro
endif

lunar_date_binary_age = lunar_date_minor_version * 100 + lunar_date_micro_version
lunar_date_lt_current = lunar_date_minor_version * 100 + lunar_date_micro_version - lunar_date_interface_age
lunar_date_lt_revision = lunar_date_interface_age
lunar_date_lt_age = lunar_date_binary_age - lunar_date_interface_age

# maintaining compatibility with the previous libtool versioning
# current = minor * 100 + micro - interface
# revision = interface

# Dependencies
glib_req_version = '>= 2.32.0'
glib_dep = dependency('glib-2.0', version: glib_req_version)
gio_dep = dependency('gio-2.0', version: glib_req_version)
gio_unix_dep = dependency('gio-unix-2.0', version: glib_req_version)

root_dir = include_directories('.')

conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())

# get suported warning flags
test_args = [
  '-fstack-protector-strong',
  '-Waggregate-return',
  '-Warray-bounds',
  '-Wcast-align',
  '-Wclobbered',
  '-Wdeclaration-after-statement',
  '-Wempty-body',
  '-Wextra',
  '-Wformat=2',
  '-Wformat-nonliteral',
  '-Wformat-security',
  '-Wformat-signedness',
  '-Wignored-qualifiers',
  '-Wimplicit-function-declaration',
  '-Winit-self',
  '-Winline',
  '-Wmissing-declarations',
  '-Wmissing-format-attribute',
  '-Wmissing-include-dirs',
  '-Wmissing-noreturn',
  '-Wmissing-parameter-type',
  '-Wmissing-prototypes',
  '-Wnested-externs',
  '-Wno-discarded-qualifiers',
  '-Wno-missing-field-initializers',
  '-Wno-strict-aliasing',
  '-Wno-suggest-attribute=format',
  '-Wno-unused-parameter',
  '-Wold-style-definition',
  '-Woverride-init',
  '-Wpacked',
  '-Wpointer-arith',
  '-Wredundant-decls',
  '-Wreturn-type',
  '-Wshadow',
  '-Wsign-compare',
  '-Wstrict-aliasing',
  '-Wstrict-prototypes',
  '-Wswitch-default',
  '-Wtype-limits',
  '-Wundef',
  '-Wuninitialized',
  '-Wunused-but-set-variable',
  '-Wwrite-strings'
]

cc = meson.get_compiler('c')
foreach arg: test_args
  if cc.has_argument(arg)
    add_project_arguments(arg, language : 'c')
  endif
endforeach

# enable full RELRO where possible
# FIXME: until https://github.com/mesonbuild/meson/issues/1140 is fixed
global_link_args = []
test_link_args = [
  '-Wl,-z,relro',
  '-Wl,-z,now',
]
foreach arg: test_link_args
  if cc.has_argument(arg)
    global_link_args += arg
  endif
endforeach
add_global_link_arguments(
  global_link_args,
  language: 'c'
)

gnome = import('gnome')
i18n = import('i18n')

conf.set('LUNAR_DATE_BINARY_AGE', lunar_date_binary_age)
conf.set_quoted('GETTEXT_PACKAGE', meson.project_name())
conf.set_quoted('LUNAR_DATE_LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))
conf.set_quoted('DATADIR', join_paths(get_option('prefix'), get_option('datadir')))
conf.set_quoted('LIBDIR', join_paths(get_option('prefix'), get_option('libdir')))
conf.set_quoted('BINDIR', join_paths(get_option('prefix'), get_option('bindir')))
conf.set_quoted('SYSCONFDIR', join_paths(get_option('prefix'), get_option('sysconfdir')))
conf.set_quoted('LOCALSTATEDIR', join_paths(get_option('prefix'), get_option('localstatedir')))
conf.set_quoted('LIBEXECDIR', join_paths(get_option('prefix'), get_option('libexecdir')))

configure_file(
  output : 'config.h',
  configuration : conf
)

add_global_arguments([
  '-DHAVE_CONFIG_H',
  '-I' + meson.build_root(),
], language: 'c')

subdir('po')
subdir('lunar-date')

#subdir('data')
#subdir('example')
subdir('service')
#subdir('tools')
#subdir('utils')
#subdir('vapi')

if get_option('enable-gtk-doc')
  subdir('docs')
endif

if get_option('enable-tests')
  subdir('tests')
endif