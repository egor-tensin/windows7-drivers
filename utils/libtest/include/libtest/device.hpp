/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include "libservice/all.hpp"

namespace libtest
{
    class Device : libservice::Device
    {
    public:
        Device();

        unsigned int exchange_ints(unsigned int) const;
    };
}
