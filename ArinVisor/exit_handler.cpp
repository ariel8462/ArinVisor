#include <ntddk.h>
#include <intrin.h>

#include "exit_handler.h"
#include "vmx.h"

void ExitHandler::handle_vm_exit(unsigned long long exit_reason, VirtualCpu* vcpu, guest_state_vmx& guest_state, bool& increment_rip) noexcept
{
	switch (exit_reason)
	{
	case 0: return vmexit_exception_or_nmi_interrupt(vcpu, guest_state, increment_rip);
	case 1: return vmexit_external_interrupt(vcpu, guest_state, increment_rip);
	case 2: return vmexit_triple_fault(vcpu, guest_state, increment_rip);
	case 3: return vmexit_init_singal(vcpu, guest_state, increment_rip);
	case 4: return vmexit_sipi(vcpu, guest_state, increment_rip);
	case 5: return vmexit_smi(vcpu, guest_state, increment_rip);
	case 6: return vmexit_other_smi(vcpu, guest_state, increment_rip);
	case 7: return vmexit_interrupt_window(vcpu, guest_state, increment_rip);
	case 8: return vmexit_nmi_window(vcpu, guest_state, increment_rip);
	case 9: return vmexit_task_switch(vcpu, guest_state, increment_rip);
	case 10: return vmexit_cpuid(vcpu, guest_state, increment_rip);
	case 11: return vmexit_getsec(vcpu, guest_state, increment_rip);
	case 12: return vmexit_hlt(vcpu, guest_state, increment_rip);
	case 13: return vmexit_invd(vcpu, guest_state, increment_rip);
	case 14: return vmexit_invlpg(vcpu, guest_state, increment_rip);
	case 15: return vmexit_rdpmc(vcpu, guest_state, increment_rip);
	case 16: return vmexit_rdtsc(vcpu, guest_state, increment_rip);
	case 17: return vmexit_rsm(vcpu, guest_state, increment_rip);
	case 18: return vmexit_vmcall(vcpu, guest_state, increment_rip);
	case 19: return vmexit_vmclear(vcpu, guest_state, increment_rip);
	case 20: return vmexit_vmlaunch(vcpu, guest_state, increment_rip);
	case 21: return vmexit_vmptrld(vcpu, guest_state, increment_rip);
	case 22: return vmexit_vmptrst(vcpu, guest_state, increment_rip);
	case 23: return vmexit_vmread(vcpu, guest_state, increment_rip);
	case 24: return vmexit_vmresume(vcpu, guest_state, increment_rip);
	case 25: return vmexit_vmwrite(vcpu, guest_state, increment_rip);
	case 26: return vmexit_vmxoff(vcpu, guest_state, increment_rip);
	case 27: return vmexit_vmxon(vcpu, guest_state, increment_rip);
	case 28: return vmexit_cr_access(vcpu, guest_state, increment_rip);
	case 29: return vmexit_mov_dr(vcpu, guest_state, increment_rip);
	case 30: return vmexit_io_instruction(vcpu, guest_state, increment_rip);
	case 31: return vmexit_rdmsr(vcpu, guest_state, increment_rip);
	case 32: return vmexit_wrmsr(vcpu, guest_state, increment_rip);
	case 33: return vmexit_vmentry_failure_invalid_guest_state(vcpu, guest_state, increment_rip);
	case 34: return vmexit_vmentry_failure_msr_loading(vcpu, guest_state, increment_rip);
	case 36: return vmexit_mwait(vcpu, guest_state, increment_rip);
	case 37: return vmexit_monitor_trap_flag(vcpu, guest_state, increment_rip);
	case 39: return vmexit_monitor(vcpu, guest_state, increment_rip);
	case 40: return vmexit_pause(vcpu, guest_state, increment_rip);
	case 41: return vmexit_vmentry_failure_machine_check_event(vcpu, guest_state, increment_rip);
	case 43: return vmexit_tpr_below_threshold(vcpu, guest_state, increment_rip);
	case 44: return vmexit_apic_access(vcpu, guest_state, increment_rip);
	case 45: return vmexit_virtualized_eoi(vcpu, guest_state, increment_rip);
	case 46: return vmexit_access_to_gdtr_or_idtr(vcpu, guest_state, increment_rip);
	case 47: return vmexit_access_to_ldtr_or_tr(vcpu, guest_state, increment_rip);
	case 48: return vmexit_ept_violation(vcpu, guest_state, increment_rip);
	case 49: return vmexit_ept_misconfiguration(vcpu, guest_state, increment_rip);
	case 50: return vmexit_invept(vcpu, guest_state, increment_rip);
	case 51: return vmexit_rdtscp(vcpu, guest_state, increment_rip);
	case 52: return vmexit_vmx_preemption_timer_expired(vcpu, guest_state, increment_rip);
	case 53: return vmexit_invvpid(vcpu, guest_state, increment_rip);
	case 54: return vmexit_wbinvd_or_wbnoinvd(vcpu, guest_state, increment_rip);
	case 55: return vmexit_xsetbv(vcpu, guest_state, increment_rip);
	case 56: return vmexit_apic_write(vcpu, guest_state, increment_rip);
	case 57: return vmexit_rdrand(vcpu, guest_state, increment_rip);
	case 58: return vmexit_invpcid(vcpu, guest_state, increment_rip);
	case 59: return vmexit_vmfunc(vcpu, guest_state, increment_rip);
	case 60: return vmexit_encls(vcpu, guest_state, increment_rip);
	case 61: return vmexit_rdseed(vcpu, guest_state, increment_rip);
	case 62: return vmexit_page_modification_log_full(vcpu, guest_state, increment_rip);
	case 63: return vmexit_xsaves(vcpu, guest_state, increment_rip);
	case 64: return vmexit_xrstors(vcpu, guest_state, increment_rip);
	case 66: return vmexit_spp_related_event(vcpu, guest_state, increment_rip);
	case 67: return vmexit_umwait(vcpu, guest_state, increment_rip);
	case 68: return vmexit_tpause(vcpu, guest_state, increment_rip);
	case 69: return vmexit_loadiwkey(vcpu, guest_state, increment_rip);
	}
}

void ExitHandler::vmexit_exception_or_nmi_interrupt(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_external_interrupt(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_triple_fault(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_init_singal(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_sipi(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_smi(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_other_smi(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_interrupt_window(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_nmi_window(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_task_switch(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_cpuid(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_getsec(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_hlt(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invd(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invlpg(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdpmc(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdtsc(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rsm(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmcall(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmclear(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmlaunch(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmptrld(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmptrst(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmread(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmresume(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmwrite(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmxoff(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{
	unsigned long long current_rip;
	size_t instruction_length = 0;

	vmread(arch::VmcsFields::VMCS_GUEST_RIP, &current_rip);
	vmread(arch::VmcsFields::VMCS_VMEXIT_INSTRUCTION_LENGTH, &instruction_length);

	vcpu->guest_context.Rip = reinterpret_cast<unsigned long long>(
		reinterpret_cast<unsigned char*>(current_rip) + instruction_length);

	vmread(arch::VmcsFields::VMCS_GUEST_RSP, &vcpu->guest_context.Rsp);

	unsigned long long vmcs_region_physical_address = MmGetPhysicalAddress(vcpu->vmcs_region).QuadPart;

	::__vmx_vmclear(&vmcs_region_physical_address);
	::__vmx_off();

	increment_rip = false;
}

void ExitHandler::vmexit_vmxon(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_cr_access(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_mov_dr(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_io_instruction(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdmsr(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_wrmsr(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmentry_failure_invalid_guest_state(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmentry_failure_msr_loading(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_mwait(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_monitor_trap_flag(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_monitor(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_pause(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmentry_failure_machine_check_event(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_tpr_below_threshold(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_apic_access(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_virtualized_eoi(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_access_to_gdtr_or_idtr(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_access_to_ldtr_or_tr(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_ept_violation(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_ept_misconfiguration(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invept(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdtscp(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmx_preemption_timer_expired(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invvpid(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_wbinvd_or_wbnoinvd(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_xsetbv(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_apic_write(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdrand(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invpcid(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmfunc(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_encls(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdseed(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_page_modification_log_full(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_xsaves(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_xrstors(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_spp_related_event(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_umwait(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_tpause(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_loadiwkey(VirtualCpu* vcpu, guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}                                  
