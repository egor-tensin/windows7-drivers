/**
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include "common.hpp"
#include "singleton.hpp"

#include <string>
#include <system_error>

namespace libservice
{
    class WinErrorCategory : public std::error_category
                           , public Singleton<WinErrorCategory>
    {
    public:
        const char* name() const LIBSERVICE_NOEXCEPT { return "windows"; }

        std::string message(int) const;

    private:
        friend class Singleton<WinErrorCategory>;
    };
}

#define LIBSERVICE_ERROR_PREFIX "Error in function '" \
                                LIBSERVICE_FUNCTION_NAME \
                                "' at file '" \
                                LIBSERVICE_FILE_PATH \
                                "', line " \
                                LIBSERVICE_LINE_NUMBER_STRING
