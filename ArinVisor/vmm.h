#pragma once

#include "arch.h"

struct VirtualCpu
{
	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;
	unsigned long processor_number;
};

extern VirtualCpu* vcpu;
