#pragma once

#include <ntddk.h>

#include "arch.h"

struct VirtualCpu
{
	unsigned long processor_number;

	void* msr_bitmap;

	//temp, remove later and implement some other solution
	union
	{
		CONTEXT guest_context;
		unsigned char stack[0x6000];
	};

	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;
};
