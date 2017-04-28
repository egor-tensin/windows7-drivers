// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "libnt_namespace/device.hpp"

#include "libservice/all.hpp"

#include <Windows.h>

#include <string>
#include <vector>

namespace libnt_namespace
{
    namespace
    {
        const auto device_path = "\\\\.\\nt_namespace";
        const auto control_code = CTL_CODE(0x8000, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS);
    }

    Device::Device()
        : libservice::Device(libservice::Device::open(device_path))
    { }

    std::wstring Device::convert_nt_path(const std::wstring& src)
    {
        const auto in_buf = src.c_str();
        const auto in_buf_size = (src.size() + 1) * sizeof(wchar_t);

        const auto nbreq = get_required_output_size(
            control_code,
            in_buf,
            in_buf_size);

        std::vector<unsigned char> output(nbreq);

        send_control_code(
            control_code,
            in_buf,
            in_buf_size,
            output.data(),
            nbreq);

        return reinterpret_cast<wchar_t*>(output.data());
    }
}
