project('nodeproc', 'cpp', version: '0.0.1')

sdl3_dep = dependency('sdl3')
opengl_dep = dependency('opengl')

deps = [sdl3_dep, opengl_dep]

subdir('lib')

incs = include_directories(
  'lib',
)

srcfiles = files(
  'src/main.cpp',
)

exec = executable(
  'node_processing',
  srcfiles,
  include_directories: incs,
  link_with: imgui_sharedlib,
  #link_with: imgui_staticlib,
  dependencies: deps,
)
