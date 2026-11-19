#include "Graphics.hh"

#include "check.hh"

Graphics::Graphics(::HWND hwnd, int width, int height) : m_width { width }, m_height { height }
{

    // create the device and context, using the debug layer in debug builds when it is installed
    ::UINT flags { 0u };
    const ::D3D_FEATURE_LEVEL levels[] { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

#ifndef NDEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ::HRESULT result { ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, 2u, D3D11_SDK_VERSION, &m_device, &m_feature_level, &m_context) };

    if (FAILED(result) && flags != 0u)
    {

        result = ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0u, levels, 2u, D3D11_SDK_VERSION, &m_device, &m_feature_level, &m_context);

    }

    check(result, "D3D11CreateDevice");

    // walk from the device up to the DXGI factory that creates swap chains
    ::Microsoft::WRL::ComPtr<::IDXGIDevice> dxgi_device {};
    ::Microsoft::WRL::ComPtr<::IDXGIAdapter> adapter {};
    ::Microsoft::WRL::ComPtr<::IDXGIFactory2> factory {};
    check(m_device.As(&dxgi_device), "query IDXGIDevice");
    check(dxgi_device->GetAdapter(&adapter), "IDXGIDevice::GetAdapter");
    check(adapter->GetParent(IID_PPV_ARGS(&factory)), "IDXGIAdapter::GetParent");

    // find out whether presenting without vsync can tear (needed for uncapped FPS)
    ::Microsoft::WRL::ComPtr<::IDXGIFactory5> factory5 {};

    if (SUCCEEDED(factory.As(&factory5)))
    {

        ::BOOL allow_tearing { FALSE };

        if (SUCCEEDED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing))))
        {

            m_tearing = allow_tearing == TRUE;

        }

    }

    // create the swap chain: two buffers, flip model, attached to the window
    ::DXGI_SWAP_CHAIN_DESC1 swap_desc {};
    swap_desc.Width = static_cast<::UINT>(width);
    swap_desc.Height = static_cast<::UINT>(height);
    swap_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_desc.SampleDesc.Count = 1u;
    swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_desc.BufferCount = 2u;
    swap_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_desc.Flags = m_tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;

    check(factory->CreateSwapChainForHwnd(m_device.Get(), hwnd, &swap_desc, nullptr, nullptr, &m_swap_chain), "CreateSwapChainForHwnd");
    check(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER), "MakeWindowAssociation");

    // make a render target view of the back buffer
    ::Microsoft::WRL::ComPtr<::ID3D11Texture2D> back_buffer {};
    check(m_swap_chain->GetBuffer(0u, IID_PPV_ARGS(&back_buffer)), "IDXGISwapChain1::GetBuffer");
    check(m_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_rtv), "CreateRenderTargetView");

    // make the depth buffer and its view
    ::D3D11_TEXTURE2D_DESC depth_desc {};
    depth_desc.Width = static_cast<::UINT>(width);
    depth_desc.Height = static_cast<::UINT>(height);
    depth_desc.MipLevels = 1u;
    depth_desc.ArraySize = 1u;
    depth_desc.Format = DXGI_FORMAT_D32_FLOAT;
    depth_desc.SampleDesc.Count = 1u;
    depth_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    check(m_device->CreateTexture2D(&depth_desc, nullptr, &m_depth_texture), "CreateTexture2D (depth)");
    check(m_device->CreateDepthStencilView(m_depth_texture.Get(), nullptr, &m_dsv), "CreateDepthStencilView");

}

void Graphics::beginFrame(float r, float g, float b, float a)
{

    // flip-model swap chains must rebind the back buffer every frame
    const float color[4] { r, g, b, a };
    ::ID3D11RenderTargetView* rtv { m_rtv.Get() };
    m_context->OMSetRenderTargets(1u, &rtv, m_dsv.Get());
    m_context->ClearRenderTargetView(m_rtv.Get(), color);
    m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

    // draw into the whole window
    const ::D3D11_VIEWPORT viewport { 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, 1.0f };
    m_context->RSSetViewports(1u, &viewport);

}

void Graphics::present(bool vsync)
{

    const ::UINT flags { (!vsync && m_tearing) ? DXGI_PRESENT_ALLOW_TEARING : 0u };
    check(m_swap_chain->Present(vsync ? 1u : 0u, flags), "IDXGISwapChain1::Present");

}

::ID3D11Device* Graphics::device() const
{

    return m_device.Get();

}

::ID3D11DeviceContext* Graphics::context() const
{

    return m_context.Get();

}

::D3D_FEATURE_LEVEL Graphics::featureLevel() const
{

    return m_feature_level;

}
