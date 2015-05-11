/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

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

        explicit Handle(HANDLE raw)
            : m_impl(raw)
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

        explicit operator bool() const
        {
            return static_cast<bool>(m_impl);
        }

        explicit operator HANDLE() const
        {
            return m_impl.get();
        }

        void swap(Handle& other) LIBSERVICE_NOEXCEPT
        {
            using std::swap;
            swap(m_impl, other.m_impl);
        }

    private:
        struct Deleter
        {
            void operator()(HANDLE raw)
            {
                ::CloseHandle(raw);
            }
        };

        std::unique_ptr<std::remove_pointer<HANDLE>::type, Deleter> m_impl;

        Handle(const Handle&) = delete;
    };

    void swap(Handle& a, Handle& b) LIBSERVICE_NOEXCEPT;
}

namespace std
{
    template <>
    void swap<libservice::Handle>(
        libservice::Handle& a,
        libservice::Handle& b) LIBSERVICE_NOEXCEPT;
}
