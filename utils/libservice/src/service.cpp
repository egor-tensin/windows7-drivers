/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "libservice/common.hpp"
#include "libservice/service.hpp"
#include "libservice/service_handle.hpp"
#include "libservice/service_manager.hpp"
#include "libservice/windows_error.hpp"

#include <Windows.h>

#include <string>
#include <utility>
#include <vector>

namespace libservice
{
    namespace
    {
        ServiceHandle open_service(const ServiceManager& mgr, const std::string& name)
        {
            const auto raw = OpenServiceA(
                static_cast<SC_HANDLE>(mgr),
                name.c_str(),
                SERVICE_ALL_ACCESS);

            if (NULL == raw)
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return ServiceHandle(raw);
        }

        ServiceHandle install_service(
            const ServiceManager& mgr,
            const std::string& name,
            const std::string& bin_path)
        {
            const auto raw = CreateServiceA(
                static_cast<SC_HANDLE>(mgr),
                name.c_str(),
                name.c_str(),
                SERVICE_ALL_ACCESS,
                SERVICE_KERNEL_DRIVER,
                SERVICE_DEMAND_START,
                SERVICE_ERROR_NORMAL,
                bin_path.c_str(),
                NULL,
                NULL,
                NULL,
                NULL,
                NULL);

            if (NULL == raw)
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return ServiceHandle(raw);
        }

        void start_service(const ServiceHandle& handle)
        {
            if (!StartService(static_cast<SC_HANDLE>(handle), 0, NULL))
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        void stop_service(const ServiceHandle& handle)
        {
            SERVICE_STATUS service_status;

            if (!ControlService(static_cast<SC_HANDLE>(handle), SERVICE_CONTROL_STOP, &service_status))
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        void uninstall_service(const ServiceHandle& handle)
        {
            if (!DeleteService(static_cast<SC_HANDLE>(handle)))
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        bool does_service_exist(const ServiceManager& mgr,
                                const std::string& name)
        {
            const auto raw = OpenService(static_cast<SC_HANDLE>(mgr),
                                         name.c_str(),
                                         SERVICE_QUERY_STATUS);

            if (NULL != raw)
            {
                ServiceHandle handle(raw);
                return true;
            }

            const auto ec = GetLastError();

            switch (ec)
            {
                case ERROR_SERVICE_DOES_NOT_EXIST:
                    return false;

                default:
                    throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }
        
        SERVICE_STATUS_PROCESS query_service_status(const ServiceHandle& handle)
        {
            SERVICE_STATUS_PROCESS status;
            DWORD nbreq;

            if (!QueryServiceStatusEx(static_cast<SC_HANDLE>(handle),
                                      SC_STATUS_PROCESS_INFO,
                                      reinterpret_cast<BYTE*>(&status),
                                      sizeof(status),
                                      &nbreq))
            {
                const auto ec = GetLastError();
                throw std::system_error(ec, WinErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return status;
        }

        DWORD query_service_state(const ServiceHandle& handle)
        {
            return query_service_status(handle).dwCurrentState;
        }
        
        SERVICE_STATUS_PROCESS wait_for_service_state(
            const ServiceHandle& handle,
            const DWORD desired_state)
        {
            auto status = query_service_status(handle);

            DWORD old_timestamp = GetTickCount();

            DWORD old_check_point = status.dwCheckPoint;
            DWORD old_wait_hint = status.dwWaitHint;

            while (desired_state != status.dwCurrentState)
            {
                DWORD wait_time = old_wait_hint / 10;

                if (wait_time < 1000)
                    wait_time = 1000;
                else if (wait_time > 10000)
                    wait_time = 10000;

                Sleep(wait_time);

                status = query_service_status(handle);

                if (desired_state == status.dwCurrentState)
                    break;

                if (status.dwCheckPoint > old_check_point)
                {
                    old_timestamp = GetTickCount();

                    old_check_point = status.dwCheckPoint;
                    old_wait_hint = status.dwWaitHint;
                }
                else if (GetTickCount() - old_timestamp > old_wait_hint)
                {
                    return status;
                }
            }
            return status;
        }
    }

    Service Service::open(const ServiceManager& mgr, const std::string& name)
    {
        return Service(open_service(mgr, name));
    }

    Service Service::install(const ServiceManager& mgr,
                             const std::string& name,
                             const std::string& bin_path)
    {
        return Service(install_service(mgr, name, bin_path));
    }

    bool Service::does_exist(const ServiceManager& mgr,
                                     const std::string& name)
    {
        return does_service_exist(mgr, name);
    }

    void Service::start() const
    {
        const auto state = query_service_state(m_handle);

        switch (state)
        {
            case SERVICE_STOPPED:
                break;

            case SERVICE_STOP_PENDING:
                wait_for_service_state(m_handle, SERVICE_STOPPED);
                break;

            default:
                return;
        }

        start_service(m_handle);
        wait_for_service_state(m_handle, SERVICE_RUNNING);
    }

    void Service::stop() const
    {
        switch (query_service_state(m_handle))
        {
            case SERVICE_STOPPED:
                return;

            case SERVICE_STOP_PENDING:
                wait_for_service_state(m_handle, SERVICE_STOPPED);
                return;
        }

        stop_service(m_handle);
        wait_for_service_state(m_handle, SERVICE_STOPPED);
    }

    void Service::uninstall() const
    {
        stop();
        uninstall_service(m_handle);
    }

    void swap(Service& a, Service& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}

namespace std
{
    template <>
    void swap<libservice::Service>(
        libservice::Service& a,
        libservice::Service& b) LIBSERVICE_NOEXCEPT
    {
        a.swap(b);
    }
}
