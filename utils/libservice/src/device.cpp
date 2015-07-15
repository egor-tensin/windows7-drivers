/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/all.hpp"

#include <Windows.h>

#include <cstddef>

#include <string>
#include <system_error>
#include <utility>

namespace libservice
{
    namespace
    {
        Handle open_device(const std::string& path)
        {
            const auto raw = CreateFileA(
                path.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

            if (INVALID_HANDLE_VALUE == raw)
            {
                const auto ec = GetLastError();
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return Handle(raw);
        }
    }

    Device Device::open(const std::string& path)
    {
        return Device(open_device(path));
    }

    std::size_t Device::get_required_output_size(
        Code code,
        const void* in_buf,
        std::size_t in_buf_size) const
    {
        DWORD nbreq;

        std::size_t nbwritten = DeviceIoControl(
            handle,
            code,
            const_cast<void*>(in_buf),
            in_buf_size,
            NULL,
            0,
            &nbreq,
            NULL);

        if (0 == nbwritten)
        {
            const auto ec = GetLastError();

            switch (ec)
            {
                case ERROR_MORE_DATA:
                    return nbreq;

                default:
                    throw std::system_error(
                        ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        return nbwritten;
    }

    std::size_t Device::send_control_code(
        Code code,
        const void* in_buf,
        std::size_t in_buf_size,
        void* out_buf,
        std::size_t out_buf_size) const
    {
        DWORD nbreq;

        std::size_t nbwritten = DeviceIoControl(
            handle,
            code,
            const_cast<void*>(in_buf),
            in_buf_size,
            out_buf,
            out_buf_size,
            &nbreq,
            NULL);

        if (0 == nbwritten)
        {
            const auto ec = GetLastError();
            throw std::system_error(
                ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
        }

        return nbwritten;
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
