/**
 * \file
 * \author Egor Tensin <Egor.Tensin@gmail.com>
 * \date 2015
 * \copyright This file is licensed under the terms of the MIT License.
 *            See LICENSE.txt for details.
 */

#pragma once

#define LIBSERVICE_FILE_PATH __FILE__
#define LIBSERVICE_LINE_NUMBER __LINE__
#define LIBSERVICE_FUNCTION_NAME __FUNCTION__

#define LIBSERVICE_TO_STRING(s) LIBSERVICE_TO_STRING_(s)
#define LIBSERVICE_TO_STRING_(s) #s

#define LIBSERVICE_LINE_NUMBER_STRING LIBSERVICE_TO_STRING(LIBSERVICE_LINE_NUMBER)

#define LIBSERVICE_NOEXCEPT throw()
