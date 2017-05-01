// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"
#include "service_handle.hpp"

#include <Windows.h>

#include <utility>

namespace libservice
{
    class ServiceManager
    {
    public:
        static ServiceManager open();

        ServiceManager(ServiceManager&& other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
        }

        ServiceManager& operator=(ServiceManager other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
            return *this;
        }

        void swap(ServiceManager& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(handle, other.handle);
        }

        operator SC_HANDLE() const
        {
            return handle;
        }

    private:
        ServiceManager(ServiceHandle handle)
            : handle(std::move(handle))
        { }

        ServiceHandle handle;

        ServiceManager(const ServiceManager&) = delete;
    };

    inline void swap(ServiceManager& a, ServiceManager& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    inline void swap<libservice::ServiceManager>(
        libservice::ServiceManager& a,
        libservice::ServiceManager& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
