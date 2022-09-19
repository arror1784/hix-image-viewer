cmake_minimum_required(VERSION 3.0.0)
project(SimpleImageViewerFromIPC VERSION 0.1.0)

add_executable(SimpleImageViewerFromIPC main.cpp base64.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 gtk+-3.0 REQUIRED)

set(GTK_INC "")
set(GTK_LIB "")

# FOR ARM CROSS COMPILE
if(${CMAKE_CXX_COMPILER} MATCHES arm)
    # set(MYSYSROOT /home/jsh/raspi17/sysroot)
    set(CMAKE_SYSROOT /home/jsh/workspace/gcc_for_raspberrypi/sysroot)

    target_include_directories(SimpleImageViewerFromIPC
    PUBLIC
        # /usr/lib/arm-linux-gnueabihf/glib-2.0/include/
        # /usr/lib/arm-linux-gnueabihf/
    )

    message(" IS ARM VERSION")

    foreach(i ${GTK3_LIBRARIES})
        list(APPEND GTK_LIB "${i}")
        message("${i}")
    endforeach()

    message(" include path")
    
    foreach(i ${GTK3_INCLUDE_DIRS})
        list(APPEND GTK_INC "${CMAKE_SYSROOT}${i}")
        message("${CMAKE_SYSROOT}${i}")
    endforeach()

    list(APPEND GTK_INC "${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf/glib-2.0/include")
else()

    foreach(i ${GTK3_INCLUDE_DIRS})
        list(APPEND GTK_INC "${i}")
    endforeach()
    foreach(i ${GTK3_LIBRARIES})
        list(APPEND GTK_LIB "${i}")
    endforeach()

    target_include_directories(SimpleImageViewerFromIPC

    PUBLIC
        /usr/lib/x86_64-linux-gnu/glib-2.0/include/
        /usr/lib/x86_64-linux-gnu/arm-linux-gnueabihf/
    )
    message(" IS NOT ARM VERSION")
endif()


target_compile_options(SimpleImageViewerFromIPC
PUBLIC
    -lrt
    -lm
    -Wall
    -Wextra
    -no-pie)

target_link_libraries(SimpleImageViewerFromIPC
PUBLIC
    ${GTK3_LIBRARIES}
    -lrt
    -lm
)
target_include_directories(SimpleImageViewerFromIPC
PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${GTK_INC}
)

message(STATUS "Compiler")
message(STATUS " - ID       \t: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS " - Version  \t: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - Path     \t: ${CMAKE_CXX_COMPILER}")

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
