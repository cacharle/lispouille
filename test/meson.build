test_sources = files(
  'test_token.c',
  'test_lexer.c',
  '../src/token.c',
  '../src/lexer.c',
)

cc = meson.get_compiler('c')
criterion_dep = cc.find_library('criterion', required : true)
test_exec = executable(
  'test_criterion',
  test_sources,
  # default_options : ['debug'],
  # c_args : ['--coverage'],
  # link_with : lib,
  include_directories : include_directories('../src'),
  dependencies : criterion_dep
)
test('test', test_exec)
