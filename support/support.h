#pragma once

#define PHNT_VERSION PHNT_WIN11
#include <phnt_windows.h>
#include <phnt.h>

// Assertion
#define ASSERT(cond)                                                              \
		do {                                                                      \
			if (!cond)                                                            \
			{                                                                     \
				DbgPrint("Assertion failed: " #cond "\n");                        \
				NtTerminateProcess(NtCurrentProcess(), STATUS_ASSERTION_FAILURE); \
			}                                                                     \
		} while (0);


// Number of elements in array
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
