/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include <ntddk.h>

NTSTATUS set_up_devices(DRIVER_OBJECT *);
void destroy_devices();
