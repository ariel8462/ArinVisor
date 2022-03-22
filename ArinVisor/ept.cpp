#include <ntddk.h>

#include "ept.h"
#include "memory.h"
#include "utils.h"

Ept::Ept(VirtualCpu* vcpu) noexcept
	: vcpu_(vcpu)
{
	setup_ept();
	InitializeListHead(&list_head_);
}

Ept::~Ept()
{
	free_pte_list();
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

auto Ept::split_page(arch::EptLargePde* pde_to_split) noexcept -> arch::EptPte*
{
	if (!pde_to_split->bits.large_page)
	{
		KdPrint(("[-] Tried splitting a non-large page?\n"));
		return nullptr;
	}

	auto pte = new (NonPagedPool, kTag) arch::EptPte[kPageEntryCount];

	if (!pte)
	{
		KdPrint(("[-] Pte allocation failed :(\n"));
		return nullptr;
	}

	RtlSecureZeroMemory(pte, sizeof(arch::EptPte));
	pde_to_split->bits.large_page = false;

	for (int i = 0; i < kPageEntryCount; i++)
	{
		pte[i].bits.write_access = pde_to_split->bits.write_access;
		pte[i].bits.read_access = pde_to_split->bits.read_access;
		pte[i].bits.execute_access = pde_to_split->bits.execute_access;
		pte[i].bits.memory_type = pde_to_split->bits.memory_type;
		pte[i].bits.pfn = (pde_to_split->bits.pfn << 21) / PAGE_SIZE + i;
		pte[i].bits.accessed = pde_to_split->bits.accessed;
		pte[i].bits.dirty = pde_to_split->bits.dirty;
		pte[i].bits.ignore_pat = pde_to_split->bits.ignore_pat;
		pte[i].bits.execute_access_from_user_mode = pde_to_split->bits.execute_access_from_user_mode;
	}

	arch::EptPde pde = { 0 };

	pde.bits.write_access = pde_to_split->bits.write_access;
	pde.bits.read_access = pde_to_split->bits.read_access;
	pde.bits.execute_access = pde_to_split->bits.execute_access;
	pde.bits.pfn = MmGetPhysicalAddress(pte).QuadPart / PAGE_SIZE;
	pde.bits.accessed = pde_to_split->bits.accessed;
	pde.bits.execute_access_from_user_mode = pde_to_split->bits.execute_access_from_user_mode;

	*reinterpret_cast<arch::EptPde*>(pde_to_split) = pde;

	PteEntry* pte_entry = new (NonPagedPool, kTag) PteEntry;
	pte_entry->pte = pte;

	InsertTailList(&list_head_, &pte_entry->entry);

	KeIpiGenericCall(utils::invept, 0);
	
	return pte;
}

void Ept::free_pte_list() noexcept
{
	while (!IsListEmpty(&list_head_))
	{
		auto entry = RemoveTailList(&list_head_);
		auto current_struct = CONTAINING_RECORD(entry, PteEntry, entry);

		delete current_struct->pte;
		delete current_struct;
	}
}
