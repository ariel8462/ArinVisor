#pragma once

#include "vmm.h"

namespace utils
{
	bool is_intel_cpu();
	bool is_vmx_supported();
	void free_memory(VmmContext* vmm_context);
	bool is_hypervisor_present();
}
