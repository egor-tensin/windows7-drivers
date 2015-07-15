/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/all.hpp"

#include <Windows.h>

#include <string>

namespace libservice
{
    std::string WindowsErrorCategory::message(int code) const
    {
        char* buf_ptr;

        const auto nbwritten = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<char*>(&buf_ptr),
            0,
            NULL);

        if (0 == nbwritten)
        {
            LocalFree(buf_ptr);
            return "Couldn't format error message";
        }

        std::string str(buf_ptr, nbwritten - 2);
        LocalFree(buf_ptr);
        return str;
    }
}
