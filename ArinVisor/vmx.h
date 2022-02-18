#pragma once

#include "arch.h"
#include "vmm.h"

namespace vmx
{
	void enable_vmx();
	auto allocate_vcpu() -> VirtualCpu*;
	bool init_vmxon(VirtualCpu* vcpu);
	auto allocate_vmxon_region() -> arch::VmmRegions*;
	auto allocate_vmcs_region() -> arch::VmmRegions*;
}
