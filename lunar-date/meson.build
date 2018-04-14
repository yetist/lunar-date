gnome = import('gnome')
pkg = import('pkgconfig')

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
  'lunar-date-utils.c',
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

install_headers(source_h, subdir : 'lunar-date-3.0')

liblunar_date = shared_library('lunar-date-3.0',
  source_c, source_enum, source_resources,
  dependencies: [gio_dep],
  soversion: soversion,
  version: libversion,
  include_directories: top_srcdir,
  install : true,
  install_dir : get_option('libdir')
  )

liblunar_date_deps = declare_dependency(sources: source_h,
                                include_directories: [top_srcdir, include_directories('.')],
                                dependencies: [gio_dep, gio_unix_dep],
                                link_with: liblunar_date)
pkg.generate(
  name: 'lunar-date-3.0',
  version: meson.project_version(),
  description: 'Chinese Lunar Date Library',
  requires_private: 'gio-2.0',
  filebase: 'lunar-date-3.0',
  libraries: '-llunar-date-3.0',
  subdirs: 'lunar-date-3.0',
  )

if get_option('with_introspection')
  liblunar_date_gir = gnome.generate_gir(liblunar_date,
    sources: [source_h, source_c, source_enum.get(1)],
    nsversion : api_version,
    namespace : 'LunarDate',
    symbol_prefix : 'lunar',
    identifier_prefix : 'Lunar',
    link_with: liblunar_date,
    includes : ['GObject-2.0'],
    install : true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [ '--c-include=lunar-date.h' ],
    )
  if get_option('with_vala')
    lunar_date_vapi = gnome.generate_vapi('lunar-date-' + api_version,
      sources: liblunar_date_gir.get(0),
      packages: 'gio-2.0',
      install: true,
      install_dir: vapidir,
      )
  endif
endif
