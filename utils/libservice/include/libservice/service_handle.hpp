/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include "common.hpp"

#include <Windows.h>

#include <memory>
#include <utility>

namespace libservice
{
    class ServiceHandle
    {
    public:
        ServiceHandle() = default;

        explicit ServiceHandle(SC_HANDLE raw)
            : m_impl(raw)
        { }

        ServiceHandle(ServiceHandle&& other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
        }

        ServiceHandle& operator=(ServiceHandle other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
            return *this;
        }

        explicit operator bool() const
        {
            return static_cast<bool>(m_impl);
        }

        explicit operator SC_HANDLE() const
        {
            return m_impl.get();
        }

        void swap(ServiceHandle& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(m_impl, other.m_impl);
        }

    private:
        struct Deleter
        {
            void operator()(SC_HANDLE raw)
            {
                ::CloseServiceHandle(raw);
            }
        };

        std::unique_ptr<SC_HANDLE__, Deleter> m_impl;

        ServiceHandle(const ServiceHandle&) = delete;
    };

    void swap(ServiceHandle&, ServiceHandle&) LIBSERVICE_NOEXCEPT;
}

namespace std
{
    template <>
    void swap<libservice::ServiceHandle>(
        libservice::ServiceHandle&,
        libservice::ServiceHandle&) LIBSERVICE_NOEXCEPT;
}
