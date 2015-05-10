/**
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include <mutex>

namespace libservice
{
    template <typename DerivedType>
    class Singleton
    {
    public:
        static DerivedType& get()
        {
            std::call_once(is_initialized, initialize);
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

        static DerivedType& get_unsafe()
        {
            static DerivedType instance;
            return instance;
        }

        static std::once_flag is_initialized;
    };

    template <typename DerivedType>
    std::once_flag Singleton<DerivedType>::is_initialized;
}
