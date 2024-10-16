cmake_minimum_required(VERSION 3.29)
project(MES)

include(FetchContent)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)


set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/**.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/**.h")

add_executable(MES ${SOURCES} ${HEADERS})
target_include_directories(MES PRIVATE src)

target_link_libraries(MES PUBLIC yaml-cpp::yaml-cpp)
