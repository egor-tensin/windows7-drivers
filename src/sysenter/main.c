/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#include <ntddk.h>

static __int64 old_msr_value = 0;
static void *old_ki_fast_call_entry = NULL;

static void __stdcall log_system_call()
{
    static LONG count = 0;
    static const LONG throttle = 10000;

    LONG n = InterlockedIncrement(&count);

    if (n % throttle == 0)
        DbgPrint("Another %ld of `sysenter`s (eax=)!\n", throttle);
}

static void __declspec(naked) new_ki_fast_call_entry()
{
    __asm
    {
        pushad
        pushfd
        mov ecx, 0x23
        push 0x30
        pop fs
        mov ds, cx
        mov es, cx
        call log_system_call
        popfd
        popad
        jmp [old_ki_fast_call_entry]
    }
}

#define IA32_SYSENTER_EIP 0x176

static void hook_sysenter()
{
    /*
    __asm
    {
        mov ecx, IA32_SYSENTER_EIP
        rdmsr
        mov old_ki_fast_call_entry, eax
        mov eax, new_ki_fast_call_entry
        xor edx, edx
        wrmsr
    }
    */

    old_msr_value = __readmsr(IA32_SYSENTER_EIP);
    old_ki_fast_call_entry = (void *) old_msr_value;
    __writemsr(IA32_SYSENTER_EIP, new_ki_fast_call_entry);
}

static void unhook_sysenter()
{
    /*
    __asm
    {
        mov ecx, IA32_SYSENTER_EIP
        mov eax, old_ki_fast_call_entry
        xor edx, edx
        wrmsr
    }
    */

    __writemsr(IA32_SYSENTER_EIP, old_msr_value);
}

static void on_driver_unload(DRIVER_OBJECT *driver_object)
{
    KTIMER timer;
    LARGE_INTEGER time_out;

    unhook_sysenter();

    KeInitializeTimer(&timer);
    time_out.QuadPart = -30000000; // 3 sec
    KeSetTimer(&timer, time_out, NULL);

    KeWaitForSingleObject(&timer, Executive, KernelMode, FALSE, NULL);
}

NTSTATUS DriverEntry(DRIVER_OBJECT *driver_object,
                     UNICODE_STRING *registry_path)
{
    driver_object->DriverUnload = on_driver_unload;
    hook_sysenter();
    return STATUS_SUCCESS;
}
