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
        static bool does_exist(const ServiceManager&,
                               const std::string& name);

        static Service open(const ServiceManager&,
                            const std::string& name);
        static Service install(const ServiceManager&,
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
            swap(m_handle, other.m_handle);
        }

    private:
        explicit Service(ServiceHandle h)
            : m_handle(std::move(h))
        { }

        ServiceHandle m_handle;

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
