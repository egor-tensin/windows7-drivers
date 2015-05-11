/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libnt_path_converter/all.hpp"

#include <iostream>
#include <exception>

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        libnt_path_converter::Device dev;
        for (int i = 1; i < argc; ++i)
            std::wcout << dev.convert_nt_path(argv[i]) << L"\n";
    }
    catch (const std::exception& e)
    {
        std::wcerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
