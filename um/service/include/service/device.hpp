// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"
#include "handle.hpp"

#include <Windows.h>

#include <cstddef>

#include <string>
#include <utility>

namespace service
{
    class Device
    {
    public:
        typedef DWORD Code;

        static Device open(const std::string& path);

        Device(Device&& other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
        }

        Device& operator=(Device other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
            return *this;
        }

        void swap(Device& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(handle, other.handle);
        }

        std::size_t get_required_output_size(
            Code code,
            const void* in_buf,
            std::size_t in_buf_size) const;

        std::size_t send_control_code(
            Code code,
            const void* in_buf,
            std::size_t in_buf_size,
            void* out_buf,
            std::size_t out_buf_size) const;

    private:
        Device(Handle handle)
            : handle(std::move(handle))
        { }

        Handle handle;

        Device(const Device&) = delete;
    };

    inline void swap(Device& a, Device& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    inline void swap<service::Device>(
        service::Device& a,
        service::Device& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
