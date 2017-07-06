conf = configuration_data()
conf.set('PACKAGE_VERSION', meson.project_version())
version_xml = configure_file(input: 'version.xml.in',
    output: 'version.xml',
    configuration: conf)

gnome.gtkdoc(
    'lunar-date',
    main_xml : 'lunar-date-docs.xml',
    src_dir : [join_paths(meson.source_root(), 'lunar-date'),join_paths(meson.build_root(), 'lunar-date')],
    scan_args: '--deprecated-guards="GLIB_DEPRECATED"',
    mkdb_args : '--xml-mode --output-format=xml',
    dependencies: [gio_dep],
    install : true
    )
