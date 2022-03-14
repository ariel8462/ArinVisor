#pragma once

#include "arch.h"
#include "exit_handler.h"

struct VmmContext
{
	unsigned long processor_count;
	VirtualCpu* processors_vcpu[8];
	ExitHandler exit_handler;
};

extern VmmContext* vmm_context;
