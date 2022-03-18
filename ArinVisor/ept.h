#pragma once

#include "arch.h"
#include "vcpu.h"

class Ept
{
public:
	Ept(VirtualCpu* vcpu) noexcept;

	Ept(const Ept&) = delete;
	Ept& operator=(const Ept&) = delete;

public:
	//split ept, hook ept, etc - to do

private:
	void setup_ept() noexcept;

private:
	VirtualCpu* vcpu_;
};
