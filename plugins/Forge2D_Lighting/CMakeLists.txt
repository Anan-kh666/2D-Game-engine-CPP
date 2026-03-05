cmake_minimum_required(VERSION 3.15)
project(Forge2D_Lighting)

# Premium Plugin Code
add_library(Forge2D_Lighting STATIC
    src/LightingExtension.cpp
)

target_include_directories(Forge2D_Lighting PUBLIC include)
target_link_libraries(Forge2D_Lighting PRIVATE EngineCore)
