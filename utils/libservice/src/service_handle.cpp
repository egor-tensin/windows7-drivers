// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "libservice/all.hpp"

#include <utility>

namespace libservice
{
    void swap(ServiceHandle& a, ServiceHandle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    void swap<libservice::ServiceHandle>(
        libservice::ServiceHandle& a,
        libservice::ServiceHandle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
