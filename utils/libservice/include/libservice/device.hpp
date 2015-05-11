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

#include <string>
#include <utility>

namespace libservice
{
    class Device
    {
    public:
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
            swap(m_handle, other.m_handle);
        }

        DWORD get_required_output_size(DWORD code,
                                       const void* in_buf,
                                       DWORD in_buf_size) const;

        DWORD send_control_code(DWORD code,
                                const void* in_buf,
                                DWORD in_buf_size,
                                void* out_buf,
                                DWORD out_buf_size) const;

    private:
        explicit Device(Handle h)
            : m_handle(std::move(h))
        { }

        Handle m_handle;

        Device(const Device&) = delete;
    };

    void swap(libservice::Device&, libservice::Device&) LIBSERVICE_NOEXCEPT;
}

namespace std
{
    template <>
    void swap<libservice::Device>(
        libservice::Device&, libservice::Device&) LIBSERVICE_NOEXCEPT;
}
