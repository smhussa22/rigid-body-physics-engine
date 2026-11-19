#include "Window.hh"

#include <stdexcept>

Window::Window(int width, int height, const char* title)
{

    if (!::glfwInit())
    {

        throw std::runtime_error { "glfwInit failed" };

    }

    // request an OpenGL 4.6 core context
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window.reset(::glfwCreateWindow(width, height, title, nullptr, nullptr));

    if (!m_window)
    {

        ::glfwTerminate();
        throw std::runtime_error { "glfwCreateWindow failed" };

    }

    // bind the context, turn vsync off so benchmarks aren't capped, then load OpenGL
    ::glfwMakeContextCurrent(m_window.get());
    ::glfwSwapInterval(0);

    if (!::gladLoadGLLoader(reinterpret_cast<::GLADloadproc>(::glfwGetProcAddress)))
    {

        m_window.reset();
        ::glfwTerminate();
        throw std::runtime_error { "gladLoadGLLoader failed" };

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

void Window::swap()
{

    ::glfwSwapBuffers(m_window.get());

}

void Window::pollEvents()
{

    ::glfwPollEvents();

}

bool Window::key(int k) const
{

    return ::glfwGetKey(m_window.get(), k) == GLFW_PRESS;

}

::GLFWwindow* Window::handle() const
{

    return m_window.get();

}
