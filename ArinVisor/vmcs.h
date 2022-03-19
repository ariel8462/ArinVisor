#pragma once

#include "vcpu.h"

class SetupVmcs
{
public:
	SetupVmcs(VirtualCpu* vcpu) noexcept;
	~SetupVmcs();

	SetupVmcs(const SetupVmcs&) = delete;
	SetupVmcs& operator=(const SetupVmcs&) = delete;

public:
	bool setup_vmcs_fields();

	operator bool()
	{
		return vcpu_->vmcs_region != nullptr;
	}

private:
	void allocate_vmcs_region() noexcept;

	void setup_vmcs_guest_state();
	void setup_vmcs_host_state();

	void setup_vmcs_guest_registers();
	void setup_vmcs_guest_segments();

	void setup_vmcs_host_registers();
	void setup_vmcs_host_segments();

	void setup_vm_execution_control_fields();
	void setup_pin_based_execution_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr);
	void setup_processor_based_execution_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr);
	void setup_secondary_processor_based_execution_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr);
	void setup_vmentry_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr);
	void setup_vmexit_control(const arch::Ia32VmxBasicMsr& vmx_basic_control_msr);
	void setup_msr_bitmap();
	void setup_eptp();

private:
	VirtualCpu* vcpu_;
	bool success_ = true;

	arch::DescriptorTable gdtr_ = { 0 };
	arch::DescriptorTable idtr_ = { 0 };
};
