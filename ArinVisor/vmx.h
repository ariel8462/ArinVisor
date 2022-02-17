#pragma once

#include "arch.h"
#include "vmm.h"

namespace vmx
{
	void enable_vmx();
	auto allocate_vcpu()->VirtualCpu*;
	auto init_vmxon(VirtualCpu* vcpu) -> void*;
}