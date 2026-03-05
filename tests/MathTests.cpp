#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <gtest/gtest.h>

// A simple test to verify GLM math works as expected
TEST(MathTests, VectorAddition) {
  glm::vec2 a(1.0f, 2.0f);
  glm::vec2 b(3.0f, 4.0f);
  glm::vec2 result = a + b;

  EXPECT_FLOAT_EQ(result.x, 4.0f);
  EXPECT_FLOAT_EQ(result.y, 6.0f);
}

// A simple test to verify the ECS registry (EnTT) initializes
TEST(ECSTests, EntityCreation) {
  entt::registry registry;
  auto entity = registry.create();

  EXPECT_TRUE(registry.valid(entity));
}
