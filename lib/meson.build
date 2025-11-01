imgui_srcs = [
  'imgui.cpp',
  'imgui_demo.cpp',
  'imgui_draw.cpp',
  'imgui_impl_opengl3.cpp',
  'imgui_impl_sdl3.cpp',
  'imgui_tables.cpp',
  'imgui_widgets.cpp',
  'implot.cpp',
  'implot_demo.cpp',
  'implot_items.cpp',
  'ImNodeFlow.cpp',
]
imgui_sharedlib = shared_library(
  'imguilib',
  imgui_srcs,
  dependencies: dependency('sdl3'),
  cpp_args: [
    '-Wall',
    '-Wextra',
    '-pedantic',
    '-mavx',
    '-march=native',
  ],
)
imgui_staticlib = static_library(
  'imguilib',
  imgui_srcs,
  dependencies: dependency('sdl3'),
  cpp_args: [
    '-Wall',
    '-Wextra',
    '-pedantic',
    '-mavx',
    '-march=native',
  ],
)
