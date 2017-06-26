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

#gnome.mkenums(
#    source: ,
#    c_template: lunar-date-enum-types.h.template,
#    )

resources_src = gnome.compile_resources(
  'lunar-date-resources',
  'lunar-date.gresource.xml',
  source_dir : '.',
  c_name : 'lunar_date'
)
