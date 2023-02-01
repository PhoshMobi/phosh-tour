project('phosh-tour', 'c',
          version: '0.24.0',
    meson_version: '>= 0.56.0',
  default_options: [ 'warning_level=2',
                     'werror=false',
                     'c_std=gnu11',
                   ],
)

application_id = 'sm.puri.PhoshTour'
prefix = get_option('prefix')
libdir = join_paths(prefix, get_option('libdir'))
plugins_dir = join_paths(prefix, libdir, 'phosh', 'plugins')

glib_ver = '>= 2.62'
glib_dep = dependency('glib-2.0', version: glib_ver)
gio_dep = dependency('gio-2.0', version: glib_ver)
gtk_dep = dependency('gtk4', version: '>= 4.4')
adwaita_dep = dependency('libadwaita-1', version: '>= 1.1')

i18n = import('i18n')

gnome = import('gnome')

cc = meson.get_compiler('c')
global_c_args = []
test_c_args = [
  '-Wcast-align',
  '-Wdate-time',
  '-Wdeclaration-after-statement',
  ['-Werror=format-security', '-Werror=format=2'],
  '-Wendif-labels',
  '-Werror=incompatible-pointer-types',
  '-Werror=missing-declarations',
  '-Werror=overflow',
  '-Werror=return-type',
  '-Werror=shift-count-overflow',
  '-Werror=shift-overflow=2',
  '-Werror=implicit-fallthrough=3',
  '-Wfloat-equal',
  '-Wformat-nonliteral',
  '-Wformat-security',
  '-Winit-self',
  '-Wmaybe-uninitialized',
  '-Wmissing-field-initializers',
  '-Wmissing-include-dirs',
  '-Wmissing-noreturn',
  '-Wnested-externs',
  '-Wno-missing-field-initializers',
  '-Wno-sign-compare',
  '-Wno-strict-aliasing',
  '-Wno-unused-parameter',
  '-Wold-style-definition',
  '-Wpointer-arith',
  '-Wredundant-decls',
  '-Wshadow',
  '-Wstrict-prototypes',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wtype-limits',
  '-Wundef',
  '-Wunused-function',
]
if get_option('buildtype') != 'plain'
  test_c_args += '-fstack-protector-strong'
endif

foreach arg: test_c_args
  if cc.has_multi_arguments(arg)
    global_c_args += arg
  endif
endforeach
add_project_arguments(
  global_c_args,
  language: 'c'
)


config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('GETTEXT_PACKAGE', 'mobile-settings')
config_h.set_quoted('LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))
config_h.set_quoted('PHOSH_TOUR_BRAND', get_option('brand'))
config_h.set_quoted('PHOSH_TOUR_VENDOR', get_option('vendor'))
config_h.set_quoted('PHOSH_TOUR_APP_ID', application_id)

configure_file(
  output: 'phosh-tour-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.project_build_root(),
], language: 'c')

subdir('data')
subdir('src')
subdir('po')

summary({
     'Brand': get_option('brand'),
     'Vendor': get_option('vendor'),
  },
  bool_yn: true,
  section: 'Config',
)

# gnome.post_install() is available since meson 0.59.0
# Distributions use their own tooling (e.g. postinst, triggers, etc)
# so it is okay if the post_install() is not run on distro builds
if meson.version().version_compare('>=0.59.0')
  gnome.post_install(
    glib_compile_schemas: true,
    gtk_update_icon_cache: true,
    update_desktop_database: true,
  )
endif
