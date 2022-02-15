#include <ntddk.h>
#include <intrin.h>

#include "arch.h"

void driver_unload(PDRIVER_OBJECT driver_object);

//Checks if the cpu brand is intel
auto is_intel_cpu() -> bool
{
	arch::cpu_features cpuid_struct = { 0 };
	char vendor_buffer[13];

	::__cpuid(reinterpret_cast<int*>(&cpuid_struct), 0);

	::memcpy_s(vendor_buffer, 12, &cpuid_struct.ebx.raw, 4);
	::memcpy_s(vendor_buffer + 4, 12, &cpuid_struct.edx.raw, 4);
	::memcpy_s(vendor_buffer + 8, 12, &cpuid_struct.ecx.raw, 4);
	vendor_buffer[12] = '\0';
	
	return !::strcmp(vendor_buffer, "GenuineIntel") ? true : false;
}

//Checks if the cpu supports Intel VT-x and enabled in bios
auto is_vmx_supported() -> bool
{
	arch::cpu_features cpuid_struct = { 0 };
	::__cpuid(reinterpret_cast<int*>(&cpuid_struct), 0);

	if (!cpuid_struct.ecx.vmx)
	{
		KdPrint(("[-] VMX is not supported by the cpu\n"));
		return false;
	}
	
	arch::feature_control_msr features = { 0 };
	features.raw = ::__readmsr(static_cast<unsigned long>(arch::Msr::IA32_FEATURE_CONTROL));

	//if virtualization disabled in bios
	if (features.lock &&
		((cpuid_struct.ecx.smx && !features.enable_smx) ||
			(!cpuid_struct.ecx.smx && !features.enable_vmx)))
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

	return STATUS_SUCCESS;
}

void driver_unload(PDRIVER_OBJECT driver_object)
{
	//clean up stuff
}