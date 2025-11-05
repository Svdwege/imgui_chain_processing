project(
  'nodeproc',
  'cpp',
  version: '0.0.1',
  default_options: [
    'c_std=c23',
    'cpp_std=c++23',
    #    'warning_level=everything',
  ],
)

args = [
  '-mavx',
  '-msse3',
  '-Wno-abi-tag',
  '-Wno-effc++',
  '-march=native',
]
add_global_arguments(args, language: 'cpp')
sdl3_dep = dependency('sdl3')
opengl_dep = dependency('opengl')

deps = [sdl3_dep, opengl_dep]

subdir('lib')

incs = include_directories(
  'lib',
  'src',
)

srcfiles = files(
  'src/application.cpp',
  'src/main.cpp',
  'src/stb_image.cpp',
)

exec = executable(
  'node_processing',
  srcfiles,
  include_directories: incs,
  link_with: imgui_sharedlib,
  #link_with: imgui_staticlib,
  dependencies: deps,
)
