
file(GLOB CORE_SRC "*.c")
file(GLOB CORE_SRC_H "*.h")

add_library(core SHARED ${CORE_SRC} ${CORE_SRC_H})
target_include_directories(core PUBLIC .)

target_link_libraries(core raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(core "-framework IOKit")
    target_link_libraries(core "-framework Cocoa")
    target_link_libraries(core "-framework OpenGL")
endif()
