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

bool SceneSerializer::Deserialize(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filepath << std::endl;
        return false;
    }
    json sceneData;
    file >> sceneData;
    if (!sceneData.contains("Entities")) return false;
    for (auto& entityData : sceneData["Entities"]) {
        entt::entity entity = m_Scene->CreateEntity();
        if (entityData.contains("TransformComponent")) {
            auto& tc = m_Scene->GetRegistry().emplace<TransformComponent>(entity);
            tc.Position = {
                entityData["TransformComponent"]["Position"][0],
                entityData["TransformComponent"]["Position"][1],
                entityData["TransformComponent"]["Position"][2]
            };
            tc.Size = {
                entityData["TransformComponent"]["Size"][0],
                entityData["TransformComponent"]["Size"][1]
            };
        }
        if (entityData.contains("SpriteRendererComponent")) {
            auto& src = m_Scene->GetRegistry().emplace<SpriteRendererComponent>(entity);
            src.Color = {
                entityData["SpriteRendererComponent"]["Color"][0],
                entityData["SpriteRendererComponent"]["Color"][1],
                entityData["SpriteRendererComponent"]["Color"][2],
                entityData["SpriteRendererComponent"]["Color"][3]
            };
        }
        if (entityData.contains("Rigidbody2DComponent")) {
            auto& rb2d = m_Scene->GetRegistry().emplace<Rigidbody2DComponent>(entity);
            rb2d.Type = (Rigidbody2DType)entityData["Rigidbody2DComponent"]["Type"];
            rb2d.FixedRotation = entityData["Rigidbody2DComponent"]["FixedRotation"];
        }

        if (entityData.contains("BoxCollider2DComponent")) {
            auto& bc2d = m_Scene->GetRegistry().emplace<BoxCollider2DComponent>(entity);
            bc2d.Size = {
                entityData["BoxCollider2DComponent"]["Size"][0],
                entityData["BoxCollider2DComponent"]["Size"][1]
            };
            bc2d.Density = entityData["BoxCollider2DComponent"]["Density"];
            bc2d.Friction = entityData["BoxCollider2DComponent"]["Friction"];
            bc2d.Restitution = entityData["BoxCollider2DComponent"]["Restitution"];
        }
    }
    std::cout << "Successfully loaded scene from " << filepath << "!" << std::endl;
    return true;
}