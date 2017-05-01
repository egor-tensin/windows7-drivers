// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "service/all.hpp"

#include <Windows.h>

#include <exception>
#include <iostream>
#include <system_error>

int main()
{
    try
    {
        throw std::system_error(
            ERROR_FILE_NOT_FOUND,
            service::WindowsErrorCategory::get(),
            LIBSERVICE_ERROR_PREFIX);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
