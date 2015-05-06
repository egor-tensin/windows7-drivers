/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#include <ntddk.h>

#define CONVERT_NT_PATH CTL_CODE(0x8000, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
