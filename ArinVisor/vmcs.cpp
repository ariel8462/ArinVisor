#include <ntddk.h>
#include <intrin.h>

#include "vmcs.h"
#include "vmx.h"
#include "arch.h"
#include "helpers.h"
#include "memory.h"

constexpr unsigned short kSelectorHostMask = 0x7;

static void set_control(unsigned int& field, unsigned long long control_set)
{
	arch::ControlSetting bit_control = { control_set };
	
	field |= bit_control.low_part;
	field &= bit_control.high_part;
}

SetupVmcs::SetupVmcs(VirtualCpu* vcpu)  noexcept
	: vcpu_(vcpu)
{
	allocate_vmcs_region();
}

SetupVmcs::~SetupVmcs()
{
	if (vcpu_->vmcs_region != nullptr)
	{
		MmFreeContiguousMemory(vcpu_->vmcs_region);
	}
	if (vcpu_->msr_bitmap != nullptr)
	{
		delete vcpu_->msr_bitmap;
	}
}

bool SetupVmcs::setup_vmcs_fields()
{
	arch::Ia32VmxBasicMsr ia32_vmx_basic;
	ia32_vmx_basic.raw = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_BASIC)
	);

	vcpu_->vmcs_region->header.raw = ia32_vmx_basic.bits.revision_identifier;
	vcpu_->vmcs_region->header.bits.shadow_vmcs_indicator = 0;

	unsigned long long vmcs_region_physical_address = MmGetPhysicalAddress(vcpu_->vmcs_region).QuadPart;

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

	::_sgdt(&gdtr_);
	::__sidt(&idtr_);

	setup_vmcs_guest_state();
	setup_vmcs_host_state();
	setup_vm_execution_control_fields();
	
	if (!success_)
	{
		KdPrint(("Error in some vmwrite\n"));
		return false;
	}
	
	return true;
}

void SetupVmcs::setup_vmcs_guest_state()
{
	setup_vmcs_guest_registers();
	setup_vmcs_guest_segments();

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_VMCS_LINK_POINTER, 0xffffffffffffffff);
}

void SetupVmcs::setup_vmcs_guest_registers()
{
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CR0, ::__readcr0());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CR3, ::__readcr3());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CR4, ::__readcr4());

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_DR7, ::__readdr(7));

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_RFLAGS, ::__readeflags());

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_RIP, reinterpret_cast<unsigned long long>(_enter_guest));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_RSP, reinterpret_cast<unsigned long long>(
		vcpu_->stack + 0x6000 - 8)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_DEBUGCTL, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_DEBUGCTL)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SYSENTER_CS, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_CS)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SYSENTER_ESP, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_ESP)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SYSENTER_EIP, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_EIP)
	)
	);
}

void SetupVmcs::setup_vmcs_guest_segments()
{
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_SELECTOR, ::_read_ldtr());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_TR_SELECTOR, ::_read_tr());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CS_SELECTOR, ::_read_cs());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_DS_SELECTOR, ::_read_ds());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SS_SELECTOR, ::_read_ss());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_GS_SELECTOR, ::_read_gs());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_FS_SELECTOR, ::_read_fs());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_ES_SELECTOR, ::_read_es());

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_LIMIT, __segmentlimit(::_read_ldtr()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_TR_LIMIT, __segmentlimit(::_read_tr()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CS_LIMIT, __segmentlimit(::_read_cs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_DS_LIMIT, __segmentlimit(::_read_ds()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SS_LIMIT, __segmentlimit(::_read_ss()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_GS_LIMIT, __segmentlimit(::_read_gs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_FS_LIMIT, __segmentlimit(::_read_fs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_ES_LIMIT, __segmentlimit(::_read_es()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_GDTR_LIMIT, gdtr_.limit);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_IDTR_LIMIT, idtr_.limit);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_ldtr()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_TR_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_tr()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_cs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_DS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_ds()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_ss()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_GS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_gs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_FS_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_fs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_ES_ACCESS_RIGHTS, arch::get_segment_access_rights(::_read_es()));

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_LDTR_BASE, arch::get_segment_base(gdtr_.base, ::_read_ldtr()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_TR_BASE, arch::get_segment_base(gdtr_.base, ::_read_tr()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_CS_BASE, arch::get_segment_base(gdtr_.base, ::_read_cs()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_DS_BASE, arch::get_segment_base(gdtr_.base, ::_read_ds()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_SS_BASE, arch::get_segment_base(gdtr_.base, ::_read_ss()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_ES_BASE, arch::get_segment_base(gdtr_.base, ::_read_es()));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_GDTR_BASE, gdtr_.base);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_IDTR_BASE, idtr_.base);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_GS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_GS_BASE)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_GUEST_FS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_FS_BASE)
	)
	);
}

void SetupVmcs::setup_vmcs_host_state()
{
	setup_vmcs_host_registers();
	setup_vmcs_host_segments();
}

void SetupVmcs::setup_vmcs_host_registers()
{
	success_ = vmx::vmwrite(arch::VmcsFields::VMCS_HOST_CR0, ::__readcr0());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_CR3, ::__readcr3());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_CR4, ::__readcr4());

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_RIP, reinterpret_cast<unsigned long long>(_vm_exit_handler));
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_RSP, reinterpret_cast<unsigned long long>(
		vcpu_->stack + 0x6000 - 8)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_SYSENTER_CS, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_CS)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_SYSENTER_ESP, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_ESP)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_SYSENTER_EIP, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_SYSENTER_EIP)
	)
	);
}

void SetupVmcs::setup_vmcs_host_segments()
{
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_TR_SELECTOR, ::_read_tr() & ~kSelectorHostMask);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_CS_SELECTOR, ::_read_cs() & ~kSelectorHostMask);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_DS_SELECTOR, ::_read_ds() & ~kSelectorHostMask);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_SS_SELECTOR, ::_read_ss() & ~kSelectorHostMask);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_GS_SELECTOR, ::_read_gs() & ~kSelectorHostMask);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_FS_SELECTOR, ::_read_fs() & ~kSelectorHostMask);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_ES_SELECTOR, ::_read_es() & ~kSelectorHostMask);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_GS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_GS_BASE)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_FS_BASE, ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_FS_BASE)
	)
	);

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_GDTR_BASE, gdtr_.base);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_IDTR_BASE, idtr_.base);
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_HOST_TR_BASE, arch::get_segment_base(gdtr_.base, ::_read_tr()));
}

void SetupVmcs::setup_vm_execution_control_fields()
{
	arch::Ia32VmxBasicMsr vmx_basic_control_msr;

	vmx_basic_control_msr.raw = ::__readmsr(
		static_cast<unsigned long>(arch::Msr::IA32_VMX_BASIC)
	);

	setup_pin_based_execution_control(vmx_basic_control_msr);
	setup_processor_based_execution_control(vmx_basic_control_msr);
	setup_secondary_processor_based_execution_control(vmx_basic_control_msr);
	setup_vmentry_control(vmx_basic_control_msr);
	setup_vmexit_control(vmx_basic_control_msr);
	setup_msr_bitmap();
	setup_eptp();

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_CTRL_CR0_READ_SHADOW, ::__readcr0());
	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_CTRL_CR4_READ_SHADOW, ::__readcr4());
}

void SetupVmcs::setup_pin_based_execution_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr)
{
	arch::PinBasedVmExecutionControl pin_based_execution_control = { 0 };
	
	set_control(
		pin_based_execution_control.raw,
		vmx_basic_control_msr.bits.vmx_capability_hint ?
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_TRUE_PINBASED_CTLS)
		) :
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_PINBASED_CTLS)
		)
	);

	success_ &= vmx::vmwrite(
		arch::VmcsFields::VMCS_CTRL_PIN_BASED_VM_EXECUTION_CONTROLS, pin_based_execution_control.raw
	);
}

void SetupVmcs::setup_processor_based_execution_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr)
{
	arch::ProcessorBasedVmExecutionControl processor_based_execution_control = { 0 };

	processor_based_execution_control.bits.activate_secondary_controls = true;
	processor_based_execution_control.bits.use_msr_bitmaps = true;

	set_control(
		processor_based_execution_control.raw,
		vmx_basic_control_msr.bits.vmx_capability_hint ?
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_TRUE_PROCBASED_CTLS)
		) :
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_PROCBASED_CTLS)
		)
	);

	success_ &= vmx::vmwrite(
		arch::VmcsFields::VMCS_CTRL_PROCESSOR_BASED_VM_EXECUTION_CONTROLS, processor_based_execution_control.raw)
		;
}

void SetupVmcs::setup_secondary_processor_based_execution_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr)
{
	arch::SecondaryProcessorBasedVmExecutionControl secondary_processor_based_execution_control = { 0 };

	secondary_processor_based_execution_control.bits.enable_rdtscp = true;
	secondary_processor_based_execution_control.bits.enable_xsave_xrstors = true;
	secondary_processor_based_execution_control.bits.enable_invpcid = true;
	secondary_processor_based_execution_control.bits.enable_ept = true;

	set_control(
		secondary_processor_based_execution_control.raw,
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_PROCBASED_CTLS2)
		)
	);

	success_ &= vmx::vmwrite(
		arch::VmcsFields::VMCS_CTRL_SECONDARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS,
		secondary_processor_based_execution_control.raw
	);

}

void SetupVmcs::setup_vmentry_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr)
{
	arch::VmEntryControlField vm_entry_control_field = { 0 };

	vm_entry_control_field.bits.ia32e_mode_guest = true;

	set_control(
		vm_entry_control_field.raw,
		vmx_basic_control_msr.bits.vmx_capability_hint ?
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_TRUE_ENTRY_CTLS)
		) :
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_ENTRY_CTLS)
		)
	);

	success_ &= vmx::vmwrite(
		arch::VmcsFields::VMCS_CTRL_VMENTRY_CONTROLS, vm_entry_control_field.raw
	);
}

void SetupVmcs::setup_vmexit_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr)
{
	arch::VmExitControlField vm_exit_control_field = { 0 };

	vm_exit_control_field.bits.host_address_space_size = true;

	set_control(
		vm_exit_control_field.raw,
		vmx_basic_control_msr.bits.vmx_capability_hint ?
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_TRUE_EXIT_CTLS)
		) :
		::__readmsr(
			static_cast<unsigned long>(arch::Msr::IA32_VMX_EXIT_CTLS)
		)
	);

	success_ &= vmx::vmwrite(
		arch::VmcsFields::VMCS_CTRL_VMEXIT_CONTROLS, vm_exit_control_field.raw
	);
}

void SetupVmcs::setup_msr_bitmap()
{
	vcpu_->msr_bitmap = new (NonPagedPool, kTag) char[PAGE_SIZE];

	if (!vcpu_->msr_bitmap)
	{
		KdPrint(("[-] Failed allocating msr bitmap\n"));
		success_ = false;
		return;
	}

	RtlSecureZeroMemory(vcpu_->msr_bitmap, PAGE_SIZE);
	auto msr_bitmap_physical_address = MmGetPhysicalAddress(vcpu_->msr_bitmap).QuadPart;

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_CTRL_MSR_BITMAP_ADDRESS, msr_bitmap_physical_address);
}

void SetupVmcs::setup_eptp()
{
	arch::Eptp eptp = { 0 };

	eptp.bits.walk_length = 3;
	eptp.bits.memory_type = 6; // write-back
	eptp.bits.pml4 = MmGetPhysicalAddress(&vcpu_->paging_structs.pml4).QuadPart >> 12;

	success_ &= vmx::vmwrite(arch::VmcsFields::VMCS_CTRL_EPT_POINTER, eptp.raw);
}

void SetupVmcs::allocate_vmcs_region() noexcept
{
	PHYSICAL_ADDRESS physical_max = { 0 };
	physical_max.QuadPart = MAXULONG64;

	auto vmcs_region = reinterpret_cast<arch::VmmRegions*>(
		MmAllocateContiguousMemory(arch::VMX_REGION_SIZE, physical_max)
		);

	if (!vmcs_region)
	{
		KdPrint(("[-] Vmcs region allocation failed\n"));
		return;
	}

	RtlSecureZeroMemory(vmcs_region, arch::VMX_REGION_SIZE);
	vcpu_->vmcs_region = vmcs_region;
}
