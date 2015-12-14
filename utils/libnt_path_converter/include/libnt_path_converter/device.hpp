/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

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
