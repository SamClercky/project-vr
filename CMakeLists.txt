cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(Project-VR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (MSVC) # add RTTI data, needed for dynamic cast
    add_compile_options(/GR)
endif ()

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
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif ()
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT opengl_test2)

# GLAD
add_library(glad STATIC 3rdparty/glad/src/glad.c)
set(GLAD_INCLUDE "3rdparty/glad/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

# Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
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
        src/stl/std_image.cpp

        src/components/Camera.h
        src/components/CollisionObject.h src/components/CollisionObject.cpp
        src/components/DeltaTime.h
        src/components/FollowedByCamera.h
        src/components/GameStateGlobals.h
        src/components/Light.h
        src/components/Material.h
        src/components/Position.h
        src/components/Renderable.h
        src/components/RotateAnimation.h
        src/components/ShortLivedObject.h

        src/engine/AssetManager.h src/engine/AssetManager.cpp
        src/engine/Mesh.h src/engine/Mesh.cpp
        src/engine/Model.h
        src/engine/Renderer.h src/engine/Renderer.cpp
        src/engine/Shader.h src/engine/Shader.cpp
        src/engine/Texture2D.h src/engine/Texture2D.cpp
        src/engine/Window.h src/engine/Window.cpp

        src/prefabs/CameraPrefab.h src/prefabs/CameraPrefab.cpp
        src/prefabs/CubeMapPrefab.h src/prefabs/CubeMapPrefab.cpp
        src/prefabs/PaintingPrefab.h src/prefabs/PaintingPrefab.cpp
        src/prefabs/PlayerPrefab.h src/prefabs/PlayerPrefab.cpp
        src/prefabs/PointLightPrefab.h src/prefabs/PointLightPrefab.cpp
        src/prefabs/RabbitPrefab.h src/prefabs/RabbitPrefab.cpp
        src/prefabs/SmokePrefab.h src/prefabs/SmokePrefab.cpp
        src/prefabs/SpotLightPrefab.h src/prefabs/SpotLightPrefab.cpp
        src/prefabs/TablePrefab.h src/prefabs/TablePrefab.cpp

        src/scenes/Scene.h src/scenes/Scene.cpp
        
        src/systems/CameraSystem.h src/systems/CameraSystem.cpp
        src/systems/bulletDebugDraw.h src/systems/bulletDebugDraw.cpp
        src/systems/bulletSystem.h src/systems/bulletSystem.cpp
        src/systems/DestroyFallOutOfWorldSystem.h src/systems/DestroyFallOutOfWorldSystem.cpp
        src/systems/DrawSystem.h src/systems/DrawSystem.cpp
        src/prefabs/EnvironmentPrefab.h src/prefabs/EnvironmentPrefab.cpp
        src/systems/InputUpdaterSystem.h src/systems/InputUpdaterSystem.cpp
        src/systems/LightSystem.h src/systems/LightSystem.cpp
        src/systems/RotateSystem.h src/systems/RotateSystem.cpp
        src/prefabs/ThrowableBulletPrefab.h src/prefabs/ThrowableBulletPrefab.cpp
        src/systems/ThrowableBulletShooterSystem.h src/systems/ThrowableBulletShooterSystem.cpp
        src/systems/ViewportUpdateSystem.h src/systems/ViewportUpdateSystem.cpp
        )

# Project building
add_definitions(-DGLFW_INCLUDE_NONE)
#add_definitions(-DVR_USE_ARROW_KEYS_AS_MOUSE) # Replace mouse movements with arrow keys
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
