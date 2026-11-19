#include "Window.hh"

#include <print>

int main()
{

    // open a 1280x720 window with an OpenGL 4.6 context and print which GPU we got
    Window window { 1280, 720, "Soft Body Physics Engine" };
    std::println("{} | OpenGL {}", reinterpret_cast<const char*>(::glGetString(GL_RENDERER)), reinterpret_cast<const char*>(::glGetString(GL_VERSION)));

    // keep running until closed (on escape key), keep redrawing and handle input
    while (!window.shouldClose())
    {

        if (window.key(GLFW_KEY_ESCAPE))
        {

            window.close();

        }

        ::glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.swap();
        window.pollEvents();

    }

}

