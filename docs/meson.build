toml_conf = configuration_data()
toml_conf.set('version', meson.project_version())

gidocgen = find_program('gi-docgen', required: get_option('docs'), native: true)

gidocgen_common_args = [
  '--quiet',
  '--no-namespace-dir',
  ]

if get_option('werror')
  gidocgen_common_args += ['--fatal-warnings']
endif

if get_option('docs') and not get_option('introspection')
  error('API reference requires introspection.')
endif

if get_option('docs')
  lunar_date_toml = configure_file(
    input: 'lunar-date.toml.in',
    output: 'lunar-date.toml',
    configuration: toml_conf,
    install: true,
    install_dir: datadir / 'doc' / 'lunar-date',
    )

  custom_target('lunar-date-doc',
    input: [ lunar_date_toml, liblunar_date_gir[0] ],
    output: 'lunar-date',
    command: [
      gidocgen,
      'generate',
      gidocgen_common_args,
      '--add-include-path=@0@'.format(meson.current_build_dir() / '../lunar-date'),
      '--config=@INPUT0@',
      '--output-dir=@OUTPUT@',
      '--content-dir=@0@'.format(meson.current_build_dir()),
      '--content-dir=@0@'.format(meson.current_source_dir()),
      '@INPUT1@',
      ],
    depends: liblunar_date_gir[0],
    build_by_default: true,
    install: true,
    install_dir: datadir / 'doc',
    install_tag: 'doc',
    )
endif
