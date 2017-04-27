// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "libsimple/all.hpp"

#include "libservice/all.hpp"

#include <Windows.h>

namespace libsimple
{
    namespace
    {
        const char* const device_path = "\\\\.\\simple_device1";
        const auto exchange_ints_ctl_code = CTL_CODE(0x8001, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS);
    }

    Device::Device()
        : libservice::Device(libservice::Device::open(device_path))
    { }

    unsigned int Device::exchange_ints(unsigned int src) const
    {
        unsigned int dest;

        send_control_code(
            exchange_ints_ctl_code,
            &src,
            sizeof(src),
            &dest,
            sizeof(dest));

        return dest;
    }
}
