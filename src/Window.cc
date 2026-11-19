#include "Window.hh"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <stdexcept>

Window::Window(int width, int height, const char* title) : m_width { width }, m_height { height }
{

    if (!::glfwInit())
    {

        throw std::runtime_error { "glfwInit failed" };

    }

    // no OpenGL context (Direct3D draws instead) and a fixed size
    ::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    ::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window.reset(::glfwCreateWindow(width, height, title, nullptr, nullptr));

    if (!m_window)
    {

        ::glfwTerminate();
        throw std::runtime_error { "glfwCreateWindow failed" };

    }

}

Window::~Window()
{

    m_window.reset();
    ::glfwTerminate();

}

bool Window::shouldClose() const
{

    return ::glfwWindowShouldClose(m_window.get());

}

void Window::close()
{

    ::glfwSetWindowShouldClose(m_window.get(), true);

}

void Window::pollEvents()
{

    ::glfwPollEvents();

}

bool Window::key(int k) const
{

    return ::glfwGetKey(m_window.get(), k) == GLFW_PRESS;

}

::HWND Window::hwnd() const
{

    return ::glfwGetWin32Window(m_window.get());

}

int Window::width() const
{

    return m_width;

}

int Window::height() const
{

    return m_height;

}

::GLFWwindow* Window::handle() const
{

    return m_window.get();

}
