cmake_minimum_required(VERSION 3.26)

# Allow cuda and cpp languages
project(opengl-3d-scenery LANGUAGES CXX)

# Set cpp standard
set(CMAKE_CXX_STANDARD 17)

# Find patterns with these patterns and add them to sources
file(GLOB SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
    "src/app/*.cpp"
    "src/app/*.hpp"
    "src/app/*.h"
    "src/vendor/stbi_image.h"
)

file(GLOB_RECURSE RENDERER_SOURCES
    "src/renderer/*.cpp"
    "src/renderer/*.hpp"
    "src/renderer/*.h"
)

list(APPEND SOURCES ${RENDERER_SOURCES})

add_subdirectory(src/vendor)

# Set include and lib directories
include_directories(dependencies/include)

# Add lib directory
if(WIN32)
    set(LIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/lib/win)
    file(GLOB EXTERNAL_LIBS "${LIB_DIR}/*.lib")
elseif(UNIX)
    set(LIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/lib/linux)
    file(GLOB EXTERNAL_LIBS "${LIB_DIR}/*.so")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Create an executable from the source files
add_executable(opengl-3d-scenery ${SOURCES})

# Link the executable to the external libraries
target_link_libraries(opengl-3d-scenery
    ImGui
    ${EXTERNAL_LIBS}
)
#
#if (WIN32)
#    if ($ENV{CLION_IDE})
#        add_custom_command(
#                TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_CURRENT_BINARY_DIR}/../dependencies/bin/glfw3.dll
#                ${CMAKE_CURRENT_BINARY_DIR}/../dependencies/bin/glew32.dll
#                ${CMAKE_CURRENT_BINARY_DIR}
#        )
#        add_custom_command(
#                TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${CMAKE_CURRENT_BINARY_DIR}/res
#                ${CMAKE_CURRENT_BINARY_DIR}/res
#        )
#    else()
#        add_custom_command(
#                TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_CURRENT_BINARY_DIR}/dependencies/bin/glfw3.dll
#                ${CMAKE_CURRENT_BINARY_DIR}/dependencies/bin/glew32.dll
#                ${CMAKE_CURRENT_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>
#        )
#        add_custom_command(
#                TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${CMAKE_CURRENT_BINARY_DIR}/res
#                ${CMAKE_CURRENT_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/res
#        )
#    endif()
#endif()