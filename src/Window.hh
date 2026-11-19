#ifndef WINDOW_HH
#define WINDOW_HH

#include <glad/glad.h> // must come before GLFW
#include <GLFW/glfw3.h>

#include <memory>

// destroys a GLFW window when its owning pointer goes out of scope
struct WindowDeleter
{

    void operator()(::GLFWwindow* window) const noexcept
    {

        if (window != nullptr)
        {

            ::glfwDestroyWindow(window);

        }

    }

};

using WindowPtr = std::unique_ptr<::GLFWwindow, WindowDeleter>;

// RAII wrapper around a GLFW window plus its OpenGL 4.6 core context
class Window
{

public:

    // inits GLFW, opens the window, makes the context current, and loads OpenGL via GLAD
    Window(int width, int height, const char* title);

    // destroys the window and shuts GLFW down
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    // true once the user closed the window or close() was called
    bool shouldClose() const;

    // asks the loop to end on the next check of shouldClose()
    void close();

    // shows the frame drawn into the hidden back buffer
    void swap();

    // collects pending OS events (input, resize, close button); call once per frame
    void pollEvents();

    // true while the given GLFW_KEY_* is held, as of the last pollEvents()
    bool key(int k) const;

    // raw handle, for things like the ImGui backend
    ::GLFWwindow* handle() const;

private:

    WindowPtr m_window { nullptr }; // owned GLFW window

};

#endif // WINDOW_HH
