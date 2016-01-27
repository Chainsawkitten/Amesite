cmake_minimum_required(VERSION 2.8.12)

project(Modership)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests" FORCE)
add_subdirectory(externals/glfw)
include_directories(externals/glfw/include)

# GLEW
add_subdirectory(externals/glew/build/cmake)
include_directories(externals/glew/include)

# SimpleIni
include_directories(externals/simpleini)

# GLM
include_directories(externals/glm)

# STB
include_directories(externals/stb)

# OpenAL Soft
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Don't build OpenAL Soft examples" FORCE)
add_subdirectory(externals/openal-soft)
include_directories(externals/openal-soft/include)
if(WIN32)
    set(OPENAL_LIB OpenAL32)
else()
    set(OPENAL_LIB openal)
endif()

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

# Setup embedded source
set(EMBEDDED_INPUT
        shaders/Default3D.frag
        shaders/Default3D.vert
    )

# Headerize
add_subdirectory(externals/Headerize)
macro(ProcessWithHeaderize headervar sourcevar inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
    add_custom_command(
        OUTPUT ${outputfile}.hpp ${outputfile}.cpp
        COMMAND Headerize ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${inputfile}
        COMMENT "Generating headers for ${inputfile}"
    )
    list(APPEND ${headervar} ${outputfile}.hpp)
    list(APPEND ${sourcevar} ${outputfile}.cpp)
endmacro()

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

include_directories(src)

# Doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(EngineDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/EngineDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_target(GameDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/GameDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

## TARGETS ##

## Engine ##
set(ENGINE_SRCS
        src/Engine/MainWindow.cpp
        src/Engine/Resources.cpp
        src/Engine/Geometry/Cube.cpp
        src/Engine/Geometry/Geometry3D.cpp
        src/Engine/Shader/Shader.cpp
        src/Engine/Shader/ShaderProgram.cpp
        src/Engine/Util/FileSystem.cpp
        src/Engine/Util/Log.cpp
        src/Engine/Util/Settings.cpp
		src/Engine/Util/Input.cpp
        src/Engine/Component/Lens.cpp
        src/Engine/Component/Transform.cpp
        src/Engine/Component/Mesh.cpp
        src/Engine/Entity/Entity.cpp
    )

set(ENGINE_HEADERS
        src/Engine/MainWindow.hpp
        src/Engine/Resources.hpp
        src/Engine/Geometry/Cube.hpp
        src/Engine/Geometry/Geometry3D.hpp
        src/Engine/Shader/Shader.hpp
        src/Engine/Shader/ShaderProgram.hpp
        src/Engine/Util/FileSystem.hpp
        src/Engine/Util/Log.hpp
        src/Engine/Util/Settings.hpp
		src/Engine/Util/Input.hpp
        src/Engine/Component/Lens.hpp
        src/Engine/Component/Transform.hpp
        src/Engine/Component/Mesh.hpp
        src/Engine/Entity/Entity.hpp
    )

create_directory_groups(${ENGINE_SRCS} ${ENGINE_HEADERS})

add_library(Engine STATIC ${ENGINE_SRCS} ${ENGINE_HEADERS} ${EMBEDDED_SOURCE})
set_property(TARGET Engine PROPERTY CXX_STANDARD 11)
set_property(TARGET Engine PROPERTY CXX_STANDARD_REQUIRED ON)

## Game ##
set(GAME_SRCS
        src/Game/main.cpp
        src/Game/Util/GameSettings.cpp
    )

set(GAME_HEADERS
        src/Game/Util/GameSettings.hpp
    )

create_directory_groups(${GAME_SRCS} ${GAME_HEADERS})

add_executable(Modership ${GAME_SRCS} ${GAME_HEADERS})
target_link_libraries(Modership Engine)
target_include_directories(Modership PUBLIC src/Engine)
target_link_libraries(Modership glew glfw ${GLFW_LIBRARIES})
set_property(TARGET Modership PROPERTY CXX_STANDARD 11)
set_property(TARGET Modership PROPERTY CXX_STANDARD_REQUIRED ON)
