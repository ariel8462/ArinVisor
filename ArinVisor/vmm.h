#pragma once

#include "arch.h"

struct VirtualCpu
{
	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;
};
