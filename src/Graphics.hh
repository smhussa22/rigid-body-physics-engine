#ifndef GRAPHICS_HH
#define GRAPHICS_HH

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_5.h>
#include <wrl/client.h>

// owns the Direct3D 11 device, immediate context, swap chain, back buffer view and depth buffer
class Graphics
{

public:

    // creates the device (feature level 11_0+), a flip-model swap chain on hwnd, and a depth buffer
    Graphics(::HWND hwnd, int width, int height);

    ~Graphics() = default;

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    // binds the back buffer + depth buffer, clears them to the given color, and sets the full-window viewport
    void beginFrame(float r, float g, float b, float a);

    // shows the finished frame; vsync off uses tearing when the display supports it (for benchmarking)
    void present(bool vsync);

    // the device, used to create buffers, shaders, and textures
    ::ID3D11Device* device() const;

    // the immediate context, used to issue draw and dispatch calls
    ::ID3D11DeviceContext* context() const;

    // the feature level the device was created with
    ::D3D_FEATURE_LEVEL featureLevel() const;

private:

    ::Microsoft::WRL::ComPtr<::ID3D11Device> m_device {};              // creates GPU resources
    ::Microsoft::WRL::ComPtr<::ID3D11DeviceContext> m_context {};      // issues GPU commands
    ::Microsoft::WRL::ComPtr<::IDXGISwapChain1> m_swap_chain {};       // presents frames to the window
    ::Microsoft::WRL::ComPtr<::ID3D11RenderTargetView> m_rtv {};       // view of the back buffer we draw into
    ::Microsoft::WRL::ComPtr<::ID3D11Texture2D> m_depth_texture {};    // depth buffer memory
    ::Microsoft::WRL::ComPtr<::ID3D11DepthStencilView> m_dsv {};       // view of the depth buffer
    ::D3D_FEATURE_LEVEL m_feature_level { D3D_FEATURE_LEVEL_11_0 };    // level the device actually got
    int m_width { 0 };                                                 // back buffer width in pixels
    int m_height { 0 };                                                // back buffer height in pixels
    bool m_tearing { false };                                          // display supports uncapped (tearing) present

};

#endif // GRAPHICS_HH
