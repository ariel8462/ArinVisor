#pragma once

#include "vcpu.h"

#pragma pack(push, 1)
struct guest_state_vmx
{
	unsigned long long rsp;
	unsigned long long rbp;
	unsigned long long rax;
	unsigned long long rbx;
	unsigned long long rcx;
	unsigned long long rdx;
	unsigned long long rsi;
	unsigned long long rdi;
	unsigned long long r8;
	unsigned long long r9;
	unsigned long long r10;
	unsigned long long r11;
	unsigned long long r12;
	unsigned long long r13;
	unsigned long long r14;
	unsigned long long r15;
};
#pragma pack(pop)

class ExitHandler
{
public:
	ExitHandler() = default;

	ExitHandler(const ExitHandler&) = delete;
	ExitHandler& operator=(const ExitHandler&) = delete;

	ExitHandler(const ExitHandler&&) = delete;
	ExitHandler& operator=(const ExitHandler&&) = delete;

public:
	void handle_vm_exit(unsigned long long exit_reason, VirtualCpu* vcpu, guest_state_vmx*& guest_state, bool& increment_rip) noexcept;

private:
	void vmexit_exception_or_nmi_interrupt(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_external_interrupt(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_triple_fault(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_init_singal(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_sipi(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_smi(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_other_smi(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_interrupt_window(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_nmi_window(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_task_switch(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_cpuid(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_getsec(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_hlt(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_invd(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_invlpg(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rdpmc(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rdtsc(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rsm(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmcall(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmclear(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmlaunch(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmptrld(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmptrst(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmread(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmresume(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmwrite(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmxoff(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmxon(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_cr_access(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_mov_dr(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_io_instruction(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rdmsr(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_wrmsr(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmentry_failure_invalid_guest_state(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmentry_failure_msr_loading(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_mwait(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_monitor_trap_flag(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_monitor(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_pause(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmentry_failure_machine_check_event(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_tpr_below_threshold(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_apic_access(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_virtualized_eoi(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_access_to_gdtr_or_idtr(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_access_to_ldtr_or_tr(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_ept_violation(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_ept_misconfiguration(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_invept(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rdtscp(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmx_preemption_timer_expired(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_invvpid(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_wbinvd_or_wbnoinvd(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_xsetbv(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_apic_write(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rdrand(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_invpcid(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_vmfunc(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_encls(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_rdseed(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_page_modification_log_full(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_xsaves(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_xrstors(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_spp_related_event(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_umwait(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_tpause(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
	void vmexit_loadiwkey(VirtualCpu* vcpu, guest_state_vmx* guest_state, bool& increment_rip) noexcept;
};
