#pragma once

#include "arch.h"
#include "exit_handler.h"

struct VmmContext
{
	unsigned long processor_count;
	void* processors_vcpu;
	ExitHandler exit_handler;
};

extern VmmContext* vmm_context;
