// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "service/all.hpp"

#include <Windows.h>

#include <exception>
#include <iostream>

int main()
{
    try
    {
        throw service::windows_error::make(
            ERROR_FILE_NOT_FOUND,
            __FILE__, __LINE__, __FUNCTION__);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
