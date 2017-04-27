// Copyright (c) 2015 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Windows 7 drivers" project.
// For details, see https://github.com/egor-tensin/windows7-drivers.
// Distributed under the MIT License.

#pragma once

#define LIBSERVICE_FILE_PATH __FILE__
#define LIBSERVICE_LINE_NUMBER __LINE__
#define LIBSERVICE_FUNCTION_NAME __FUNCTION__

#define LIBSERVICE_TO_STRING(s) LIBSERVICE_TO_STRING_(s)
#define LIBSERVICE_TO_STRING_(s) #s

#define LIBSERVICE_LINE_NUMBER_STRING LIBSERVICE_TO_STRING(LIBSERVICE_LINE_NUMBER)

#define LIBSERVICE_NOEXCEPT throw()
