cmake_minimum_required(VERSION 3.15)
project(2DEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.12.2
)
FetchContent_MakeAvailable(entt)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v2.4.1
)
set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)

FetchContent_Declare(
    json 
    GIT_REPOSITORY https://github.com/nlohmann/json.git 
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

include_directories(
    include
    vendor/glad/include
    vendor/stb_image
    ${glm_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${entt_SOURCE_DIR}/src
    ${box2d_SOURCE_DIR}/include
    ${json_SOURCE_DIR}/include
)

add_executable(2DEngine
    src/main.cpp
    src/Application.cpp
    src/Input.cpp
    src/Shader.cpp
    src/Buffer.cpp
    src/VertexArray.cpp
    src/Texture2D.cpp
    src/ImGuiLayer.cpp
    src/OrthographicCamera.cpp
    src/Renderer2D.cpp
    src/Scene.cpp
    src/SceneSerializer.cpp
    vendor/glad/src/glad.c
    vendor/stb_image/stb_image.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(2DEngine PRIVATE glfw box2d)
if (APPLE)
    target_link_libraries(2DEngine PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()