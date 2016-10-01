// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "libtest/all.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
    bool parse_int(unsigned int& dest, const std::string& src)
    {
        std::istringstream iss(src);
        char c;
        return iss >> dest && !iss.get(c);
    }
}

int main(int argc, char* argv[])
{
    try
    {
        unsigned int src;

        if (argc != 2 || !parse_int(src, argv[1]))
        {
            std::cout << "Usage: " << argv[0] << " N\n";
            return 1;
        }

        std::cout << libtest::Device().exchange_ints(src) << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
