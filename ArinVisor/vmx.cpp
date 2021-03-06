#include <ntddk.h>
#include <intrin.h>

#include "vcpu.h"
#include "vmx.h"
#include "utils.h"
#include "arch.h"
#include "exit_handler.h"

Vmxon::Vmxon(VirtualCpu* vcpu) noexcept
	: vcpu_(vcpu)
{
	enable_vmx();
	allocate_vmxon_region();
}

Vmxon::~Vmxon()
{
	if (vcpu_->vmxon_region != nullptr)
	{
		MmFreeContiguousMemory(vcpu_->vmxon_region);
	}
}

void Vmxon::enable_vmx() noexcept
{
	set_feature_control();
	adjust_control_registers();
}

void Vmxon::set_feature_control() noexcept
{
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
}

void Vmxon::adjust_control_registers() noexcept
{
	arch::Cr4 cr4;
	cr4.raw = ::__readcr4();

	cr4.bits.vmxe = true;
	::__writecr4(cr4.raw);

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

void Vmxon::allocate_vmxon_region() noexcept
{
	PHYSICAL_ADDRESS physical_max = { 0 };
	physical_max.QuadPart = MAXULONG64;

	auto vmxon_region = reinterpret_cast<arch::VmmRegions*>(
		MmAllocateContiguousMemory(arch::VMX_REGION_SIZE, physical_max)
		);

	if (!vmxon_region)
	{
		KdPrint(("[-] Vmxon region allocation failed\n"));
		return;
	}

	RtlSecureZeroMemory(vmxon_region, arch::VMX_REGION_SIZE);
	vcpu_->vmxon_region = vmxon_region;
}

bool Vmxon::init_vmxon()
{
	arch::Ia32VmxBasicMsr ia32_vmx_basic;
	ia32_vmx_basic.raw = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_BASIC)
	);

	vcpu_->vmxon_region->header.raw = ia32_vmx_basic.bits.revision_identifier;
	vcpu_->vmxon_region->header.bits.shadow_vmcs_indicator = 0;

	unsigned long long vmxon_region_physical_address = MmGetPhysicalAddress(vcpu_->vmxon_region).QuadPart;

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
	KdPrint(("[+] Vcpu %d is now in VMX operation\n", vcpu_->processor_number));

	return true;
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

extern "C" int vm_exit_handler(guest_state_vmx* guest_state)
{
	unsigned long long exit_reason;

	KdPrint(("[+] caught VM-exit!\n"));
	vmx::vmread(arch::VmcsFields::VMCS_EXIT_REASON, &exit_reason);
	KdPrint(("[*] exit reason: %lld\n", exit_reason));
	
	bool increment_rip = true;
	auto vcpu = vmm_context->processors_vcpu[KeGetCurrentProcessorNumber()];

	vmm_context->exit_handler.handle_vm_exit(exit_reason, vcpu, guest_state, increment_rip);
	
	if (increment_rip)
	{
		unsigned long long current_rip;
		size_t instruction_length = 0;

		vmx::vmread(arch::VmcsFields::VMCS_GUEST_RIP, &current_rip);
		vmx::vmread(arch::VmcsFields::VMCS_VMEXIT_INSTRUCTION_LENGTH, &instruction_length);
		vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_RIP, reinterpret_cast<unsigned long long>(
			reinterpret_cast<unsigned char*>(current_rip) + instruction_length
			)
		);
	}

	//if vmxoff
	return exit_reason == 26 ? 1 : 0;
}
