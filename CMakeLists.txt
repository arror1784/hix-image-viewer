cmake_minimum_required(VERSION 3.0.0)
project(SimpleImageViewerFromIPC VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(SimpleImageViewerFromIPC main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 gtk+-3.0 REQUIRED)

target_compile_options(SimpleImageViewerFromIPC PUBLIC
            -Wall
            -Wextra
            -mtune=native
            -no-pie
            -lm)
target_include_directories(SimpleImageViewerFromIPC PUBLIC
            ${workspaceFolder}/**
            /usr/lib/x86_64-linux-gnu/glib-2.0/include/
            /usr/lib/x86_64-linux-gnu/
            /usr/include/gtk-3.0
            /usr/include/pango-1.0
            /usr/include/glib-2.0
            /usr/lib/libffi-3.2.1
            /usr/include/harfbuzz
            /usr/include/fribidi
            /usr/include/freetype
            /usr/include/libpng16
            /usr/include/cairo
            /usr/include/pixman-1
            /usr/include/gdk-pixbuf-2.0
            /usr/include/libmount
            /usr/include/blkid
            /usr/include/gio-unix-2.0
            /usr/include/atk-1.0
            /usr/include/at-spi2-atk/2.0
            /usr/include/at-spi-2.0
            /usr/include/dbus-1.0
            /usr/lib/dbus-1.0/include
            /usr/lib/gcc/x86_64-pc-linux-gnu/9.2.0/include
            /usr/local/include
            /usr/lib/gcc/x86_64-pc-linux-gnu/9.2.0/include-fixed
            /usr/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
