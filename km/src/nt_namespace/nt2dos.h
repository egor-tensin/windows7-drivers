/*
 * Copyright (c) 2015 Egor Tensin <egor@tensin.name>
 * This file is part of the "Windows 7 drivers" project.
 * For details, see https://github.com/egor-tensin/windows7-drivers
 * Distributed under the MIT License.
 */

#pragma once

#include <ntstrsafe.h>

NTSTATUS nt2dos(UNICODE_STRING* resolved, UNICODE_STRING* unresolved);
