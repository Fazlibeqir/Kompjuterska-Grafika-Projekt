cmake_minimum_required(VERSION 3.0)
project(Kompjuterska-Grafika-Projekt)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Glitter/Vendor/bullet)


if(MSVC)
    add_compile_options(/MP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /INCREMENTAL")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Glitter/Headers/
        Glitter/Vendor/assimp/include/
        Glitter/Vendor/bullet/src/
        Glitter/Vendor/glad/include/
        Glitter/Vendor/glfw/include/
        Glitter/Vendor/glm/
        Glitter/Vendor/stb/
        Glitter/Vendor/imgui/
        Glitter/Vendor/imgui/backends/
        Glitter/Vendor/irrKlang/includes

        )

file(GLOB VENDORS_SOURCES Glitter/Vendor/glad/src/glad.c
        Glitter/Vendor/imgui/backends/imgui_impl_glfw.cpp
        Glitter/Vendor/imgui/backends/imgui_impl_opengl3.cpp
        Glitter/Vendor/imgui/imgui*.cpp
        )
file(GLOB PROJECT_HEADERS Glitter/Headers/*.hpp
        Glitter/Headers/*.h)
file(GLOB PROJECT_SOURCES Glitter/Sources/*.cpp)
file(GLOB PROJECT_SHADERS ${SHADER_DIR}/*.frag
        ${SHADER_DIR}/*.vert)
file(GLOB PROJECT_AUDIO Glitter/Sources/assets/media/*.ogg)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
        Readme.md
        .gitattributes
        .gitignore
        .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("audio" FILES ${PROJECT_AUDIO})

add_library(stb_image ${CMAKE_SOURCE_DIR}/Glitter/Vendor/stb/deprecated/stb_image.c)
add_definitions(-DGLFW_INCLUDE_NONE
        -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
        -DASSIMP_WARNINGS_AS_ERRORS=OFF)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
        ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
        ${VENDORS_SOURCES} ${PROJECT_AUDIO}

        Glitter/Headers/shader.hpp
        Glitter/Headers/mesh.hpp
        Glitter/Vendor/stb/deprecated/rrsprintf.h
        Glitter/Vendor/stb/deprecated/stretchy_buffer.h
        Glitter/Vendor/imgui/imgui.h
        Glitter/Headers/camera.hpp
        Glitter/Sources/init.cpp
        Glitter/Headers/init.h
        #Glitter/Sources/MainMenu.cpp
        #Glitter/Headers/MainMenu.h
        #Glitter/Sources/Game.cpp
        #Glitter/Headers/Game.h
        #Glitter/Headers/Skybox.hpp
        #Glitter/Sources/Skybox.cpp
        Glitter/Headers/Physics.h
        #Glitter/Headers/FrameBuffer.hpp
        #Glitter/Sources/FrameBuffer.cpp
        )
target_link_libraries(
        ${PROJECT_NAME}
        assimp
        glfw
        stb_image
        ${GLFW_LIBRARIES}
        ${GLAD_LIBRARIES}
        BulletDynamics
        BulletCollision
        LinearMath
        ${CMAKE_CURRENT_SOURCE_DIR}/Glitter/Vendor/irrKlang/libs/irrKlang.lib
)
set(DLL_LIST
        "Glitter/irrKlang.dll"
        "Glitter/ikpFlac.dll"
        "Glitter/ikpMP3.dll"
        )

# Copy DLLs to the build directory
foreach(DLL ${DLL_LIST})
    add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}
            )
endforeach()
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
#Specify relative paths for shader and model files
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Glitter/Shaders")
set(MODEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Glitter/Sources/assets/model")
set(SKY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Glitter/Sources/assets/img")
set(AUDIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Glitter/Sources/assets/media")
#Add preprocessor definitions for the relative paths
target_compile_definitions(${PROJECT_NAME} PRIVATE
        SHADER_DIR="${SHADER_DIR}"
        MODEL_DIR="${MODEL_DIR}"
        SKY_DIR="${SKY_DIR}"
        AUDIO_DIR="${AUDIO_DIR}"
        )
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Sources/assets/model $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Sources/assets/img $<TARGET_FILE_DIR:${PROJECT_NAME}>
        DEPENDS ${PROJECT_SHADERS})
if(WIN32)
    add_custom_target(build COMMAND make /m8)
endif()
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Shaders ${CMAKE_BINARY_DIR}/Glitter/Shaders
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Models ${CMAKE_BINARY_DIR}/Glitter/Models
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Skybox ${CMAKE_BINARY_DIR}/Glitter/Skybox
#        DEPENDS ${PROJECT_SHADERS})