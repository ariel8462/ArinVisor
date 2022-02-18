#include <ntddk.h>
#include <intrin.h>

#include "vmx.h"
#include "arch.h"
#include "load.h"

VirtualCpu* vcpu = nullptr;

void driver_unload(PDRIVER_OBJECT driver_object);

//Checks if the cpu brand is intel
auto is_intel_cpu() -> bool
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

//Checks if the cpu supports Intel VT-x and enabled in bios
auto is_vmx_supported() -> bool
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
		((cpuid_info.ecx.smx && !feature_control.enable_vmx_in_smx) ||
			(!cpuid_info.ecx.smx && !feature_control.enable_vmx)))
	{
		KdPrint(("[-] VMX is disabled in bios\n"));
		return false;
	}
	
	return true;
}

//add logging eventually, move is_intel_cpu and is_vmx_supported to seperate file
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(registry_path);

	//bla bla stuff to initialize, driver unload routine, irp major functions
	driver_object->DriverUnload = driver_unload;

	if (!is_intel_cpu())
	{
		KdPrint(("[-] The CPU is not supported\n"));
		return STATUS_NOT_SUPPORTED;
	}
	
	if (!is_vmx_supported())
	{
		return STATUS_NOT_SUPPORTED;
	}

	auto success = load::load_hypervisor(vcpu);

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

	//clean up stuff
	if (vcpu != nullptr)
	{
		KdPrint(("[+] Deleting vcpu OwO\n"));
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
