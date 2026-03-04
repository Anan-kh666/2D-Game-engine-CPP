#pragma once
#include <entt/entt.hpp>
#include "OrthographicCamera.h"

class Scene {
public:
    Scene();
    ~Scene();

    entt::entity CreateEntity();
    void OnUpdate(const OrthographicCamera& camera);
    entt::registry& GetRegistry() { return m_Registry; }

private:
    entt::registry m_Registry;
};