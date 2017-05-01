// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"
#include "service_handle.hpp"
#include "service_manager.hpp"

#include <string>
#include <utility>

namespace libservice
{
    class Service
    {
    public:
        static bool exists(
            const ServiceManager&,
            const std::string& name);

        static Service open(
            const ServiceManager&,
            const std::string& name);

        static Service install(
            const ServiceManager&,
            const std::string& name,
            const std::string& bin_path);

        void start() const;
        void stop() const;
        void uninstall() const;

        Service(Service&& other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
        }

        Service& operator=(Service other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
            return *this;
        }

        void swap(Service& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(handle, other.handle);
        }

    private:
        Service(ServiceHandle handle)
            : handle(std::move(handle))
        { }

        ServiceHandle handle;

        Service(const Service&) = delete;
    };

    inline void swap(Service& a, Service& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    inline void swap<libservice::Service>(
        libservice::Service& a,
        libservice::Service& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
