/*
 * Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
 * This file is part of the "Windows 7 drivers" project.
 * For details, see https://github.com/egor-tensin/windows7-drivers.
 * Distributed under the MIT License.
 */

#include "device.h"

#include <ntddk.h>

static NTSTATUS device_open(DEVICE_OBJECT *device_object, IRP *irp)
{
    NTSTATUS status = STATUS_SUCCESS;

    irp->IoStatus.Status = status;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

typedef NTSTATUS (*ioctl_handler)(
    void *, unsigned long,
    void *, unsigned long,
    ULONG_PTR *);

static NTSTATUS handle_say_hello(
    void *in_buf,
    unsigned long in_buf_size,
    void *out_buf,
    unsigned long out_buf_size,
    ULONG_PTR *nbwritten)
{
    DbgPrint("Hello, world!\n");
    return STATUS_SUCCESS;
}

static unsigned int i = 42;

static NTSTATUS handle_exchange_ints(
    void *in_buf,
    unsigned long in_buf_size,
    void *out_buf,
    unsigned long out_buf_size,
    ULONG_PTR *nbwritten)
{
    unsigned int temp_i;

    if (in_buf_size != sizeof(temp_i))
        return STATUS_INVALID_BUFFER_SIZE;

    RtlCopyMemory(&temp_i, in_buf, in_buf_size);
    DbgPrint("%08x\n", temp_i);

    if (out_buf_size < sizeof(i))
        return STATUS_BUFFER_TOO_SMALL;

    RtlCopyMemory(out_buf, &i, sizeof(i));
    *nbwritten += sizeof(i);
    i = temp_i;
    return STATUS_SUCCESS;
}

#define SAY_HELLO CTL_CODE(0x8000, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define EXCHANGE_INTS CTL_CODE(0x8001, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

static NTSTATUS device_ioctl(DEVICE_OBJECT *device_object, IRP *irp)
{
    IO_STACK_LOCATION *io_stack_loc;
    void* in_buf, *out_buf;
    unsigned long in_buf_size, out_buf_size;
    ioctl_handler handler;
    NTSTATUS status = STATUS_UNSUCCESSFUL;

    irp->IoStatus.Status = status;
    irp->IoStatus.Information = 0;
    io_stack_loc = IoGetCurrentIrpStackLocation(irp);

    in_buf = out_buf = irp->AssociatedIrp.SystemBuffer;
    in_buf_size = io_stack_loc->Parameters.DeviceIoControl.InputBufferLength;
    out_buf_size = io_stack_loc->Parameters.DeviceIoControl.OutputBufferLength;

    switch (io_stack_loc->Parameters.DeviceIoControl.IoControlCode)
    {
        case SAY_HELLO:
            handler = handle_say_hello;
            break;

        case EXCHANGE_INTS:
            handler = handle_exchange_ints;
            break;

        default:
            status = irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
            goto complete_request;
    }

    status = irp->IoStatus.Status = handler(
        in_buf, in_buf_size,
        out_buf, out_buf_size,
        &irp->IoStatus.Information);

complete_request:
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    return status;
}

typedef struct
{
    const wchar_t *path;
    const wchar_t *symlink;
}
DeviceInfo;

typedef struct
{
    DEVICE_OBJECT *object;
    UNICODE_STRING path;
    UNICODE_STRING symlink;
}
Device;

#define NUMOF_DEVICES 2

static DeviceInfo devices_info[NUMOF_DEVICES] =
{
    {
        L"\\Device\\test_device1",
        L"\\DosDevices\\test_device1",
    },
    {
        L"\\Device\\test_device2",
        L"\\DosDevices\\test_device2",
    },
};

static Device devices[NUMOF_DEVICES];

static void destroy_device(int i)
{
    IoDeleteSymbolicLink(&devices[i].symlink);
    IoDeleteDevice(devices[i].object);
}

void destroy_devices()
{
    int i;
    for (i = 0; i < NUMOF_DEVICES; ++i)
        destroy_device(i);
}

static NTSTATUS set_up_device(DRIVER_OBJECT *driver_object, int i)
{
    NTSTATUS status = STATUS_SUCCESS;

    DbgPrint("Setting up device...\n");
    DbgPrint("\tPath: %ws\n", devices_info[i].path);
    DbgPrint("\tSymlink: %ws\n", devices_info[i].symlink);

    RtlInitUnicodeString(&devices[i].path, devices_info[i].path);
    RtlInitUnicodeString(&devices[i].symlink, devices_info[i].symlink);

    status = IoCreateDevice(
        driver_object,
        0,
        &devices[i].path,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &devices[i].object);

    if (!NT_SUCCESS(status))
        return status;

    devices[i].object->Flags |= DO_BUFFERED_IO;
    devices[i].object->Flags &= ~DO_DEVICE_INITIALIZING;

    if (!NT_SUCCESS(status = IoCreateSymbolicLink(
            &devices[i].symlink, &devices[i].path)))
        goto delete_device;

    return status;

delete_device:
    IoDeleteDevice(devices[i].object);

    return status;
}

NTSTATUS set_up_devices(DRIVER_OBJECT *driver_object)
{
    int i, j;
    NTSTATUS status = STATUS_SUCCESS;

    for (i = 0; i < NUMOF_DEVICES; ++i)
        if (!NT_SUCCESS(status = set_up_device(driver_object, i)))
            goto destroy_devices;

    driver_object->MajorFunction[IRP_MJ_CREATE] = device_open;
    driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = device_ioctl;

    return status;

destroy_devices:
    for (j = 0; j < i; ++j)
        destroy_device(j);

    return status;
}
