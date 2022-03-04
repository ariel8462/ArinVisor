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
	VirtualCpu* processors_vcpu[8]; //to do - change later from constant to number of active processors
};

extern VmmContext* vmm_context;
