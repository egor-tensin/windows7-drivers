/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

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

    void swap(Service&, Service&) LIBSERVICE_NOEXCEPT;
}

namespace std
{
    template <>
    void swap<libservice::Service>(
        libservice::Service&,
        libservice::Service&) LIBSERVICE_NOEXCEPT;
}
