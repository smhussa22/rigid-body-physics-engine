#ifndef BUFFER_HH
#define BUFFER_HH

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#include <cstddef>

// RAII wrapper around a GPU structured buffer (e.g. particle data) with views for shader reads and writes
class Buffer
{

public:

    // creates a buffer of count elements of element_size bytes each, optionally filled from data (nullptr leaves it zeroed)
    Buffer(::ID3D11Device* device, std::size_t element_size, std::size_t count, const void* data);

    ~Buffer() = default;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    // uploads new data over the whole buffer (data must hold element_size * count bytes)
    void write(::ID3D11DeviceContext* context, const void* data);

    // copies the whole buffer back to the CPU; slow (waits for the GPU), so use it for checks and debugging only
    void read(::ID3D11DeviceContext* context, void* out) const;

    // read-only view, bound with CSSetShaderResources for shaders that only read this buffer
    ::ID3D11ShaderResourceView* srv() const;

    // read-write view, bound with CSSetUnorderedAccessViews for shaders that write this buffer
    ::ID3D11UnorderedAccessView* uav() const;

    // the underlying buffer resource
    ::ID3D11Buffer* buffer() const;

private:

    ::Microsoft::WRL::ComPtr<::ID3D11Buffer> m_buffer {};              // GPU memory
    ::Microsoft::WRL::ComPtr<::ID3D11ShaderResourceView> m_srv {};     // read-only shader view
    ::Microsoft::WRL::ComPtr<::ID3D11UnorderedAccessView> m_uav {};    // read-write shader view
    std::size_t m_byte_size { 0 };                                     // total size in bytes

};

#endif // BUFFER_HH
