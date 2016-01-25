cmake_minimum_required(VERSION 2.8.12)

project(Modership)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests" FORCE)
add_subdirectory(externals/glfw)
include_directories(externals/glfw/include)

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

include_directories(src)

## TARGETS ##

## Game ##
set(GAME_SRCS
        src/Game/main.cpp
    )

set(GAME_HEADERS
        
    )

create_directory_groups(${GAME_SRCS} ${GAME_HEADERS})

add_executable(Modership ${GAME_SRCS} ${GAME_HEADERS})
target_link_libraries(Modership glfw ${GLFW_LIBRARIES})
set_property(TARGET Modership PROPERTY CXX_STANDARD 11)
set_property(TARGET Modership PROPERTY CXX_STANDARD_REQUIRED ON)