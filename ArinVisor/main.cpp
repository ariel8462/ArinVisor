#include <ntddk.h>
#include <intrin.h>

#include "arch.h"
#include "load.h"
#include "utils.h"
#include "vmm.h"
#include "vcpu.h"
#include "vmx.h"
#include "memory.h"

VmmContext* vmm_context = nullptr;
void driver_unload(PDRIVER_OBJECT driver_object);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(registry_path);

	GROUP_AFFINITY affinity = { 0 };
	GROUP_AFFINITY original_affinity = { 0 };
	PROCESSOR_NUMBER processor_number = { 0 };

	driver_object->DriverUnload = driver_unload;

	if (!utils::is_intel_cpu())
	{
		KdPrint(("[-] The CPU is not supported\n"));
		return STATUS_NOT_SUPPORTED;
	}
	
	if (!utils::is_vmx_supported())
	{
		return STATUS_NOT_SUPPORTED;
	}

	vmm_context = new (NonPagedPool, kTag) VmmContext;

	if (!vmm_context)
	{
		KdPrint(("[-] VMM context struct allocation failed\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	vmm_context->processor_count = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);

	//launch ArinVisor in all cores
	for (unsigned long i = 0; i < vmm_context->processor_count; i++)
	{
		RtlSecureZeroMemory(&affinity, sizeof(GROUP_AFFINITY));
		KeGetProcessorNumberFromIndex(i, &processor_number);

		affinity.Group = processor_number.Group;
		affinity.Mask = 1ull << processor_number.Number;

		KeSetSystemGroupAffinityThread(&affinity, &original_affinity);

		auto success = load::load_hypervisor(vmm_context->processors_vcpu[i]);

		KeRevertToUserGroupAffinityThread(&original_affinity);

		if (!success)
		{
			KdPrint(("[-] loading ArinVisor failed on processor %d\n", i));
			KdPrint(("[-] Failed to load ArinVisor\n"));
			utils::free_memory(vmm_context);

			return STATUS_UNSUCCESSFUL;
		}
	}

	KdPrint(("[+] ArinVisor loaded successfully\n"));

	return STATUS_SUCCESS;
}

void driver_unload(PDRIVER_OBJECT driver_object)
{
	UNREFERENCED_PARAMETER(driver_object);

	vmx::vmxoff();
	utils::free_memory(vmm_context);

	KdPrint(("[+] ArinVisor unloaded successfully\n"));
}
