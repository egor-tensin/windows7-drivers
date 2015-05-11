/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/common.hpp"
#include "libservice/device.hpp"
#include "libservice/handle.hpp"
#include "libservice/windows_error.hpp"

#include <Windows.h>

#include <string>
#include <system_error>
#include <utility>

namespace libservice
{
    namespace
    {
        Handle open_device(const std::string& path)
        {
            const auto raw = CreateFileA(path.c_str(),
                                         GENERIC_READ | GENERIC_WRITE,
                                         0,
                                         NULL,
                                         OPEN_EXISTING,
                                         FILE_ATTRIBUTE_NORMAL,
                                         NULL);

            if (INVALID_HANDLE_VALUE == raw)
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return Handle(raw);
        }
    }

    Device Device::open(const std::string& path)
    {
        return Device(open_device(path));
    }

    DWORD Device::get_required_output_size(
        DWORD code,
        const void* in_buf,
        DWORD in_buf_size) const
    {
        DWORD nbreq;

        DWORD ret = DeviceIoControl(
            static_cast<HANDLE>(m_handle),
            code,
            const_cast<void*>(in_buf),
            in_buf_size,
            NULL,
            0,
            &nbreq,
            NULL);

        if (0 == ret)
        {
            const auto ec = GetLastError();

            switch (ec)
            {
                case ERROR_MORE_DATA:
                    return nbreq;

                default:
                    throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        return ret;
    }

    DWORD Device::send_control_code(
        DWORD code,
        const void* in_buf,
        DWORD in_buf_size,
        void* out_buf,
        DWORD out_buf_size) const
    {
        DWORD nbreq;

        DWORD ret = DeviceIoControl(
            static_cast<HANDLE>(m_handle),
            code,
            const_cast<void*>(in_buf),
            in_buf_size,
            out_buf,
            out_buf_size,
            &nbreq,
            NULL);

        if (0 == ret)
        {
            const auto ec = GetLastError();
            throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
        }

        return ret;
    }

    void swap(Device& a, Device& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    void swap<libservice::Device>(
        libservice::Device& a,
        libservice::Device& b)
    {
        a.swap(b);
    }
}
