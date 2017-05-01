// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#include "libservice/all.hpp"

#include <Windows.h>

#include <string>
#include <system_error>

namespace libservice
{
    namespace
    {
        ServiceHandle open_service(
            const ServiceManager& mgr,
            const std::string& name)
        {
            const auto raw = OpenServiceA(
                mgr,
                name.c_str(),
                SERVICE_ALL_ACCESS);

            if (NULL == raw)
            {
                const auto ec = GetLastError();
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return raw;
        }

        ServiceHandle install_service(
            const ServiceManager& mgr,
            const std::string& name,
            const std::string& bin_path)
        {
            const auto raw = CreateServiceA(
                mgr,
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
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }

            return raw;
        }

        void start_service(const ServiceHandle& handle)
        {
            if (!StartService(handle, 0, NULL))
            {
                const auto ec = GetLastError();
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        void stop_service(const ServiceHandle& handle)
        {
            SERVICE_STATUS service_status;

            if (!ControlService(handle, SERVICE_CONTROL_STOP, &service_status))
            {
                const auto ec = GetLastError();
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        void uninstall_service(const ServiceHandle& handle)
        {
            if (!DeleteService(handle))
            {
                const auto ec = GetLastError();
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        bool service_exists(
            const ServiceManager& mgr,
            const std::string& name)
        {
            const auto raw = OpenServiceA(
                mgr,
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
                    throw std::system_error(
                        ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
            }
        }

        SERVICE_STATUS_PROCESS query_service_status(const ServiceHandle& handle)
        {
            SERVICE_STATUS_PROCESS status;
            DWORD nbreq;

            const auto buf_ptr = reinterpret_cast<BYTE*>(&status);
            const auto buf_size = sizeof(status);

            if (!QueryServiceStatusEx(
                handle, SC_STATUS_PROCESS_INFO, buf_ptr, buf_size, &nbreq))
            {
                const auto ec = GetLastError();
                throw std::system_error(
                    ec, WindowsErrorCategory::get(), LIBSERVICE_ERROR_PREFIX);
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

    Service Service::open(
        const ServiceManager& mgr,
        const std::string& name)
    {
        return open_service(mgr, name);
    }

    Service Service::install(
        const ServiceManager& mgr,
        const std::string& name,
        const std::string& bin_path)
    {
        return install_service(mgr, name, bin_path);
    }

    bool Service::exists(
        const ServiceManager& mgr,
        const std::string& name)
    {
        return service_exists(mgr, name);
    }

    void Service::start() const
    {
        const auto state = query_service_state(handle);

        switch (state)
        {
            case SERVICE_STOPPED:
                break;

            case SERVICE_STOP_PENDING:
                wait_for_service_state(handle, SERVICE_STOPPED);
                break;

            default:
                return;
        }

        start_service(handle);
        wait_for_service_state(handle, SERVICE_RUNNING);
    }

    void Service::stop() const
    {
        switch (query_service_state(handle))
        {
            case SERVICE_STOPPED:
                return;

            case SERVICE_STOP_PENDING:
                wait_for_service_state(handle, SERVICE_STOPPED);
                return;
        }

        stop_service(handle);
        wait_for_service_state(handle, SERVICE_STOPPED);
    }

    void Service::uninstall() const
    {
        stop();
        uninstall_service(handle);
    }
}
