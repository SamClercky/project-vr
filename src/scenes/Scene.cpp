#include "Scene.h"
#include "components/DeltaTime.h"
#include "prefabs/CameraPrefab.h"
#include "prefabs/CubePrefab.h"
#include "systems/CameraSystem.h"
#include "systems/DrawSystem.h"
#include "systems/RotateSystem.h"

using namespace scenes;

Scene::Scene(engine::Renderer &renderer) : m_registry(entt::registry{}) {
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  m_registry.ctx().emplace<components::DeltaTime>(0);
  auto cubeObj = prefabs::cubePrefabLoader(renderer);
  for (auto position : cubePositions) {
    prefabs::cubePrefab(cubeObj, m_registry, position);
  }

  prefabs::cameraPrefab(m_registry);
}

void Scene::update(uint64_t deltaTime) {
  m_registry.ctx().erase<components::DeltaTime>();
  m_registry.ctx().emplace<components::DeltaTime>(deltaTime);
  systems::rotateSystem(m_registry);
}

void Scene::render(engine::Renderer::RenderGuard &renderer) {
  systems::cameraSystem(renderer, m_registry);
  systems::drawSystem(renderer, m_registry);
}
