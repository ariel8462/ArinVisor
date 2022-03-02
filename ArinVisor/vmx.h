#pragma once

#include "arch.h"
#include "vmm.h"

namespace vmx
{
	void enable_vmx();
	bool init_vmxon(VirtualCpu* vcpu);
	auto allocate_vmxon_region() -> arch::VmmRegions*;
	void vmxoff();
}
