// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "libservice/all.hpp"

#include <string>

namespace libnt_path_converter
{
    class Device : libservice::Device
    {
    public:
        Device();

        std::wstring convert_nt_path(const std::wstring&);
    };
}
