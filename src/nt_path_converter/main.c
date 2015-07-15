/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include "device.h"

#include <ntddk.h>

static void on_driver_unload(DRIVER_OBJECT *driver_object)
{
    destroy_devices();
}

NTSTATUS DriverEntry(
    DRIVER_OBJECT *driver_object,
    UNICODE_STRING *registry_path)
{
    driver_object->DriverUnload = on_driver_unload;
    return set_up_devices(driver_object);
}
