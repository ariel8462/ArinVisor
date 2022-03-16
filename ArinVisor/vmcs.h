#pragma once

#include "vcpu.h"

class SetupVmcs
{
public:
	SetupVmcs(VirtualCpu* vcpu) noexcept;
	~SetupVmcs() noexcept;

	SetupVmcs(const SetupVmcs&) = delete;
	SetupVmcs& operator=(const SetupVmcs&) = delete;

	SetupVmcs(const SetupVmcs&&) = delete;
	SetupVmcs& operator=(const SetupVmcs&&) = delete;

public:
	bool setup_vmcs_fields();

	operator bool()
	{
		return vcpu_->vmcs_region != nullptr;
	}

private:
	void allocate_vmcs_region() noexcept;

private:
	VirtualCpu* vcpu_;
};
