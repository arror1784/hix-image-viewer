cmake_minimum_required(VERSION 3.0.0)
project(hix-image-viewer VERSION 0.1.0)

add_executable(hix-image-viewer main.cpp base64.cpp communicate.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 gtk+-3.0 REQUIRED)

target_include_directories(hix-image-viewer

PUBLIC
    /usr/lib/x86_64-linux-gnu/glib-2.0/include/
    /usr/lib/x86_64-linux-gnu/arm-linux-gnueabihf/
)

message("${GTK3_INCLUDE_DIRS}")


target_compile_options(hix-image-viewer
PUBLIC
    -Wall
    -Wextra
    -std=c++17
    -no-pie)

target_link_libraries(hix-image-viewer
PUBLIC
    ${GTK3_LIBRARIES}
    -lrt
    -lm
    -lpthread
    -lstdc++fs
)
target_include_directories(hix-image-viewer
PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${GTK3_INCLUDE_DIRS}
)

message(STATUS "Compiler")
message(STATUS " - ID       \t: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS " - Version  \t: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - Path     \t: ${CMAKE_CXX_COMPILER}")

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
