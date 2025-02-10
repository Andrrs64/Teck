cmake_minimum_required(VERSION 3.15)
project(Teck)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(raylib 3.0 REQUIRED)

include(cmake/CPM.cmake)

# Fetch ImGui
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG cb16be3a3fc1f9cd146ae24d52b615f8a05fa93d) # v1.90.9

FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

add_library(
    imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})

# rlImgui
FetchContent_Declare(
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui
    GIT_TAG d765c1ef3d37cf939f88aaa272a59a2713d654c9)
FetchContent_MakeAvailable(rlImGui)
FetchContent_GetProperties(rlImGui SOURCE_DIR RLIMGUI_DIR)

add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImgui.cpp)
target_link_libraries(rlimgui PRIVATE imgui raylib)
target_include_directories(rlimgui INTERFACE ${rlimgui_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(editor)

