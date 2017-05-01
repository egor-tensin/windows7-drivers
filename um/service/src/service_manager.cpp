// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "libservice/all.hpp"

#include <Windows.h>

#include <system_error>

namespace libservice
{
    ServiceManager ServiceManager::open()
    {
        const auto raw = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (NULL == raw)
        {
            const auto ec = GetLastError();
            throw std::system_error(
                ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
        }

        return ServiceHandle(raw);
    }
}
