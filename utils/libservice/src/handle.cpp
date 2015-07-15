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
    void swap(Handle& a, Handle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    void swap<libservice::Handle>(
        libservice::Handle& a,
        libservice::Handle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
