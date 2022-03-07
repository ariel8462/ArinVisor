#include <ntddk.h>
#include <intrin.h>

#include "vmx.h"
#include "utils.h"
#include "arch.h"
#include "exit_handler.h"

void vmx::enable_vmx()
{
	arch::Cr4 cr4;
	cr4.raw = ::__readcr4();

	cr4.bits.vmxe = true;
	::__writecr4(cr4.raw);

	arch::FeatureControlMsr feature_control;
	feature_control.raw = ::__readmsr(static_cast<unsigned long>(arch::Msr::IA32_FEATURE_CONTROL));

	if (!feature_control.bits.lock)
	{
		feature_control.bits.lock = 1;
		feature_control.bits.enable_vmx = 1;
		feature_control.bits.enable_vmx_in_smx = 1;

		::__writemsr(
			static_cast<unsigned long>(arch::Msr::IA32_FEATURE_CONTROL), feature_control.raw
		);

		KdPrint(("[+] Enabled VMX in IA32_FEATURE_CONTROL msr\n"));
	}

	KdPrint(("[+] lock enabled: %lld\n   vmx in smx enabled: %lld\n   vmx enabled: %lld\n",
		feature_control.bits.lock, feature_control.bits.enable_vmx_in_smx, feature_control.bits.enable_vmx));

	auto ia32_vmx_cr0_fixed0 = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_CR0_FIXED0)
	);

	auto ia32_vmx_cr0_fixed1 = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_CR0_FIXED1)
	);

	arch::Cr0 cr0;
	cr0.raw = ::__readcr0();
	cr0.raw |= ia32_vmx_cr0_fixed0;
	cr0.raw &= ia32_vmx_cr0_fixed1;
	::__writecr0(cr0.raw);

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

	KdPrint(("[+] Adjusted cr0 and cr4:\n   cr0 = %lld\n   cr4 = %lld\n", cr0.raw, cr4.raw));
}

bool vmx::init_vmxon(VirtualCpu* vcpu)
{
	arch::Ia32VmxBasicMsr ia32_vmx_basic;
	ia32_vmx_basic.raw = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_BASIC)
	);

	vcpu->vmxon_region->header.raw = ia32_vmx_basic.bits.revision_identifier;
	vcpu->vmxon_region->header.bits.shadow_vmcs_indicator = 0;

	unsigned long long vmxon_region_physical_address = MmGetPhysicalAddress(vcpu->vmxon_region).QuadPart;

	if (!vmxon_region_physical_address)
	{
		KdPrint(("[-] Failed getting the physical address of vmxon region\n"));
		return false;
	}

	auto failed = ::__vmx_on(&vmxon_region_physical_address);

	if (failed)
	{
		KdPrint(("[-] VMXON failed\n"));
		return false;
	}

	KdPrint(("[+] Entered VMX state!\n"));
	KdPrint(("[+] Vcpu %d is now in VMX operation\n", vcpu->processor_number));

	return true;
}

auto vmx::allocate_vmxon_region() -> arch::VmmRegions*
{
	PHYSICAL_ADDRESS physical_max = { 0 };
	physical_max.QuadPart = MAXULONG64;

	auto vmxon_region = reinterpret_cast<arch::VmmRegions*>(
		MmAllocateContiguousMemory(arch::VMX_REGION_SIZE, physical_max)
		);

	if (!vmxon_region)
	{
		KdPrint(("[-] Vmxon region allocation failed\n"));
		return nullptr;
	}

	RtlSecureZeroMemory(vmxon_region, arch::VMX_REGION_SIZE);

	return vmxon_region;
}

void vmx::vmxoff()
{
	GROUP_AFFINITY affinity = { 0 };
	GROUP_AFFINITY original_affinity = { 0 };
	PROCESSOR_NUMBER processor_number = { 0 };

	for (unsigned long i = 0; i < vmm_context->processor_count; i++)
	{
		RtlSecureZeroMemory(&affinity, sizeof(GROUP_AFFINITY));
		KeGetProcessorNumberFromIndex(i, &processor_number);

		affinity.Group = processor_number.Group;
		affinity.Mask = 1ull << processor_number.Number;

		KeSetSystemGroupAffinityThread(&affinity, &original_affinity);

		::__vmx_off();

		KeRevertToUserGroupAffinityThread(&original_affinity);
	}
}

extern "C" void vm_exit_handler(guest_state_vmx guest_state)
{
	unsigned long long exit_reason;

	KdPrint(("[+] caught VM-exit!\n"));
	vmread(arch::VmcsFields::VMCS_EXIT_REASON, &exit_reason);
	KdPrint(("[*] exit reason: %lld\n", exit_reason));
	
	bool increment_rip = true;
	vmm_context->exit_handler.handle_vm_exit(exit_reason, guest_state, increment_rip);

	if (increment_rip)
	{
		unsigned long long current_rip;
		size_t instruction_length = 0;

		vmread(arch::VmcsFields::VMCS_GUEST_RIP, &current_rip);
		vmread(arch::VmcsFields::VMCS_VMEXIT_INSTRUCTION_LENGTH, &instruction_length);
		vmwrite(arch::VmcsFields::VMCS_GUEST_RIP, reinterpret_cast<unsigned long long>(
			reinterpret_cast<unsigned char*>(current_rip) + instruction_length
			)
		);
	}
}
