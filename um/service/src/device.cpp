// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "service/all.hpp"

#include <Windows.h>

#include <cstddef>

#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>

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

        if (in_buf_size > std::numeric_limits<DWORD>::max())
            throw std::range_error("input buffer size is too large");

        std::size_t nbwritten = DeviceIoControl(
            handle,
            code,
            const_cast<void*>(in_buf),
            static_cast<DWORD>(in_buf_size),
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

        if (in_buf_size > std::numeric_limits<DWORD>::max())
            throw std::range_error("input buffer size is too large");
        if (out_buf_size > std::numeric_limits<DWORD>::max())
            throw std::range_error("output buffer size is too large");

        std::size_t nbwritten = DeviceIoControl(
            handle,
            code,
            const_cast<void*>(in_buf),
            static_cast<DWORD>(in_buf_size),
            out_buf,
            static_cast<DWORD>(out_buf_size),
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
}
