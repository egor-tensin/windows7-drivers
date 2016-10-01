/*
 * Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
 * This file is part of the "Windows 7 drivers" project.
 * For details, see https://github.com/egor-tensin/windows7-drivers.
 * Distributed under the MIT License.
 */

#include "device.h"

#include <ntddk.h>

VOID on_driver_unload(DRIVER_OBJECT *driver_object)
{
    DbgPrint("Unloading test driver...\n");
    destroy_devices();
}

NTSTATUS DriverEntry(
    DRIVER_OBJECT *driver_object,
    UNICODE_STRING *registry_path)
{
    DbgPrint("Loading test driver...\n");
    driver_object->DriverUnload = on_driver_unload;
    return set_up_devices(driver_object);
}
