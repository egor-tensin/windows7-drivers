// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"
#include "singleton.hpp"

#include <sstream>
#include <string>
#include <system_error>

namespace service
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

    namespace error
    {
        inline std::string build_what(
            const char* function,
            const char* file,
            int line)
        {
            std::ostringstream oss;
            oss << "Error in function '" << function
                << "' at file '" << file
                << "', line " << line;
            return oss.str();
        }
    }
}

#if defined(_MSC_VER)
#define LIBSERVICE_ERROR_PREFIX \
    "Error in function '" LIBSERVICE_FUNCTION_NAME "' at file '" LIBSERVICE_FILE_PATH "', line " LIBSERVICE_LINE_NUMBER_STRING
#elif defined(__GNUC__)
#define LIBSERVICE_ERROR_PREFIX \
    service::error::build_what(LIBSERVICE_FUNCTION_NAME, LIBSERVICE_FILE_PATH, LIBSERVICE_LINE_NUMBER)
#else
#define LIBSERVICE_ERROR_PREFIX "Error"
#endif
