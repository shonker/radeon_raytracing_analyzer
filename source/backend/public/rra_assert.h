//=============================================================================
// Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Definition of assert.
//=============================================================================

#ifndef RRA_BACKEND_PUBLIC_RRA_ASSERT_H_
#define RRA_BACKEND_PUBLIC_RRA_ASSERT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "public/rra_macro.h"

#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus

#ifdef _DEBUG
#ifdef _WIN32

#ifdef DISABLE_RRA_DEBUG_BREAK
#define RRA_DEBUG_BREAK \
    {                   \
    }
#else
/// Macro to force the debugger to break at this point in the code.
#define RRA_DEBUG_BREAK __debugbreak();
#endif
#else
#define RRA_DEBUG_BREAK \
    {                   \
    }
#endif
#else
// don't allow debug break in release builds.
#define RRA_DEBUG_BREAK
#endif

/// A typedef for the callback function for assert printing.
///
/// This can be used to re-route printing of assert messages from the RRA backend
/// to another destination. For example instead of the default behaviour of printing
/// the assert messages to the debugger's TTY the message can be re-routed to a
/// MessageBox in a GUI application.
///
/// @param [in] message                 The message generated by the assert.
///
typedef void (*RraAssertCallback)(const char* message);

/// Function to report an assert.
///
/// @param [in] file                    The name of the file as a string.
/// @param [in] line                    The index of the line in the file.
/// @param [in] condition               The boolean condition that was tested.
/// @param [in] message                 The optional message to print.
///
/// @returns
/// Always returns true.
///
bool RraAssertReport(const char* file, int32_t line, const char* condition, const char* msg);

/// Provides the ability to set a callback for assert messages.
///
/// @param [in] callback                The callback function that will receive assert messages.
///
void RraAssertSetPrintingCallback(RraAssertCallback callback);

#if _DEBUG
/// Standard assert macro.
#define RRA_ASSERT(condition)                                                      \
    do                                                                             \
    {                                                                              \
        if (!(condition) && RraAssertReport(__FILE__, __LINE__, #condition, NULL)) \
            RRA_DEBUG_BREAK                                                        \
    } while (0)

/// Assert macro with message.
#define RRA_ASSERT_MESSAGE(condition, msg)                                        \
    do                                                                            \
    {                                                                             \
        if (!(condition) && RraAssertReport(__FILE__, __LINE__, #condition, msg)) \
            RRA_DEBUG_BREAK                                                       \
    } while (0)

/// Assert macro that always fails.
#define RRA_ASSERT_FAIL(message)                            \
    do                                                      \
    {                                                       \
        RraAssertReport(__FILE__, __LINE__, NULL, message); \
        RRA_DEBUG_BREAK                                     \
    } while (0)
#else
// asserts disabled
#define RRA_ASSERT(condition)  \
    do                         \
    {                          \
        RRA_UNUSED(condition); \
    } while (0)

#define RRA_ASSERT_MESSAGE(condition, message) \
    do                                         \
    {                                          \
        RRA_UNUSED(condition);                 \
        RRA_UNUSED(message);                   \
    } while (0)

#define RRA_ASSERT_FAIL(message) \
    do                           \
    {                            \
        RRA_UNUSED(message);     \
    } while (0)
#endif  // #if _DEBUG

/// Simple static assert.
#define RRA_STATIC_ASSERT(condition) static_assert(condition, #condition)

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus
#endif  // #ifndef RRA_BACKEND_PUBLIC_RRA_ASSERT_H_
