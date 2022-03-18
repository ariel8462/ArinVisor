#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>

#include "load.h"
#include "vmx.h"
#include "utils.h"
#include "memory.h"
#include "vmcs.h"

static bool allocate_vcpu(VirtualCpu*& vcpu)
{
	vcpu = reinterpret_cast<VirtualCpu*>(
		new (NonPagedPool, kTag) VirtualCpu
		);

	if (!vcpu)
	{
		KdPrint(("[-] Vcpu allocation failed\n"));
		return false;
	}

	RtlSecureZeroMemory(vcpu, sizeof(VirtualCpu));

	return true;
}

bool load::load_hypervisor(VirtualCpu*& vcpu)
{
	if (!allocate_vcpu(vcpu))
	{
		KdPrint(("[-] Error in vcpu allocation\n"));
		return false;
	}
	
	KdPrint(("[+] allocated vcpu successfully\n"));
	
	Vmxon* vmxon = new (NonPagedPool, kTag) Vmxon(vcpu);
	
	if (!vmxon)
	{
		KdPrint(("[-] Error in vmxon region allocation\n"));
		return false;
	}

	KdPrint(("[+] allocated vmxon region successfully\n"));
	KdPrint(("[+] Enabled VMX\n"));

	vcpu->processor_number = KeGetCurrentProcessorNumber();
	SetupVmcs* setup_vmcs = new (NonPagedPool, kTag) SetupVmcs(vcpu);

	if (!setup_vmcs)
	{
		KdPrint(("[-] Error in vmcs region allocation\n"));
		return false;
	}

	KdPrint(("[+] allocated vmcs region successfully\n"));
	auto success = vmxon->init_vmxon();
	
	if (!success)
	{
		KdPrint(("[-] Entering VMX operation failed\n"));
		return false;
	}
	
	success = setup_vmcs->setup_vmcs_fields();

	if (!success)
	{
		KdPrint(("[-] Setting up VMCS failed\n"));
		return false;
	}

	Ept* ept = new (NonPagedPool, kTag) Ept(vcpu);

	vmm_context->vmxon[vcpu->processor_number] = vmxon;
	vmm_context->setup_vmcs[vcpu->processor_number] = setup_vmcs;
	vmm_context->ept[vcpu->processor_number] = ept;

	KdPrint(("[+] executing VMLAUNCH in processor %d\n", vcpu->processor_number));
	RtlCaptureContext(&vcpu->guest_context);

	if (!utils::is_hypervisor_present())
	{
		auto failed = __vmx_vmlaunch();

		if (failed)
		{
			KdPrint(("[-] VMLAUNCH failed\n"));
			return false;
		}
	}
	else
	{
		KdPrint(("[+] In guest!\n"));
	}

	return true;
}
