add_library(libslvmutils SHARED
    utils.cpp
)

target_include_directories(libslvmutils PRIVATE "${PROJECT_SOURCE_DIR}")