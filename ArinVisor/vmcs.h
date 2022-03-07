#pragma once

#include "vcpu.h"

namespace vmcs
{
	bool setup_vmcs(VirtualCpu*& vcpu);
	auto allocate_vmcs_region() -> arch::VmmRegions*;
}
