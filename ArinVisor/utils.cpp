#include <ntddk.h>
#include <intrin.h>

#include "utils.h"
#include "arch.h"
#include "vcpu.h"
#include "helpers.h"
#include "vmx.h"

constexpr static unsigned long kInveptType = 0x2;

bool utils::is_intel_cpu()
{
	arch::CpuFeatures cpuid_info = { 0 };
	char vendor_buffer[13];

	::__cpuid(reinterpret_cast<int*>(&cpuid_info), arch::CPUID_BASIC_INFO);

	::memcpy_s(vendor_buffer, 12, &cpuid_info.ebx.raw, 4);
	::memcpy_s(vendor_buffer + 4, 12, &cpuid_info.edx.raw, 4);
	::memcpy_s(vendor_buffer + 8, 12, &cpuid_info.ecx.raw, 4);
	vendor_buffer[12] = '\0';

	return !::strcmp(vendor_buffer, "GenuineIntel") ? true : false;
}

bool utils::is_vmx_supported()
{
	arch::CpuFeatures cpuid_info = { 0 };
	::__cpuid(reinterpret_cast<int*>(&cpuid_info), arch::CPUID_VERSION_INFO);

	if (!cpuid_info.ecx.vmx)
	{
		KdPrint(("[-] The cpu doesn't support VMX\n"));
		return false;
	}

	arch::FeatureControlMsr feature_control;
	feature_control.raw = ::__readmsr(static_cast<unsigned long>(arch::Msr::IA32_FEATURE_CONTROL));

	if (feature_control.bits.lock && 
		(!feature_control.bits.enable_vmx && !feature_control.bits.enable_vmx_in_smx))
	{
		KdPrint(("[-] VMX is disabled in bios\n"));
		return false;
	}

	return true;
}

void utils::free_memory(VmmContext* vmm_context)
{
	for (unsigned long i = 0; i < vmm_context->processor_count; i++)
	{
		delete vmm_context->vmxon[i];
		delete vmm_context->setup_vmcs[i];
		delete vmm_context->ept[i];
		delete vmm_context->processors_vcpu[i];
	}
	
	delete vmm_context;
}

extern "C" unsigned long long read_rip()
{
	auto vcpu = vmm_context->processors_vcpu[KeGetCurrentProcessorNumber()];
	return vcpu->guest_context.Rip;
}

extern "C" unsigned long long read_rsp()
{
	auto vcpu = vmm_context->processors_vcpu[KeGetCurrentProcessorNumber()];
	return vcpu->guest_context.Rsp;
}

bool utils::is_hypervisor_present()
{
	arch::CpuFeatures cpuid_info = { 0 };

	::__cpuid(reinterpret_cast<int*>(&cpuid_info), 0);

	return cpuid_info.ebx.raw == 0x41414141;
}

auto utils::invept(ULONG_PTR) -> ULONG_PTR
{
	arch::InveptDescriptor descriptor = { 0 };

	vmx::vmread(arch::VmcsFields::VMCS_CTRL_EPT_POINTER, &descriptor.eptp);
	_invept(kInveptType, &descriptor);

	return 0;
}
