#pragma once

#include "arch.h"

struct VirtualCpu
{
	unsigned long processor_number;

	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;

	void* msr_bitmap;

	//temp, remove later and implement some other solution
	union
	{
		CONTEXT guest_context;
		unsigned char stack[0x6000];
	};
};

struct VmmContext
{
	unsigned long processor_count;
	void* processors_vcpu;
};

extern VmmContext* vmm_context;
