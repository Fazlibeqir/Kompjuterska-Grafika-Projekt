cmake_minimum_required(VERSION 3.0)
project(AudioPro)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

# Include directories
include_directories(externals/includes)

# Collect list of source files
file(GLOB PROJECT_AUDIO media/*.ogg)

# Group source files in the IDE (optional)
source_group("audio" FILES ${PROJECT_AUDIO})

# Define a macro for the project source directory
add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

# Create executable target from main.cpp and audio sources
add_executable(${PROJECT_NAME} main.cpp ${PROJECT_AUDIO})

# Link irrKlang library
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/externals/libs/irrKlang.lib)

# List of DLLs to copy
set(DLL_LIST
        "irrKlang.dll"
        "ikpFlac.dll"
        "ikpMP3.dll"
        )

# Copy DLLs to the build directory
foreach(DLL ${DLL_LIST})
    add_custom_command(TARGET AudioPro POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${DLL}"
            $<TARGET_FILE_DIR:AudioPro>
            )
endforeach()

# Set the output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
        )
