add_executable(cjlcc
	cjlcc.cpp
)

target_include_directories(cjlcc PUBLIC "${PROJECT_SOURCE_DIR}")
target_link_libraries(cjlcc libslvmutils m)