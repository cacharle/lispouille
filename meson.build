project(
  'lispouille',
  'c',
  default_options : ['c_std=c99'],
  # c_args : ['Wall', 'Wextra']
)
# b_coverage=true
# MAP_ANONYMOUS not defined when using meson
add_global_arguments('-g', language : 'c')
# add_global_arguments('--coverage', language : 'c')
# incdir = include_directories('include')
subdir('src')
executable('lispouille', sources)
# subdir('test')
