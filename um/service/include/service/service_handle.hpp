// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"

#include <Windows.h>

#include <memory>
#include <utility>

namespace service
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

    inline void swap(ServiceHandle& a, ServiceHandle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    inline void swap<service::ServiceHandle>(
        service::ServiceHandle& a,
        service::ServiceHandle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
