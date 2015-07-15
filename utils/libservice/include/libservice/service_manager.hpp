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

    void swap(ServiceManager& a, ServiceManager& b) LIBSERVICE_NOEXCEPT;
}

namespace std
{
    template <>
    void swap<libservice::ServiceManager>(
        libservice::ServiceManager&,
        libservice::ServiceManager&) LIBSERVICE_NOEXCEPT;
}
