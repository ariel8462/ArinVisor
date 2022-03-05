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
	vmx::enable_vmx();

	KdPrint(("[+] Enabled VMX\n"));
	
	if (!allocate_vcpu(vcpu))
	{
		KdPrint(("[-] Error in vcpu allocation\n"));
		return false;
	}

	KdPrint(("[+] allocated vcpu successfully\n"));

	vcpu->processor_number = KeGetCurrentProcessorNumber();
	vcpu->vmxon_region = vmx::allocate_vmxon_region();

	if (vcpu->vmxon_region == nullptr)
	{
		KdPrint(("[-] Error in vmxon region allocation\n"));
		return false;
	}

	KdPrint(("[+] allocated vmxon region successfully\n"));
	vcpu->vmcs_region = vmcs::allocate_vmcs_region();

	if (vcpu->vmcs_region == nullptr)
	{
		KdPrint(("[-] Error in vmcs region allocation\n"));
		return false;
	}

	KdPrint(("[+] allocated vmcs region successfully\n"));
	auto success = vmx::init_vmxon(vcpu);

	if (!success)
	{
		KdPrint(("[-] Entering VMX operation failed\n"));
		return false;
	}

	success = vmcs::setup_vmcs(vcpu);

	if (!success)
	{
		KdPrint(("[-] Setting up VMCS failed\n"));
		return false;
	}

	RtlCaptureContext(&vcpu->guest_context);

	KdPrint(("[+] executing VMLAUNCH\n"));

	//for now, to test, implement some check of hypervisor presence later, infinite vm-exit on vmlaunch
	//__debugbreak();
	auto failed = __vmx_vmlaunch();

	if (failed)
	{
		KdPrint(("[-] VMLAUNCH failed ig\n"));
		return false;
	}

	__debugbreak();

	return true;
}
