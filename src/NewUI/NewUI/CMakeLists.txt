# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(project_name NewUI)
project(${project_name})

# Include sub-projects.
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

FILE(GLOB SRCS "*.cpp" "*.c" "*.def" "*.rc")
add_executable(${project_name} ${SRCS})

list(APPEND project_name_cflags /W4)
list(APPEND project_name_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=_WIN32_WINNT_VISTA)
MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})

set(CompilerFlags
	CMAKE_CXX_FLAGS
	CMAKE_CXX_FLAGS_DEBUG
	CMAKE_CXX_FLAGS_RELEASE
	CMAKE_C_FLAGS
	CMAKE_C_FLAGS_DEBUG
	CMAKE_C_FLAGS_RELEASE
	)
foreach(CompilerFlag ${CompilerFlags})
	string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()
	
target_compile_definitions(${project_name} PRIVATE ${project_name_defines})
target_compile_options(${project_name} PRIVATE ${project_name_cflags})

set(MSVC_LIBS wininet)
	
target_link_libraries(${project_name} 
PRIVATE ${MSVC_LIBS}
)