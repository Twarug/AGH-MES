cmake_minimum_required(VERSION 3.29)
project(MES)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/**.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/**.h")

add_executable(MES ${SOURCES} ${HEADERS})
target_include_directories(MES PRIVATE src)
