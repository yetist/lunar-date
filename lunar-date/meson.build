#wxtiso_sources = [
#  'main.c',
#]
#
#wxtiso_deps = [
#  dependency('glib-2.0'),
#]
#
#executable('wxtiso', wxtiso_sources,
#  dependencies: wxtiso_deps,
#  install: true,
#)

source_h = [
  'lunar-date.h',
  'lunar-version.h',
]

source_c = [
  'lunar-date.c',
  'lunar-version.c',
  'lunar-date-private.c',
]

source_enum = gnome.mkenums('lunar-date-enum-types',
    sources: source_h,
    h_template: 'lunar-date-enum-types.h.template',
    c_template: 'lunar-date-enum-types.c.template',
)

source_resources = gnome.compile_resources(
  'lunar-date-resources',
  'lunar-date.gresource.xml',
  source_dir : '.',
  c_name : 'lunar_date'
)

liblunar_date = shared_library('lunar-date-3.0',
    source_c, source_enum, source_resources,
    dependencies: [glib_dep, gio_dep, gio_unix_dep],
    include_directories: root_dir,
    install : true,
    install_dir : get_option('libdir')
)
