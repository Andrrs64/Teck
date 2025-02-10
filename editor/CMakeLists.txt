add_executable(Editor
    main.cpp
    external/tinyfiledialogs.c
    external/tinyfiledialogs.h
)

target_link_libraries(Editor core)

target_link_libraries(Editor raylib)
target_link_libraries(Editor imgui)
target_link_libraries(Editor rlimgui)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(Editor "-framework IOKit")
    target_link_libraries(Editor "-framework Cocoa")
    target_link_libraries(Editor "-framework OpenGL")
endif()
