/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include "common.hpp"
#include "handle.hpp"

#include <Windows.h>

#include <cstddef>

#include <string>
#include <utility>

namespace libservice
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

    void swap(Device&, Device&) LIBSERVICE_NOEXCEPT;
}

namespace std
{
    template <>
    void swap<libservice::Device>(
        libservice::Device&,
        libservice::Device&) LIBSERVICE_NOEXCEPT;
}
