add_library(libslvmutils SHARED
    alloc.cpp
    filemap.cpp
    strtab.cpp
)

target_include_directories(libslvmutils PRIVATE "${PROJECT_SOURCE_DIR}")