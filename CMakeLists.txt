cmake_minimum_required(VERSION 3.20)

project(yucel-y-n-25-04)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}bin.relwithdbg)
set(CMAKE_GENERATOR_TOOLSET "v143" CACHE STRING "MSVC toolset version")

include_directories(
    prj.labs
    prj.thirdparty
)

add_subdirectory(prj.codeforces)
add_subdirectory(prj.labs)
add_subdirectory(prj.test)
