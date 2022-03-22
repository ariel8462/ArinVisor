#pragma once

#include "arch.h"
#include "vcpu.h"

struct PteEntry
{
	LIST_ENTRY entry;
	arch::EptPte* pte;
};

class Ept
{
public:
	Ept(VirtualCpu* vcpu) noexcept;
	~Ept();

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

	void free_pte_list() noexcept;

private:
	VirtualCpu* vcpu_;
	LIST_ENTRY list_head_;
};
