#pragma once

#include "arch.h"

struct VirtualCpu
{
	unsigned long processor_number;
	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;
};

struct VmmContext
{
	unsigned long processor_count;
	VirtualCpu* processors_vcpu[6];
};

static auto allocate_vmm_context() -> VmmContext*
{
	VmmContext* vmm_context = reinterpret_cast<VmmContext*>(
		ExAllocatePoolWithTag(PagedPool, sizeof(VmmContext), kTag)
		);

	return (vmm_context != nullptr ? vmm_context : nullptr);
}

extern VmmContext* vmm_context;
