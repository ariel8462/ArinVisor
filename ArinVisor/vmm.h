#pragma once

#include "arch.h"
#include "exit_handler.h"
#include "vmx.h"
#include "vmcs.h"

//bad
constexpr unsigned long kProcessorCount = 8;

struct VmmContext
{
	unsigned long processor_count;
	VirtualCpu* processors_vcpu[kProcessorCount];
	ExitHandler exit_handler;

	//bad solution, create a module manager later
	Vmxon* vmxon[kProcessorCount];
	SetupVmcs* setup_vmcs[kProcessorCount];
};

extern VmmContext* vmm_context;
