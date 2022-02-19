#include <ntddk.h>
#include <intrin.h>

#include "arch.h"
#include "load.h"
#include "utils.h"

bool success = false;
void driver_unload(PDRIVER_OBJECT driver_object);

//add logging eventually to some important msr's, control registers etc
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(registry_path);

	//bla bla stuff to initialize, driver unload routine, irp major functions
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

	success = load::load_hypervisor(vcpu);

	if (!success)
	{
		KdPrint(("[-] Failed to load ArinVisor"));
		return STATUS_UNSUCCESSFUL;
	}
	
	KdPrint(("[+] ArinVisor loaded successfully\n"));

	return STATUS_SUCCESS;
}

void driver_unload(PDRIVER_OBJECT driver_object)
{
	UNREFERENCED_PARAMETER(driver_object);

	if (success)
	{
		::__vmx_off();
	}

	//clean up stuff
	if (vcpu != nullptr)
	{
		if (vcpu->vmcs_region)
		{
			MmFreeContiguousMemory(vcpu->vmcs_region);
		}
		if (vcpu->vmxon_region)
		{
			MmFreeContiguousMemory(vcpu->vmxon_region);
		}
		ExFreePool(vcpu);
	}

	KdPrint(("[+] ArinVisor unloaded successfully\n"));
}
