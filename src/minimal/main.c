#include <ntddk.h>

static void on_driver_unload(DRIVER_OBJECT *driver_object)
{ }

NTSTATUS DriverEntry(
    DRIVER_OBJECT *driver_object,
    UNICODE_STRING *registry_path)
{
    driver_object->DriverUnload = on_driver_unload;
    return STATUS_SUCCESS;
}
