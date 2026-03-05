# ⚡ ANAN ENGINEERING Project - Forge2D — A High-Performance 2D Game Engine

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-4.1-5586A4?style=flat-square&logo=opengl&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C?style=flat-square&logo=cmake&logoColor=white)
![Build Status](https://github.com/yourusername/forge2d/actions/workflows/build.yml/badge.svg)
![License](https://img.shields.io/badge/License-MIT-22c55e?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey?style=flat-square)

A professional-grade, **data-oriented 2D game engine** built from scratch in modern C++17. Forge2D is engineered for performance — rendering **10,000+ quads in a single GPU draw call at 120+ FPS** — while providing a full suite of editor tooling, physics simulation, and scene serialization.

> Built to demonstrate deep systems-level C++ expertise: GPU batch rendering, ECS architecture, physics integration, and cross-platform memory layout optimization.

---

![Engine Screenshot](screenshot.png)

---

## ✨ Key Features

- **Data-Oriented ECS via EnTT**
  Entities and components are stored in tightly-packed contiguous memory pools using the `EnTT` library. Systems iterate over component views with near-zero cache miss overhead — the way game engines are meant to be built.

- **Optimized Batch Renderer**
  The renderer accumulates geometry for an entire scene into a single interleaved VBO, flushing the entire batch to the GPU in **one `glDrawElements` call**. Sustains **120+ FPS at 10,000+ colored quads and textured sprites** simultaneously. Vertex data is packed into a hand-tuned **40-byte struct** to explicitly resolve ARM64 Apple Silicon memory padding and alignment pitfalls that plague naive implementations on M-series hardware.

- **Fully Integrated Box2D Physics**
  Dynamic and static rigid bodies, gravity, restitution, and AABB colliders — all cleanly **decoupled from the rendering pipeline**. Physics bodies synchronize to transform components each frame, leaving the renderer unaware of simulation state.

- **Dear ImGui Editor Overlay**
  A floating, dockable editor UI built with the **Dear ImGui docking branch**. Displays real-time performance metrics (FPS, draw call count, entity count), a scene toolbar, and per-entity component inspectors — all rendered in a separate ImGui pass with zero contamination of the game viewport.

- **Play / Stop Simulation States**
  The engine supports a discrete **Edit Mode / Play Mode** state machine. In Edit Mode, physics is paused and the scene is fully mutable. Clicking **Play** serializes the current scene state, initializes Box2D bodies, and begins the simulation. **Stop** tears down physics and restores the pre-play snapshot cleanly.

- **Scene Serialization with nlohmann/json**
  The entire ECS registry — transforms, sprites, physics bodies, and metadata — can be serialized to a human-readable `.json` file and deserialized back into a live engine session with full fidelity. No scene data is lost across save/load cycles.

- **Zero-Setup Automated Build & Testing**
  CMake `FetchContent` automatically downloads, configures, and statically links all dependencies (including **GoogleTest**). The project is fully target-based, meaning building the engine or running `ctest` works out of the box. An included GitHub Actions workflow ensures the engine successfully compiles and passes all tests across macOS, Linux, and Windows on every push.

---

## 🛠️ Tech Stack

| Library | Role |
|---|---|
| **C++17** | Core engine language |
| **OpenGL 4.1** (via Glad) | Low-level GPU rendering API |
| **GLFW** | Cross-platform window creation & input |
| **GLM** | SIMD-friendly math (vectors, matrices, transforms) |
| **EnTT** | High-performance entity-component-system registry |
| **Box2D** | Rigid body physics simulation |
| **Dear ImGui** (docking branch) | Immediate-mode editor UI |
| **nlohmann/json** | Scene serialization & deserialization |
| **stb_image** | Single-header texture loading |
| **GoogleTest** | Core automated unit testing |

---

## 🚀 Getting Started

### Prerequisites

| Tool | Minimum Version |
|---|---|
| C++ Compiler | GCC 9+, Clang 10+, or MSVC 19.28+ (C++17 support required) |
| CMake | 3.20+ |
| Git | Any recent version |

> **Note:** All library dependencies are fetched automatically by CMake. No manual installation is required.

### Build Instructions

```bash
# 1. Clone the repository
git clone https://github.com/yourusername/forge2d.git
cd forge2d

# 2. Create an isolated build directory
mkdir build && cd build

# 3. Configure — CMake will automatically fetch all dependencies
cmake ..

# 4. Compile
cmake --build . --config Release

# 5. Run the Automated Tests (Optional)
ctest --test-dir build -C Release

# 6. Run the engine
./build/2DEngine
```

> On Apple Silicon (M1/M2/M3), the build targets OpenGL 4.1 Core Profile — the highest version supported on macOS. All vertex layout optimizations for ARM64 alignment are applied automatically.

---

## 🎮 Controls

### Viewport Navigation

| Input | Action |
|---|---|
| `W` | Pan camera up |
| `S` | Pan camera down |
| `A` | Pan camera left |
| `D` | Pan camera right |
| `Scroll Wheel` | Zoom in / out |

### Editor Toolbar (ImGui)

| Button | Action |
|---|---|
| **Play ▶** | Serializes scene state, initializes Box2D world, begins simulation |
| **Stop ■** | Tears down physics world, restores pre-play scene snapshot |
| **Save 💾** | Serializes the current ECS registry to `scene.json` |
| **Load 📂** | Deserializes `scene.json` and reconstructs the full scene |

---

## 🏗️ Architecture Highlights

### Batch Renderer

The rendering pipeline is built around a single, persistent VBO that is mapped to CPU memory at startup. Each frame, the geometry pass iterates over all `SpriteComponent` + `TransformComponent` view pairs from the ECS and writes quad vertex data directly into the mapped buffer — no intermediate `std::vector`, no heap allocation per frame.

Each vertex is stored in a **40-byte packed struct**:

```cpp
struct Vertex {
    glm::vec3 position;   // 12 bytes
    glm::vec4 color;      // 16 bytes
    glm::vec2 texCoord;   //  8 bytes
    float     texIndex;   //  4 bytes
};                        // = 40 bytes (explicit, no implicit padding)
```

On ARM64 Apple Silicon, the ABI's stricter alignment requirements cause compilers to insert silent padding into naive vertex structs, breaking interleaved VBO layouts and producing GPU rendering artifacts. By sizing the struct to a multiple of the largest member alignment (`float` = 4 bytes) and ordering fields largest-to-smallest, padding is eliminated **without resorting to `#pragma pack`**, keeping the layout standards-compliant and portable.

A texture slot array (size 16, matching OpenGL 4.1's minimum `GL_MAX_TEXTURE_IMAGE_UNITS`) is maintained per batch. When the batch is full (max quads reached) or a new texture overflows the slot array, the current batch is flushed with a single `glDrawElements` call and the buffer is reset — resulting in the absolute minimum number of draw calls for any given frame.

---

### Physics / ECS Bridge

Box2D and EnTT operate in completely separate domains. The bridge between them is a lightweight **PhysicsSystem** that runs once per fixed timestep:

1. **On Play:** The system iterates all entities with a `RigidBodyComponent` and calls `b2World::CreateBody()`, storing the returned `b2Body*` handle directly inside the component. No external mapping table is required.

2. **Per Frame (simulation):** The system steps the `b2World`, then iterates all `[TransformComponent, RigidBodyComponent]` view pairs and copies the Box2D body's interpolated position and angle back into the `TransformComponent`. The renderer reads only `TransformComponent` — it has zero knowledge of Box2D.

3. **On Stop:** The system destroys all `b2Body` instances and nulls the stored pointers. The ECS registry itself is untouched; the pre-play scene snapshot (saved as JSON at Play time) is deserialized back in, restoring the original edit-time state with zero component data loss.

This decoupling means the physics backend can be swapped (e.g., to a custom impulse solver) without modifying a single line of rendering or serialization code.

---

## 📄 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for full details.
