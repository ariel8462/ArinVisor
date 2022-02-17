#include <intrin.h>
#include <ntddk.h>

#include "vmx.h"

void vmx::enable_vmx()
{
	arch::Cr4 cr4;
	cr4.raw = ::__readcr4();

	cr4.vmxe = true;
	::__writecr4(cr4.raw);

	arch::FeatureControlMsr feature_control;
	feature_control.raw = ::__readmsr(static_cast<unsigned long>(arch::Msr::IA32_FEATURE_CONTROL));

	if (!feature_control.lock)
	{
		feature_control.lock = 1;
		feature_control.enable_vmx = 1;

		::__writemsr(
			static_cast<unsigned long>(arch::Msr::IA32_FEATURE_CONTROL), feature_control.raw
		);

		KdPrint(("[+] Enabled VMX in IA32_FEATURE_CONTROL msr\n"));
	}

	KdPrint(("lock enabled: %lld\nvmx in smx enabled: %lld\nvmx enabled: %lld\n",
		feature_control.lock, feature_control.enable_vmx_in_smx, feature_control.enable_vmx));

	auto ia32_vmx_cr0_fixed0 = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_CR0_FIXED0)
	);

	auto ia32_vmx_cr0_fixed1 = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_CR0_FIXED1)
	);

	//to do - make cr0 struct
	auto cr0 = ::__readcr0();
	cr0 |= ia32_vmx_cr0_fixed0;
	cr0 &= ia32_vmx_cr0_fixed1;
	::__writecr0(cr0);

	auto ia32_vmx_cr4_fixed0 = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_CR4_FIXED0)
	);

	auto ia32_vmx_cr4_fixed1 = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_CR4_FIXED1)
	);

	RtlSecureZeroMemory(&cr4, sizeof(arch::Cr4));
	cr4.raw = ::__readcr4();

	cr4.raw |= ia32_vmx_cr4_fixed0;
	cr4.raw &= ia32_vmx_cr4_fixed1;
	::__writecr4(cr4.raw);

	KdPrint(("Adjusted cr0 and cr4\ncr0 = %lld\ncr4 = %lld\n", cr0, cr4.raw));
}

auto vmx::allocate_vcpu() -> VirtualCpu*
{
	VirtualCpu* vcpu = reinterpret_cast<VirtualCpu*>(
		ExAllocatePoolWithTag(NonPagedPool, sizeof(VirtualCpu), 'arin')
		);

	if (!vcpu)
	{
		KdPrint(("[-] Allocating vcpu failed\n"));
		return nullptr;
	}

	RtlSecureZeroMemory(vcpu, sizeof(VirtualCpu));

	PHYSICAL_ADDRESS physical_max = { 0 };
	physical_max.QuadPart = MAXULONG64;
	vcpu->vmcs_region = reinterpret_cast<arch::VmmRegions*>(
		MmAllocateContiguousMemory(arch::VMX_BASIC_MSR_SIZE, physical_max)
		);
	
	if (!vcpu->vmcs_region)
	{
		KdPrint(("[-] Allocating vmcs region failed\n"));
		return nullptr;
	}

	RtlSecureZeroMemory(vcpu->vmcs_region, arch::VMX_BASIC_MSR_SIZE);
	vcpu->vmxon_region = reinterpret_cast<arch::VmmRegions*>(
		MmAllocateContiguousMemory(arch::VMX_BASIC_MSR_SIZE, physical_max)
		);

	if (!vcpu->vmxon_region)
	{
		KdPrint(("[-] Allocating vmxon region failed\n"));
		return nullptr;
	}

	RtlSecureZeroMemory(vcpu->vmxon_region, arch::VMX_BASIC_MSR_SIZE);

	return vcpu;
}

int vmx::init_vmxon(VirtualCpu* vcpu)
{
	UNREFERENCED_PARAMETER(vcpu);
	arch::Ia32VmxBasicMsr ia32_vmx_basic;
	ia32_vmx_basic.raw = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_BASIC)
	);

	vcpu->vmxon_region->header.raw = ia32_vmx_basic.revision_identifier;
	vcpu->vmxon_region->header.bits.shadow_vmcs_indicator = 0;

	unsigned long long vmxon_region_physical_address = MmGetPhysicalAddress(vcpu->vmxon_region).QuadPart;

	if (!vmxon_region_physical_address)
	{
		KdPrint(("[-] Failed getting the physical address of vmxon region\n"));
		return false;
	}

	auto status = ::__vmx_on(&vmxon_region_physical_address);

	if (status)
	{
		KdPrint(("[-] VMXON failed\n"));
		return false;
	}

	KdPrint(("[+] Entered VMX state!\n"));
	KdPrint(("Vcpu %d is now in VMX operation\n", KeGetCurrentProcessorNumber()));
	return true;
}