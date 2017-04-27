// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

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
