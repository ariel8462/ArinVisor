#include <ntddk.h>

#include "exit_handler.h"

void ExitHandler::handle_vm_exit(unsigned long long exit_reason, guest_state_vmx guest_state, bool& increment_rip) noexcept
{
	switch (exit_reason)
	{
	case 0: return vmexit_exception_or_nmi_interrupt(guest_state, increment_rip);
	case 1: return vmexit_external_interrupt(guest_state, increment_rip);
	case 2: return vmexit_triple_fault(guest_state, increment_rip);
	case 3: return vmexit_init_singal(guest_state, increment_rip);
	case 4: return vmexit_sipi(guest_state, increment_rip);
	case 5: return vmexit_smi(guest_state, increment_rip);
	case 6: return vmexit_other_smi(guest_state, increment_rip);
	case 7: return vmexit_interrupt_window(guest_state, increment_rip);
	case 8: return vmexit_nmi_window(guest_state, increment_rip);
	case 9: return vmexit_task_switch(guest_state, increment_rip);
	case 10: return vmexit_cpuid(guest_state, increment_rip);
	case 11: return vmexit_getsec(guest_state, increment_rip);
	case 12: return vmexit_hlt(guest_state, increment_rip);
	case 13: return vmexit_invd(guest_state, increment_rip);
	case 14: return vmexit_invlpg(guest_state, increment_rip);
	case 15: return vmexit_rdpmc(guest_state, increment_rip);
	case 16: return vmexit_rdtsc(guest_state, increment_rip);
	case 17: return vmexit_rsm(guest_state, increment_rip);
	case 18: return vmexit_vmcall(guest_state, increment_rip);
	case 19: return vmexit_vmclear(guest_state, increment_rip);
	case 20: return vmexit_vmlaunch(guest_state, increment_rip);
	case 21: return vmexit_vmptrld(guest_state, increment_rip);
	case 22: return vmexit_vmptrst(guest_state, increment_rip);
	case 23: return vmexit_vmread(guest_state, increment_rip);
	case 24: return vmexit_vmresume(guest_state, increment_rip);
	case 25: return vmexit_vmwrite(guest_state, increment_rip);
	case 26: return vmexit_vmxoff(guest_state, increment_rip);
	case 27: return vmexit_vmxon(guest_state, increment_rip);
	case 28: return vmexit_cr_access(guest_state, increment_rip);
	case 29: return vmexit_mov_dr(guest_state, increment_rip);
	case 30: return vmexit_io_instruction(guest_state, increment_rip);
	case 31: return vmexit_rdmsr(guest_state, increment_rip);
	case 32: return vmexit_wrmsr(guest_state, increment_rip);
	case 33: return vmexit_vmentry_failure_invalid_guest_state(guest_state, increment_rip);
	case 34: return vmexit_vmentry_failure_msr_loading(guest_state, increment_rip);
	case 36: return vmexit_mwait(guest_state, increment_rip);
	case 37: return vmexit_monitor_trap_flag(guest_state, increment_rip);
	case 39: return vmexit_monitor(guest_state, increment_rip);
	case 40: return vmexit_pause(guest_state, increment_rip);
	case 41: return vmexit_vmentry_failure_machine_check_event(guest_state, increment_rip);
	case 43: return vmexit_tpr_below_threshold(guest_state, increment_rip);
	case 44: return vmexit_apic_access(guest_state, increment_rip);
	case 45: return vmexit_virtualized_eoi(guest_state, increment_rip);
	case 46: return vmexit_access_to_gdtr_or_idtr(guest_state, increment_rip);
	case 47: return vmexit_access_to_ldtr_or_tr(guest_state, increment_rip);
	case 48: return vmexit_ept_violation(guest_state, increment_rip);
	case 49: return vmexit_ept_misconfiguration(guest_state, increment_rip);
	case 50: return vmexit_invept(guest_state, increment_rip);
	case 51: return vmexit_rdtscp(guest_state, increment_rip);
	case 52: return vmexit_vmx_preemption_timer_expired(guest_state, increment_rip);
	case 53: return vmexit_invvpid(guest_state, increment_rip);
	case 54: return vmexit_wbinvd_or_wbnoinvd(guest_state, increment_rip);
	case 55: return vmexit_xsetbv(guest_state, increment_rip);
	case 56: return vmexit_apic_write(guest_state, increment_rip);
	case 57: return vmexit_rdrand(guest_state, increment_rip);
	case 58: return vmexit_invpcid(guest_state, increment_rip);
	case 59: return vmexit_vmfunc(guest_state, increment_rip);
	case 60: return vmexit_encls(guest_state, increment_rip);
	case 61: return vmexit_rdseed(guest_state, increment_rip);
	case 62: return vmexit_page_modification_log_full(guest_state, increment_rip);
	case 63: return vmexit_xsaves(guest_state, increment_rip);
	case 64: return vmexit_xrstors(guest_state, increment_rip);
	case 66: return vmexit_spp_related_event(guest_state, increment_rip);
	case 67: return vmexit_umwait(guest_state, increment_rip);
	case 68: return vmexit_tpause(guest_state, increment_rip);
	case 69: return vmexit_loadiwkey(guest_state, increment_rip);
	}
}

void ExitHandler::vmexit_exception_or_nmi_interrupt(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_external_interrupt(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_triple_fault(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_init_singal(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_sipi(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_smi(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_other_smi(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_interrupt_window(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_nmi_window(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_task_switch(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_cpuid(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_getsec(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_hlt(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invd(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invlpg(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdpmc(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdtsc(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rsm(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmcall(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmclear(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmlaunch(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmptrld(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmptrst(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmread(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmresume(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmwrite(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmxoff(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmxon(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_cr_access(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_mov_dr(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_io_instruction(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdmsr(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_wrmsr(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmentry_failure_invalid_guest_state(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmentry_failure_msr_loading(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_mwait(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_monitor_trap_flag(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_monitor(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_pause(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmentry_failure_machine_check_event(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_tpr_below_threshold(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_apic_access(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_virtualized_eoi(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_access_to_gdtr_or_idtr(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_access_to_ldtr_or_tr(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_ept_violation(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_ept_misconfiguration(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invept(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdtscp(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmx_preemption_timer_expired(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invvpid(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_wbinvd_or_wbnoinvd(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_xsetbv(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_apic_write(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdrand(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_invpcid(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_vmfunc(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_encls(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_rdseed(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_page_modification_log_full(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_xsaves(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_xrstors(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_spp_related_event(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_umwait(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_tpause(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}

void ExitHandler::vmexit_loadiwkey(guest_state_vmx guest_state, bool& increment_rip) noexcept
{

}                                  
