#include <ntddk.h>
#include <intrin.h>

#include "utils.h"
#include "arch.h"

auto utils::is_intel_cpu() -> bool
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

auto utils::is_vmx_supported() -> bool
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

	if (feature_control.lock && 
		(!feature_control.enable_vmx && !feature_control.enable_vmx_in_smx))
	{
		KdPrint(("[-] VMX is disabled in bios\n"));
		return false;
	}

	return true;
}
