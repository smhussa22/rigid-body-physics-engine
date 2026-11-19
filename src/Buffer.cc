#include "Buffer.hh"

#include "check.hh"

#include <cstring>

Buffer::Buffer(::ID3D11Device* device, std::size_t element_size, std::size_t count, const void* data) : m_byte_size { element_size * count }
{

    // allocate a structured buffer that shaders can both read and write
    ::D3D11_BUFFER_DESC desc {};
    desc.ByteWidth = static_cast<::UINT>(m_byte_size);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = static_cast<::UINT>(element_size);

    const ::D3D11_SUBRESOURCE_DATA initial { data, 0u, 0u };
    check(device->CreateBuffer(&desc, data != nullptr ? &initial : nullptr, &m_buffer), "CreateBuffer");

    // make the read-only and read-write views over the whole buffer
    ::D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc {};
    srv_desc.Format = DXGI_FORMAT_UNKNOWN;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srv_desc.Buffer.NumElements = static_cast<::UINT>(count);
    check(device->CreateShaderResourceView(m_buffer.Get(), &srv_desc, &m_srv), "CreateShaderResourceView");

    ::D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc {};
    uav_desc.Format = DXGI_FORMAT_UNKNOWN;
    uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uav_desc.Buffer.NumElements = static_cast<::UINT>(count);
    check(device->CreateUnorderedAccessView(m_buffer.Get(), &uav_desc, &m_uav), "CreateUnorderedAccessView");

}

void Buffer::write(::ID3D11DeviceContext* context, const void* data)
{

    context->UpdateSubresource(m_buffer.Get(), 0u, nullptr, data, 0u, 0u);

}

void Buffer::read(::ID3D11DeviceContext* context, void* out) const
{

    // GPU-only buffers can't be mapped, so copy into a temporary CPU-readable staging buffer first
    ::Microsoft::WRL::ComPtr<::ID3D11Device> device {};
    m_buffer->GetDevice(&device);

    ::D3D11_BUFFER_DESC desc {};
    desc.ByteWidth = static_cast<::UINT>(m_byte_size);
    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    ::Microsoft::WRL::ComPtr<::ID3D11Buffer> staging {};
    check(device->CreateBuffer(&desc, nullptr, &staging), "CreateBuffer (staging)");

    context->CopyResource(staging.Get(), m_buffer.Get());

    ::D3D11_MAPPED_SUBRESOURCE mapped {};
    check(context->Map(staging.Get(), 0u, D3D11_MAP_READ, 0u, &mapped), "Map (staging)");
    std::memcpy(out, mapped.pData, m_byte_size);
    context->Unmap(staging.Get(), 0u);

}

::ID3D11ShaderResourceView* Buffer::srv() const
{

    return m_srv.Get();

}

::ID3D11UnorderedAccessView* Buffer::uav() const
{

    return m_uav.Get();

}

::ID3D11Buffer* Buffer::buffer() const
{

    return m_buffer.Get();

}
