# GLEW
set(BUILD_UTILS OFF CACHE BOOL "Don't build GLEW utility programs" FORCE)
add_subdirectory(glew/build/cmake)
target_include_directories(glew INTERFACE glew/include)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Don't build GLFW docs" FORCE)
add_subdirectory(glfw)
#include_directories(glfw/include)

# Headerize
add_subdirectory(Headerize)

# OpenAL Soft
set(ALSOFT_UTILS OFF CACHE BOOL "Don't build OpenAL Soft utility programs" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Don't build OpenAL Soft examples" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "Don't build OpenAL Soft tests" FORCE)
add_subdirectory(openal-soft)
add_library(OpenALSoft INTERFACE)
if(WIN32)
    target_link_libraries(OpenALSoft INTERFACE OpenAL32)
else()
    target_link_libraries(OpenALSoft INTERFACE openal)
endif()
target_include_directories(OpenALSoft INTERFACE openal-soft/include)

# GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

# SimpleIni
add_library(simpleini INTERFACE)
target_include_directories(simpleini INTERFACE simpleini)

# STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)
