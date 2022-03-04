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

//remove later, place in some other place, maybe registers should be in opposite order
#pragma pack(push, 1)
struct guest_state_vmx
{
	unsigned long long rsp;
	unsigned long long rbp;
	unsigned long long rax;
	unsigned long long rbx;
	unsigned long long rcx;
	unsigned long long rdx;
	unsigned long long rsi;
	unsigned long long rdi;
	unsigned long long r8;
	unsigned long long r9;
	unsigned long long r10;
	unsigned long long r11;
	unsigned long long r12;
	unsigned long long r13;
	unsigned long long r14;
	unsigned long long r15;
};
#pragma pack(pop)