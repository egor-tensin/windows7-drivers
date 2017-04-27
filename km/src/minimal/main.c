/*
 * Copyright (c) 2017 Egor Tensin <Egor.Tensin@gmail.com>
 * This file is part of the "Windows 7 drivers" project.
 * For details, see https://github.com/egor-tensin/windows7-drivers.
 * Distributed under the MIT License.
 */

#include <ntddk.h>

static void on_driver_unload(DRIVER_OBJECT *driver_object)
{
    UNREFERENCED_PARAMETER(driver_object);
}

NTSTATUS DriverEntry(
    DRIVER_OBJECT *driver_object,
    UNICODE_STRING *registry_path)
{
    UNREFERENCED_PARAMETER(registry_path);

    driver_object->DriverUnload = on_driver_unload;
    return STATUS_SUCCESS;
}
