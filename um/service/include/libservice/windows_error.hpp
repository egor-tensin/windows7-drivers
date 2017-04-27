// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"
#include "singleton.hpp"

#include <string>
#include <system_error>

namespace libservice
{
    class WindowsErrorCategory
        : public std::error_category
        , public Singleton<WindowsErrorCategory>
    {
    public:
        const char* name() const LIBSERVICE_NOEXCEPT { return "Windows"; }

        std::string message(int) const;

    private:
        friend class Singleton<WindowsErrorCategory>;
    };
}

#define LIBSERVICE_ERROR_PREFIX \
    "Error in function '" LIBSERVICE_FUNCTION_NAME "' at file '" LIBSERVICE_FILE_PATH "', line " LIBSERVICE_LINE_NUMBER_STRING
