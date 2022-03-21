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
	//to do - ept hook
	auto split_page(arch::EptLargePde* pde) noexcept -> arch::EptPte*;
	
private:
	void setup_ept() noexcept;

	void setup_pml4() noexcept;
	void setup_pdpt() noexcept;
	void setup_pdt() noexcept;

private:
	VirtualCpu* vcpu_;
};
