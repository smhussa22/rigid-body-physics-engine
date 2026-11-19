#ifndef WINDOW_HH
#define WINDOW_HH

#include <GLFW/glfw3.h>
#include <Windows.h>

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

// RAII wrapper around a fixed-size GLFW window used only for windowing and input (no graphics API)
class Window
{

public:

    // inits GLFW and opens a fixed-size window with no OpenGL context
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

    // collects pending OS events (input, close button); call once per frame
    void pollEvents();

    // true while the given GLFW_KEY_* is held, as of the last pollEvents()
    bool key(int k) const;

    // native Win32 window handle, which Direct3D's swap chain attaches to
    ::HWND hwnd() const;

    // window width in pixels
    int width() const;

    // window height in pixels
    int height() const;

    // raw handle, for things like the ImGui backend
    ::GLFWwindow* handle() const;

private:

    WindowPtr m_window { nullptr }; // owned GLFW window
    int m_width { 0 };              // client width in pixels
    int m_height { 0 };             // client height in pixels

};

#endif // WINDOW_HH
