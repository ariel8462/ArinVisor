#pragma once

#include "vmm.h"

void free_memory(VirtualCpu* vcpu);

namespace load
{
	bool load_hypervisor(VirtualCpu*& vcpu);
}
