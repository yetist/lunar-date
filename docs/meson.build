conf = configuration_data()
conf.set('PACKAGE_VERSION', meson.project_version())
version_xml = configure_file(input: 'version.xml.in',
	output: 'version.xml',
	configuration: conf)

private_headers = [
  'config.h',
  'lunar-date-private.h',
  'lunar-date-utils.h',
]

glib_prefix = dependency('glib-2.0').get_variable(pkgconfig : 'prefix')

glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')
docpath = join_paths(get_option('datadir'), 'gtk-doc', 'html')

gnome.gtkdoc(
	'lunar-date',
	main_xml : 'lunar-date-docs.xml',
	src_dir: [
		join_paths(meson.project_source_root(), 'lunar-date'),
		join_paths(meson.project_build_root(), 'lunar-date'),
		],
	dependencies: liblunar_date_deps,
	gobject_typesfile: 'lunar-date.types',
	scan_args: [
		'--rebuild-types',
		'--ignore-headers=' + ' '.join(private_headers),
		],
	fixxref_args: [
		'--html-dir=@0@'.format(docpath),
		'--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
		'--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
		'--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio')),
		],
	install : true
	)
