/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/all.hpp"

#include <Windows.h>

#include <exception>
#include <iostream>

int main()
{
    try
    {
        throw std::system_error(ERROR_FILE_NOT_FOUND, libservice::WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return -1;
    }
    return 0;
}
