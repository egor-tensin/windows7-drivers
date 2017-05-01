// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#include "common.hpp"

#include <Windows.h>

#include <memory>
#include <type_traits>
#include <utility>

namespace libservice
{
    class Handle
    {
    public:
        Handle() = default;

        Handle(HANDLE raw)
            : impl(raw)
        { }

        Handle(Handle&& other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
        }

        Handle& operator=(Handle other) LIBSERVICE_NOEXCEPT
        {
            swap(other);
            return *this;
        }

        operator bool() const
        {
            return static_cast<bool>(impl);
        }

        operator HANDLE() const
        {
            return impl.get();
        }

        void swap(Handle& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(impl, other.impl);
        }

    private:
        struct Deleter
        {
            void operator()(HANDLE raw)
            {
                CloseHandle(raw);
            }
        };

        std::unique_ptr<std::remove_pointer<HANDLE>::type, Deleter> impl;

        Handle(const Handle&) = delete;
    };

    inline void swap(Handle& a, Handle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    inline void swap<libservice::Handle>(
        libservice::Handle& a,
        libservice::Handle& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
