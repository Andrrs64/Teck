cmake_minimum_required(VERSION 3.15)
project(Teck)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(raylib 3.0 REQUIRED)

set(imgui_SOURCE_DIR external/imgui)

add_library(
    imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})

set(rlimgui_SOURCE_DIR external/rlImGui)

add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImgui.cpp)
target_link_libraries(rlimgui PRIVATE imgui raylib)
target_include_directories(rlimgui INTERFACE ${rlimgui_SOURCE_DIR})

add_library(tinyFileDialogs STATIC external/tinyfiledialogs/tinyfiledialogs.c)
target_include_directories(tinyFileDialogs INTERFACE external/tinyfiledialogs)

add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(editor)

