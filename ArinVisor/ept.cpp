#include <ntddk.h>

#include "ept.h"

Ept::Ept(VirtualCpu* vcpu) noexcept
	: vcpu_(vcpu)
{
	setup_ept();
}

void Ept::setup_ept() noexcept
{
	setup_pml4();
	setup_pdpt();
	setup_pdt();
}

void Ept::setup_pml4() noexcept
{
	vcpu_->paging_structs.pml4[0].bits.write_access = true;
	vcpu_->paging_structs.pml4[0].bits.read_access = true;
	vcpu_->paging_structs.pml4[0].bits.execute_access = true;
	vcpu_->paging_structs.pml4[0].bits.pfn = MmGetPhysicalAddress(&vcpu_->paging_structs.pdpt).QuadPart / PAGE_SIZE;
}

void Ept::setup_pdpt() noexcept
{
	for (int i = 0; i < kPageEntryCount; i++)
	{
		vcpu_->paging_structs.pdpt[i].bits.write_access = true;
		vcpu_->paging_structs.pdpt[i].bits.read_access = true;
		vcpu_->paging_structs.pdpt[i].bits.execute_access = true;
		vcpu_->paging_structs.pdpt[i].bits.pfn = MmGetPhysicalAddress(&vcpu_->paging_structs.pdt[i]).QuadPart / PAGE_SIZE;
	}
}

void Ept::setup_pdt() noexcept
{
	for (int pdpt_entry = 0; pdpt_entry < kPageEntryCount; pdpt_entry++)
	{
		for (int pd_entry = 0; pd_entry < kPageEntryCount; pd_entry++)
		{
			vcpu_->paging_structs.pdt[pdpt_entry][pd_entry].bits.write_access = true;
			vcpu_->paging_structs.pdt[pdpt_entry][pd_entry].bits.read_access = true;
			vcpu_->paging_structs.pdt[pdpt_entry][pd_entry].bits.execute_access = true;
			vcpu_->paging_structs.pdt[pdpt_entry][pd_entry].bits.memory_type = 6;  // write-back
			vcpu_->paging_structs.pdt[pdpt_entry][pd_entry].bits.large_page = true;
			vcpu_->paging_structs.pdt[pdpt_entry][pd_entry].bits.pfn = static_cast<unsigned long long>(
				pdpt_entry) * kPageEntryCount + pd_entry;
		}
	}
}
