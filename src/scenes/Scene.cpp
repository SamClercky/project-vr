#include "Scene.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "components/DeltaTime.h"
#include "prefabs/CameraPrefab.h"
#include "prefabs/CubeMapPrefab.h"
#include "prefabs/CubePrefab.h"
#include "prefabs/DirLightPrefab.h"
#include "prefabs/EnvironmentPrefab.h"
#include "prefabs/LightCubePrefab.h"
#include "prefabs/RabbitPrefab.h"
#include "prefabs/SmokePrefab.h"
#include "systems/CameraSystem.h"
#include "systems/DrawSystem.h"
#include "systems/InputUpdaterSystem.h"
#include "systems/LightSystem.h"
#include "systems/RotateSystem.h"
#include "systems/ViewportUpdateSystem.h"
#include "systems/bulletSystem.h"
#include "systems/bulletDebugDraw.h"
#include "systems/ThrowableBulletShooterSystem.h"
#include "systems/DestroyFallOutOfWorldSystem.h"
#include "prefabs/PlayerPrefab.h"
#include "components/GameStateGlobals.h"

using namespace scenes;

std::unique_ptr<btDiscreteDynamicsWorld> setup_physics() {
    auto *broadphase = new btDbvtBroadphase();
    auto *collisionConfiguration = new btDefaultCollisionConfiguration();
    auto *dispatcher = new btCollisionDispatcher(collisionConfiguration);
    auto *solver = new btSequentialImpulseConstraintSolver();

    std::unique_ptr world = std::make_unique<btDiscreteDynamicsWorld>(dispatcher, broadphase, solver, collisionConfiguration);
    world->setGravity(btVector3{0.f, -9.81f, 0.f});

    return world;
}

Scene::Scene(engine::Window &window, engine::Renderer &renderer) : m_registry(entt::registry{}), m_window_ref(window) {
    m_dynamics_world = setup_physics();

    glm::vec3 cubePositions[] = {
            /*glm::vec3(0.0f, 0.0f, 0.0f),*/ glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f), glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f), glm::vec3(-1.3f, 1.0f, -1.5f)};

    m_registry.ctx().emplace<components::DeltaTime>(0);
    m_registry.ctx().emplace<components::GameStateGlobals>();

    std::shared_ptr<engine::Model> envModel;
    std::shared_ptr<engine::Shader> envShader;
    prefabs::environmentPrefabLoader(envShader, envModel);
    prefabs::environmentPrefab(m_registry, envShader, envModel, m_dynamics_world);

    std::shared_ptr<engine::Model> cubeModel;
    std::shared_ptr<engine::Shader> cubeShader;
//    prefabs::cubePrefabLoader(cubeModel, cubeShader);
    prefabs::lightCubePrefabLoader(cubeModel, cubeShader);
    for (auto position: cubePositions) {
//        prefabs::cubePrefab(cubeModel, cubeShader, m_registry, position);
        prefabs::lightCubePrefab(cubeModel, cubeShader, m_registry, position, m_dynamics_world);
    }

    //light test
    prefabs::dirLightPrefab(m_registry);

    std::shared_ptr<engine::Model> rabbitModel;
    std::shared_ptr<engine::Shader> rabbitShader;
    prefabs::rabbitPrefabLoader(rabbitModel, rabbitShader);
//    prefabs::rabbitPrefab(rabbitModel, rabbitShader, m_registry, glm::vec3{0.f});

    std::shared_ptr<engine::Model> smokeModel;
    std::shared_ptr<engine::Shader> smokeShader;
    prefabs::smokePrefabLoader(smokeModel, smokeShader);
    prefabs::smokePrefab(smokeModel, smokeShader, m_registry, glm::vec3{3.f, 3.f, 3.f});

    std::shared_ptr<engine::Model> cubeMapModel;
    std::shared_ptr<engine::Shader> cubeMapShader;
    prefabs::cubeMapPrefabLoader(cubeMapModel, cubeMapShader);
    prefabs::cubeMapPrefab(cubeMapModel, cubeMapShader, m_registry);

    prefabs::playerPrefab(m_registry, m_dynamics_world);
    prefabs::cameraPrefab(m_registry);
}

void Scene::update(uint64_t deltaTime, uint32_t width, uint32_t height) {
    m_registry.ctx().insert_or_assign(components::DeltaTime(deltaTime));
    auto &state = m_registry.ctx().get<components::GameStateGlobals>();
    state.viewWidth = width;
    state.viewHeight = height;

    systems::update_viewport_system(m_registry, m_window_ref);
    systems::throwableBulletSystem(m_registry, m_window_ref, m_dynamics_world);
    systems::bulletSystem(m_registry, m_dynamics_world);
    systems::inputUpdaterSystem(m_registry, m_window_ref);
    systems::rotateSystem(m_registry);

    systems::destroyFallenOutOfWorldSystem(m_registry);
}

void Scene::render(engine::Renderer::RenderGuard &renderer) {
    systems::cameraSystem(renderer, m_registry);
    systems::lightSystem(renderer, m_registry);
    systems::drawSystem(renderer, m_registry);

    if (engine::Window::is_debug_mode())
        systems::bulletDebugDrawSystem(m_registry, renderer, m_dynamics_world);
    //add light system call?
}
