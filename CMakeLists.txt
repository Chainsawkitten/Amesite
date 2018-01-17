cmake_minimum_required(VERSION 2.8.12)

project(Amesite)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -gdwarf-2 -O3 -static-libstdc++ -Wall")
endif()

# Externals
add_subdirectory(externals)

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

# Copy resources
add_custom_target(ResourceTarget ALL)
add_custom_command(
    TARGET ResourceTarget
    COMMAND ${CMAKE_COMMAND} -E copy_directory assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying assets."
)

foreach(item ${RESOURCES})
    CopyAsset(${item} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources)
endforeach()

# Doxygen documentation
option(AMESITE_BUILD_DOCS "Build the doxygen documentation" OFF)
find_package(Doxygen)
if(DOXYGEN_FOUND AND AMESITE_BUILD_DOCS)
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
endif()

# Headerize
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

## TARGETS ##
add_subdirectory(src)
