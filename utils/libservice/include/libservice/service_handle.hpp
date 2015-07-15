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

        ServiceHandle(SC_HANDLE raw)
            : impl(raw)
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

        operator bool() const
        {
            return static_cast<bool>(impl);
        }

        operator SC_HANDLE() const
        {
            return impl.get();
        }

        void swap(ServiceHandle& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(impl, other.impl);
        }

    private:
        struct Deleter
        {
            void operator()(SC_HANDLE raw)
            {
                CloseServiceHandle(raw);
            }
        };

        std::unique_ptr<SC_HANDLE__, Deleter> impl;

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
