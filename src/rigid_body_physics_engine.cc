#include "Graphics.hh"
#include "Window.hh"

#include <print>

int main()
{

    // open a 1280x720 window and bring up Direct3D 11 on it
    Window window { 1280, 720, "Rigid Body Physics Engine" };
    Graphics graphics { window.hwnd(), window.width(), window.height() };
    std::println("Direct3D 11 ready, feature level {:#x}", static_cast<unsigned>(graphics.featureLevel()));

    // keep running until closed (on escape key), keep redrawing and handle input
    while (!window.shouldClose())
    {

        if (window.key(GLFW_KEY_ESCAPE))
        {

            window.close();

        }

        graphics.beginFrame(0.1f, 0.1f, 0.15f, 1.0f);
        graphics.present(false);
        window.pollEvents();

    }

}

