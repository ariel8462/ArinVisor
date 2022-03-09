#pragma once

#include <ntddk.h>

#include "arch.h"

constexpr unsigned long kStackSize = 0x6000;

struct VirtualCpu
{
	unsigned long processor_number;

	union
	{
		CONTEXT guest_context;
		unsigned char stack[kStackSize];
	};

	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;
	void* msr_bitmap;
};
