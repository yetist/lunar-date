t=executable(
  'testing',
  sources : 'testing.c',
  include_directories : [
    include_directories('..'),
  ],
  dependencies : [gio_dep],
  link_with : liblunar_date,
  install : false,
)

test('lunar-date-testing', t)
