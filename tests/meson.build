executable(
  'testing',
  sources : 'testing.c',
  include_directories : [
    include_directories('..'),
  ],
  dependencies : [gio_dep, gio_unix_dep, glib_dep],
  link_with : liblunar_date,
  install : false,
)
