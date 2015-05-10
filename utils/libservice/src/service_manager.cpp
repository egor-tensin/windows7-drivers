/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/common.hpp"
#include "libservice/service_handle.hpp"
#include "libservice/service_manager.hpp"
#include "libservice/windows_error.hpp"

#include <Windows.h>

#include <system_error>
#include <utility>

namespace libservice
{
    ServiceManager ServiceManager::open()
    {
        SC_HANDLE raw = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (NULL == raw)
        {
            const auto ec = GetLastError();
            throw std::system_error(ec, WinErrorCategory::get());
        }

        return ServiceManager(ServiceHandle(raw));
    }

    void swap(ServiceManager& a, ServiceManager& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    void swap<libservice::ServiceManager>(
        libservice::ServiceManager& a,
        libservice::ServiceManager& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
