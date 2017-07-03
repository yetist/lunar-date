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

install_headers(source_h, subdir : join_paths('lunar-date-3.0', 'lunar-date'))

liblunar_date = shared_library('lunar-date-3.0',
    source_c, source_enum, source_resources,
    version: soversion,
    dependencies: [gio_dep],
    include_directories: top_srcdir,
    install : true,
    install_dir : get_option('libdir')
)

pkg=import('pkgconfig')
pkg.generate(
    name: 'lunar-date-3.0',
    version: meson.project_version(),
    description: 'Chinese Lunar Library',
    requires_private: 'gio-2.0',
    filebase: 'lunar-date-3.0',
    libraries: '-llunar-date-3.0',
    subdirs: 'lunar-date-3.0',
)

#if introspection_available
  gir = gnome.generate_gir(liblunar_date,
      sources: [source_h, source_c, source_enum.get(1)],
      namespace : 'LunarDate',
      nsversion : '3.0',
      symbol_prefix : 'lunar',
      identifier_prefix : 'Lunar',
      export_packages : 'lunar-date-3.0',
      includes : ['GObject-2.0'],
      install : true)
#endif

gnome.generate_vapi('lunar-date-3.0',
    sources: gir.get(0),
    metadata_dirs: '.',
    packages: 'gio-2.0',
    install : true,
)
