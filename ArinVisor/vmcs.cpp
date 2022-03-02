#include <ntddk.h>
#include <intrin.h>

#include "vmcs.h"
#include "arch.h"
#include "vmm.h"
#include "helpers.h"

template <typename T>
static constexpr auto vmwrite(arch::VmcsFields vmcs_field, T field_value)
{
	auto failed = ::__vmx_vmwrite(static_cast<size_t>(vmcs_field), static_cast<size_t>(field_value));

	return failed == STATUS_SUCCESS;
}

template <typename T>
static constexpr auto vmread(arch::VmcsFields vmcs_field, T* field_value)
{
	auto failed = ::__vmx_vmread(static_cast<size_t>(vmcs_field), static_cast<size_t>(field_value));

	return failed == STATUS_SUCCESS;
}

bool vmcs::setup_vmcs(VirtualCpu*& vcpu)
{
	arch::Ia32VmxBasicMsr ia32_vmx_basic;
	ia32_vmx_basic.raw = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_BASIC)
	);

	vcpu->vmcs_region->header.raw = ia32_vmx_basic.bits.revision_identifier;
	vcpu->vmcs_region->header.bits.shadow_vmcs_indicator = 0;

	unsigned long long vmcs_region_physical_address = MmGetPhysicalAddress(vcpu->vmcs_region).QuadPart;

	if (!vmcs_region_physical_address)
	{
		KdPrint(("[-] Failed getting the physical address of vmcs region\n"));
		return false;
	}

	auto failed = ::__vmx_vmclear(&vmcs_region_physical_address);

	if (failed)
	{
		KdPrint(("[-] Failed executing VMCLEAR\n"));
		return false;
	}

	failed = ::__vmx_vmptrld(&vmcs_region_physical_address);

	if (failed)
	{
		KdPrint(("[-] Failed executing VMPTRLD\n"));
		return false;
	}

	//setup all vmcs fields: guest-state, host-state, vm execution controls, to do: setup rip & rsp
	auto success = vmwrite(arch::VmcsFields::VMCS_GUEST_CR0, ::__readcr0());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_CR3, ::__readcr3());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_CR4, ::__readcr4());

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_DR7, ::__readdr(7));
	
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_RFLAGS, ::__readeflags());

	//rip and rsp

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_DEBUGCTL, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_DEBUGCTL)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SYSENTER_CS, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_CS)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SYSENTER_ESP, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_ESP)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SYSENTER_EIP, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_EIP)
		)
	);

	arch::DescriptorTable gdtr = { 0 };
	arch::DescriptorTable idtr = { 0 };

	::_sgdt(&gdtr);
	::__sidt(&idtr);
	
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_SELECTOR, ::_read_ldtr());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_TR_SELECTOR, ::_read_tr());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_CS_SELECTOR, ::_read_cs());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_DS_SELECTOR, ::_read_ds());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SS_SELECTOR, ::_read_ss());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_GS_SELECTOR, ::_read_gs());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_FS_SELECTOR, ::_read_fs());
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_ES_SELECTOR, ::_read_es());

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_LIMIT, __segmentlimit(::_read_ldtr()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_TR_LIMIT, __segmentlimit(::_read_tr()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_CS_LIMIT, __segmentlimit(::_read_cs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_DS_LIMIT, __segmentlimit(::_read_ds()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SS_LIMIT, __segmentlimit(::_read_ss()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_GS_LIMIT, __segmentlimit(::_read_gs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_FS_LIMIT, __segmentlimit(::_read_fs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_ES_LIMIT, __segmentlimit(::_read_es()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_GDTR_LIMIT, gdtr.limit);
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_IDTR_LIMIT, idtr.limit);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_ldtr()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_TR_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_tr()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_CS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_cs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_DS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_ds()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_ss()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_GS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_gs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_FS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_fs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_ES_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_es()));

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_BASE, arch::get_segment_base(gdtr.base, ::_read_ldtr()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_TR_BASE, arch::get_segment_base(gdtr.base, ::_read_tr()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_CS_BASE, arch::get_segment_base(gdtr.base, ::_read_cs()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_DS_BASE, arch::get_segment_base(gdtr.base, ::_read_ds()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_SS_BASE, arch::get_segment_base(gdtr.base, ::_read_ss()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_ES_BASE, arch::get_segment_base(gdtr.base, ::_read_es()));
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_GDTR_BASE, gdtr.base);
	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_IDTR_BASE, idtr.base);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_GS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_GS_BASE)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_FS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_FS_BASE)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_GUEST_VMCS_LINK_POINTER, 0xffffffffffffffff);

	success = vmwrite(arch::VmcsFields::VMCS_HOST_CR0, ::__readcr0());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_CR3, ::__readcr3());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_CR4, ::__readcr4());

	//rip & rsp

	success &= vmwrite(arch::VmcsFields::VMCS_HOST_TR_SELECTOR, ::_read_tr());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_CS_SELECTOR, ::_read_cs());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_DS_SELECTOR, ::_read_ds());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_SS_SELECTOR, ::_read_ss());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_GS_SELECTOR, ::_read_gs());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_FS_SELECTOR, ::_read_fs());
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_ES_SELECTOR, ::_read_es());

	success &= vmwrite(arch::VmcsFields::VMCS_HOST_GS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_GS_BASE)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_HOST_FS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_FS_BASE)
		)
	);

	success &= vmwrite(arch::VmcsFields::VMCS_HOST_GDTR_BASE, gdtr.base);
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_IDTR_BASE, idtr.base);
	success &= vmwrite(arch::VmcsFields::VMCS_HOST_TR_BASE, arch::get_segment_base(gdtr.base, ::_read_tr()));

	success &= vmwrite(arch::VmcsFields::VMCS_CTRL_CR0_READ_SHADOW, ::__readcr0());
	success &= vmwrite(arch::VmcsFields::VMCS_CTRL_CR4_READ_SHADOW, ::__readcr4());

	if (!success)
	{
		return false;
	}
	
	return true;
}

auto vmcs::allocate_vmcs_region() -> arch::VmmRegions*
{
	PHYSICAL_ADDRESS physical_max = { 0 };
	physical_max.QuadPart = MAXULONG64;

	auto vmcs_region = reinterpret_cast<arch::VmmRegions*>(
		MmAllocateContiguousMemory(arch::VMX_REGION_SIZE, physical_max)
		);

	if (!vmcs_region)
	{
		KdPrint(("[-] Vmcs region allocation failed\n"));
		return nullptr;
	}

	RtlSecureZeroMemory(vmcs_region, arch::VMX_REGION_SIZE);

	return vmcs_region;
}
