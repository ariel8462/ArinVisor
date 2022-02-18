#include <ntddk.h>

#include "load.h"
#include "vmx.h"

void free_memory(VirtualCpu* vcpu);

bool load::load_hypervisor(VirtualCpu*& vcpu)
{
	vmx::enable_vmx();

	KdPrint(("[+] Enabled VMX\n"));

	vcpu = vmx::allocate_vcpu();
	
	if (vcpu == nullptr)
	{
		KdPrint(("[-] Error in vcpu allocation\n"));
		return false;
	}

	KdPrint(("[+] allocated vcpu successfully"));
	vcpu->vmxon_region = vmx::allocate_vmxon_region();

	if (vcpu->vmxon_region == nullptr)
	{
		KdPrint(("[-] Error in vmxon region allocation\n"));
		free_memory(vcpu);
		return false;
	}

	KdPrint(("[+] allocated vmxon region successfully"));
	vcpu->vmcs_region = vmx::allocate_vmcs_region();

	if (vcpu->vmcs_region == nullptr)
	{
		KdPrint(("[-] Error in vmcs region allocation\n"));
		free_memory(vcpu);
		return false;
	}

	KdPrint(("[+] allocated vmcs region successfully"));
	auto success = vmx::init_vmxon(vcpu);

	if (!success)
	{
		KdPrint(("[-] Entering VMX operation failed\n"));
		free_memory(vcpu);
		return false;
	}

	return true;
}

void free_memory(VirtualCpu* vcpu)
{
	if (vcpu != nullptr)
	{
		if (vcpu->vmxon_region)
		{
			MmFreeContiguousMemory(vcpu->vmxon_region);
		}
		if (vcpu->vmcs_region)
		{
			MmFreeContiguousMemory(vcpu->vmcs_region);
		}
		ExFreePool(vcpu);
	}
}
