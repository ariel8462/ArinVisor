#pragma once

#include "arch.h"
#include "vcpu.h"

class Vmxon
{
public:
	Vmxon(VirtualCpu* vcpu) noexcept;
	~Vmxon();

	Vmxon(const Vmxon&) = delete;
	Vmxon& operator=(const Vmxon&) = delete;

public:
	bool init_vmxon();

	operator bool()
	{
		return vcpu_->vmxon_region != nullptr;
	}

private:
	void enable_vmx() const noexcept;
	void allocate_vmxon_region() noexcept;

private:
	VirtualCpu* vcpu_;
};

namespace vmx
{
	void vmxoff();

	template <typename T>
	constexpr auto vmwrite(arch::VmcsFields vmcs_field, T field_value)
	{
		auto success = __vmx_vmwrite(static_cast<size_t>(vmcs_field), static_cast<size_t>(field_value));

		return success == STATUS_SUCCESS;
	}

	template <typename T>
	constexpr auto vmread(arch::VmcsFields vmcs_field, T* field_value)
	{
		auto success = ::__vmx_vmread(static_cast<size_t>(vmcs_field), field_value);

		return success == STATUS_SUCCESS;
	}
}
