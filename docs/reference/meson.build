gnome.gtkdoc(
  'lunar-date',
  main_xml : 'lunar-date-docs.xml',
  src_dir : [join_paths(meson.source_root(), 'lunar-date'),join_paths(meson.build_root(), 'lunar-date')],
  scan_args: '--deprecated-guards="GLIB_DEPRECATED"',
  mkdb_args : '--xml-mode --output-format=xml',
  dependencies: [glib_dep, gio_dep, gio_unix_dep],
  install : true
)
