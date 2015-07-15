/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

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
