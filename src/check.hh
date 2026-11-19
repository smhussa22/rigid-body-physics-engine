#ifndef CHECK_HH
#define CHECK_HH

#include <Windows.h>

#include <format>
#include <stdexcept>

// throws with a readable message when a Direct3D / DXGI call returns a failed HRESULT
inline void check(::HRESULT result, const char* what)
{

    if (FAILED(result))
    {

        throw std::runtime_error { std::format("{} failed (HRESULT {:#x})", what, static_cast<unsigned long>(result)) };

    }

}

#endif // CHECK_HH
