gdbus_src = gnome.gdbus_codegen(
    'date-dbus-generated',
    'org.chinese.lunar.date.xml',
    interface_prefix : 'org.chinese.',
    namespace : 'Bus'
    )

executable(
    'lunar-date',
    gdbus_src,
    sources : 'lunar-date.c',
    include_directories : [
      include_directories('..'),
      ],
    dependencies : [gio_dep, gio_os_dep],
    install : true,
    install_dir : get_option('bindir')
    )

executable(
    'lunar-date-daemon',
    gdbus_src,
    sources : ['date-daemon.h', 'date-daemon.c', 'lunar-date-daemon.c'],
    dependencies : liblunar_date_deps,
    install : true,
    install_dir : join_paths(get_option('prefix'), get_option('libexecdir'), 'lunar-date')
    )

# no quoting
cdata = configuration_data()
cdata.set('pkglibexecdir', join_paths(get_option('prefix'), get_option('libexecdir'), 'lunar-date'))

# replace @pkglibexecdir@
configure_file(
    input : 'org.chinese.Lunar.Date.service.in',
    output : 'org.chinese.Lunar.Date.service',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'dbus-1/services'),
    configuration : cdata
    )
