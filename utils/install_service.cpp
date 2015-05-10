/**
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/interface.hpp"

#include <iostream>
#include <system_error>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " NAME SYS_PATH\n";
        return 1;
    }

    try
    {
        libservice::Service::install(libservice::ServiceManager::open(), argv[1], argv[2]);
    }
    catch (const std::system_error& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
