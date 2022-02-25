#pragma once

#include "vmm.h"

constexpr unsigned long kTag = 'nira';

namespace utils
{
	bool is_intel_cpu();
	bool is_vmx_supported();
	void free_memory(VmmContext* vmm_context);
}
