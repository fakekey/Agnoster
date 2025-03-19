cmake_minimum_required(VERSION 3.22.1)

project(agnoster)

add_subdirectory(whale)

set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections -std=c18")
set(CXX_FLAGS "-std=c++20 -Wall")

set(C_FLAGS "${C_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS} ${CXX_FLAGS} -O0")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Killer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Surface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SDK.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends/imgui_impl_android.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/xdl/xdl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/xdl/xdl_iterate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/xdl/xdl_linker.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/xdl/xdl_lzma.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/xdl/xdl_util.c)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/whale/include
        ${CMAKE_CURRENT_SOURCE_DIR}/misc
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/json
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/dobby/include
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/xdl
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/dobby
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends
        ${ANDROID_NDK}/sources/android/native_app_glue)

set(DOBBY_LIB ${CMAKE_CURRENT_SOURCE_DIR}/misc/dobby/${CMAKE_ANDROID_ARCH_ABI}/libdobby.a)

target_link_libraries(${CMAKE_PROJECT_NAME}
        whale
        android
        EGL
        GLESv3
        log
        ${DOBBY_LIB})
