// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "service/all.hpp"

namespace libsimple
{
    class Device : libservice::Device
    {
    public:
        Device();

        unsigned int exchange_ints(unsigned int) const;
    };
}
