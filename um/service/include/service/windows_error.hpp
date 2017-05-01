// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"
#include "singleton.hpp"

#include <Windows.h>

#include <sstream>
#include <string>
#include <system_error>

namespace service
{
    namespace windows_error
    {
        class Category
            : public std::error_category
            , public Singleton<Category>
        {
        public:
            const char* name() const LIBSERVICE_NOEXCEPT { return "Windows"; }

            std::string message(int) const;

        private:
            friend class Singleton<Category>;
        };

        inline std::string build_message_prefix(
            const char* file,
            int line,
            const char* function)
        {
            std::ostringstream oss;
            oss << "Error in function '" << function
                << "' at file '" << file
                << "', line " << line;
            return oss.str();
        }

        inline std::system_error make(
            DWORD code,
            const char* file,
            int line,
            const char* function)
        {
            return std::system_error(
                code,
                Category::get(),
                build_message_prefix(file, line, function));
        }
    }
}
