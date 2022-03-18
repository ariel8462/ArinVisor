#pragma once

#include <ntddk.h>

#include "arch.h"

constexpr unsigned long kStackSize = 0x6000;
constexpr unsigned long kPageEntryCount = 0x200;

struct PagingStructures
{
	arch::EptPml4e pml4[kPageEntryCount];

	//only 512 because of single pml4 entry (if pml4 was fully filled, size was 512*512)
	arch::EptPdpte pdpt[kPageEntryCount];

	// 512 pdpt entries, each entry has 512 pdt's, 512*512
	arch::EptLargePde pdt[kPageEntryCount][kPageEntryCount];
};

struct VirtualCpu
{
	unsigned long processor_number;

	union
	{
		CONTEXT guest_context;
		unsigned char stack[kStackSize];
	};

	PagingStructures paging_structs;
	arch::VmmRegions* vmxon_region;
	arch::VmmRegions* vmcs_region;
	void* msr_bitmap;
};
