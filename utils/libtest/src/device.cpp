/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libtest/all.hpp"

#include "libservice/all.hpp"

#include <Windows.h>

namespace libtest
{
    namespace
    {
        const char* const device_path = "\\\\.\\test_device1";
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
