project('lsd', 'cpp')

add_global_arguments('-Wno-narrowing', language: 'cpp')

#psycross_dep = dependency('psycross')

cmake = import('cmake')

cmake_proj = cmake.subproject('psycross')
psycross_dep = cmake_proj.dependency('psycross_static')
psycross_headers_dep = cmake_proj.dependency('psx_headers')

sources = [
    'src/main.cc',
    'src/entity.cc',
]

executable('lsd', sources,
    dependencies: [psycross_dep, psycross_headers_dep],
    #include_directories: cmake_proj.include_directories('include/psx'),
)
