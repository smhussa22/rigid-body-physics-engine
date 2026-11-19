cmake_minimum_required(VERSION 3.28)
project(rigid_body_physics_engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5) # glad v1 predates CMake 4
include(FetchContent)

# GLFW (window + input)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4 GIT_SHALLOW TRUE)

# GLAD v1 (OpenGL 4.6 core loader, generated at configure time with Python's stdlib only)
set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
set(GLAD_API "gl=4.6" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c" CACHE STRING "" FORCE)
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v0.1.36 GIT_SHALLOW TRUE)

# GLM (math)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1 GIT_SHALLOW TRUE)

# Dear ImGui (no CMake of its own, so built below)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.91.9 GIT_SHALLOW TRUE)

# GoogleTest (unit tests)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.17.0 GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(glfw glad glm imgui googletest)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw)

add_executable(rigid_body_physics_engine src/rigid_body_physics_engine.cc src/Window.cc)
target_link_libraries(rigid_body_physics_engine PRIVATE glfw glad glm::glm imgui)

# tests: add new test files to this list
enable_testing()
add_executable(tests tests/smoke_test.cc)
target_link_libraries(tests PRIVATE GTest::gtest_main glm::glm)
include(GoogleTest)
gtest_discover_tests(tests)
