version_h = configure_file(
  input: 'lunar-date-version.h.in',
  output : 'lunar-date-version.h',
  configuration : conf
)

source_h = [
  'lunar-date.h',
  version_h,
]

source_c = [
  'lunar-date.c',
  'lunar-date-version.c',
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

install_headers(source_h, subdir : 'lunar-date')

liblunar_date = shared_library('lunar-date-3.0',
    source_c, source_enum, source_resources,
    dependencies: [glib_dep, gio_dep, gio_unix_dep],
    include_directories: top_srcdir,
    install : true,
    install_dir : get_option('libdir')
)
