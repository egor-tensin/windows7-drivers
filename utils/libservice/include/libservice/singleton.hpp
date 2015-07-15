/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include <mutex>

namespace libservice
{
    template <typename DerivedT>
    class Singleton
    {
    public:
        static DerivedT& get()
        {
            std::call_once(initialized, initialize);
            return get_unsafe();
        }

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    private:
        static void initialize()
        {
            get_unsafe();
        }

        static DerivedT& get_unsafe()
        {
            static DerivedT instance;
            return instance;
        }

        static std::once_flag initialized;
    };

    template <typename DerivedT>
    std::once_flag Singleton<DerivedT>::initialized;
}
