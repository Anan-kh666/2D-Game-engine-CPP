cmake_minimum_required(VERSION 3.15)
project(2DEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include vendor/glad/include)

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

add_executable(2DEngine src/main.cpp src/Application.cpp src/Input.cpp src/Shader.cpp src/Buffer.cpp vendor/glad/src/glad.c)

target_link_libraries(2DEngine PRIVATE glfw)
if (APPLE)
    target_link_libraries(2DEngine PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()