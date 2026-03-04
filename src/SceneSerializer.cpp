#include "SceneSerializer.h"
#include "Components.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

SceneSerializer::SceneSerializer(Scene* scene) : m_Scene(scene) {}

void SceneSerializer::Serialize(const std::string& filepath) {
    json sceneData;
    sceneData["Entities"] = json::array();

    m_Scene->GetRegistry().each([&](auto entityID) {
        json entityData;
        entityData["EntityID"] = (uint32_t)entityID;

        if (m_Scene->GetRegistry().all_of<TransformComponent>(entityID)) {
            auto& tc = m_Scene->GetRegistry().get<TransformComponent>(entityID);
            entityData["TransformComponent"] = {
                {"Position", {tc.Position.x, tc.Position.y, tc.Position.z}},
                {"Size", {tc.Size.x, tc.Size.y}}
            };
        }

        if (m_Scene->GetRegistry().all_of<SpriteRendererComponent>(entityID)) {
            auto& src = m_Scene->GetRegistry().get<SpriteRendererComponent>(entityID);
            entityData["SpriteRendererComponent"] = {
                {"Color", {src.Color.r, src.Color.g, src.Color.b, src.Color.a}}
            };
        }

        if (m_Scene->GetRegistry().all_of<Rigidbody2DComponent>(entityID)) {
            auto& rb2d = m_Scene->GetRegistry().get<Rigidbody2DComponent>(entityID);
            entityData["Rigidbody2DComponent"] = {
                {"Type", (int)rb2d.Type},
                {"FixedRotation", rb2d.FixedRotation}
            };
        }

        if (m_Scene->GetRegistry().all_of<BoxCollider2DComponent>(entityID)) {
            auto& bc2d = m_Scene->GetRegistry().get<BoxCollider2DComponent>(entityID);
            entityData["BoxCollider2DComponent"] = {
                {"Size", {bc2d.Size.x, bc2d.Size.y}},
                {"Density", bc2d.Density},
                {"Friction", bc2d.Friction},
                {"Restitution", bc2d.Restitution}
            };
        }

        sceneData["Entities"].push_back(entityData);
    });

    std::ofstream file(filepath);
    file << std::setw(4) << sceneData << std::endl;
    std::cout << "Successfully saved scene to " << filepath << "!" << std::endl;
}