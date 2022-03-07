#pragma once

#include "arch.h"
#include "vcpu.h"

namespace vmx
{
	void enable_vmx();
	bool init_vmxon(VirtualCpu* vcpu);
	void vmxoff();
	auto allocate_vmxon_region()->arch::VmmRegions*;
}

template <typename T>
constexpr auto vmwrite(arch::VmcsFields vmcs_field, T field_value)
{
	auto success = ::__vmx_vmwrite(static_cast<size_t>(vmcs_field), static_cast<size_t>(field_value));

	return success == STATUS_SUCCESS;
}

template <typename T>
constexpr auto vmread(arch::VmcsFields vmcs_field, T* field_value)
{
	auto success = ::__vmx_vmread(static_cast<size_t>(vmcs_field), field_value);

	return success == STATUS_SUCCESS;
}
