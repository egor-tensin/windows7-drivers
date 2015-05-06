/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include <ntifs.h>

#include "nt2dos.h"

static NTSTATUS get_object_name_info(
    void *object,
    OBJECT_NAME_INFORMATION **object_name_info)
{
    unsigned long nbneeded;
    NTSTATUS status = STATUS_SUCCESS;

    status = ObQueryNameString(object, NULL, 0, &nbneeded);
    if (status != STATUS_INFO_LENGTH_MISMATCH)
        return status;
    *object_name_info = ExAllocatePoolWithTag(PagedPool, nbneeded, '1l');
    if (*object_name_info == NULL)
        return STATUS_INSUFFICIENT_RESOURCES;
    status = ObQueryNameString(object, *object_name_info, nbneeded, &nbneeded);
    if (!NT_SUCCESS(status))
        goto free_object_name_info;
    return status;

free_object_name_info:
    ExFreePool(*object_name_info);

    return status;
}

NTSTATUS nt2dos(UNICODE_STRING *u_resolved, UNICODE_STRING *u_unresolved)
{
    IO_STATUS_BLOCK io_status_block;
    DEVICE_OBJECT *volume_object;
    FILE_OBJECT *file_object;
    HANDLE file;
    OBJECT_ATTRIBUTES file_attrs;
    OBJECT_NAME_INFORMATION *file_name_info, *volume_name_info;
    UNICODE_STRING u_dos_name;
    USHORT u_resolved_size;
    NTSTATUS status = STATUS_SUCCESS;

    InitializeObjectAttributes(&file_attrs,
                               u_unresolved,
                               OBJ_CASE_INSENSITIVE | OBJ_OPENIF,
                               NULL,
                               NULL);

    status = ZwCreateFile(&file,
                          FILE_READ_ATTRIBUTES,
                          &file_attrs,
                          &io_status_block,
                          NULL,
                          FILE_ATTRIBUTE_NORMAL,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          FILE_OPEN,
                          FILE_SYNCHRONOUS_IO_NONALERT,
                          NULL, 0);

    if (!NT_SUCCESS(status))
        return status;

    status = ObReferenceObjectByHandle(file,
                                       FILE_READ_ATTRIBUTES,
                                       *IoFileObjectType,
                                       KernelMode,
                                       &file_object,
                                       NULL);
    if (!NT_SUCCESS(status))
        goto close_file;

    status = get_object_name_info(file_object, &file_name_info);
    if (!NT_SUCCESS(status))
        goto close_file;

    if (file_object->Vpb == NULL)
    {
        u_resolved->Buffer = ExAllocatePoolWithTag(
            PagedPool, file_name_info->Name.Length, '1l');
        if (u_resolved->Buffer == NULL)
        {
            status = STATUS_INSUFFICIENT_RESOURCES;
            goto free_file_name_info;
        }

        RtlInitEmptyUnicodeString(u_resolved,
                                  u_resolved->Buffer,
                                  file_name_info->Name.Length);
        RtlCopyUnicodeString(u_resolved, &file_name_info->Name);
        goto free_file_name_info;
    }

    volume_object = file_object->Vpb->RealDevice;
    status = get_object_name_info(volume_object, &volume_name_info);

    if (!NT_SUCCESS(status))
        goto free_file_name_info;

    status = IoVolumeDeviceToDosName(volume_object, &u_dos_name);
    if (!NT_SUCCESS(status))
        goto free_volume_name_info;

    u_resolved_size = file_name_info->Name.Length
        - volume_name_info->Name.Length
        + u_dos_name.Length;

    u_resolved->Buffer = ExAllocatePoolWithTag(
        PagedPool, u_resolved_size, '1l');
    if (u_resolved->Buffer == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto free_dos_name;
    }

    RtlInitEmptyUnicodeString(u_resolved, u_resolved->Buffer, u_resolved_size);

    RtlCopyUnicodeString(u_resolved, &u_dos_name);
    file_name_info->Name.Buffer +=
        volume_name_info->Name.Length / sizeof(wchar_t);
    file_name_info->Name.Length -=
        volume_name_info->Name.Length;
    RtlUnicodeStringCat(u_resolved, &file_name_info->Name);

free_dos_name:
    ExFreePool(u_dos_name.Buffer);

free_volume_name_info:
    ExFreePool(volume_name_info);

free_file_name_info:
    ExFreePool(file_name_info);

close_file:
    ZwClose(file);

    return status;
}
