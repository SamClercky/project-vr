cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(OpenGL_Template)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW
option(GLFW_INSTALL OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdparty/glfw)
if (MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif ()
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT opengl_test2)

# GLAD
add_library(glad STATIC 3rdparty/glad/src/glad.c)
set(GLAD_INCLUDE "3rdparty/glad/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

# Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_INSTALL OFF)
add_subdirectory(3rdparty/assimp)

# Bullet3
option(BUILD_CPU_DEMOS OFF)
option(BUILD_ENET OFF)
option(BUILD_CLSOCKET OFF)
OPTION(BUILD_EXTRAS OFF)
OPTION(BUILD_UNIT_TESTS OFF)
option(BUILD_BULLET2_DEMOS OFF)
add_subdirectory(3rdparty/bullet3)

# EnTT
add_subdirectory(3rdparty/entt)

# include dependencies
include_directories(
        3rdparty/glad/include
        3rdparty/glfw/include
        3rdparty/glm/
        3rdparty/stb
        3rdparty/assimp/include
        3rdparty/entt/
        ${BULLET_PHYSICS_SOURCE_DIR}/src
)

# Project files
set(PROJECT-SRC
        main.cpp
        )

# Project building
add_definitions(-DGLFW_INCLUDE_NONE)
add_executable(${PROJECT_NAME} WIN32 ${PROJECT-SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${OPENGL_LIBRARIES} glfw glad assimp EnTT
        BulletDynamics BulletCollision LinearMath)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add compiler warnings
if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

# shader files and resources
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources to binary folder")
add_dependencies(${PROJECT_NAME} copy_resources)
