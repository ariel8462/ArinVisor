#pragma once

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
	void handle_vm_exit(unsigned long long exit_reason, guest_state_vmx guest_state, bool& increment_rip) noexcept;

private:
	void vmexit_exception_or_nmi_interrupt(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_external_interrupt(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_triple_fault(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_init_singal(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_sipi(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_smi(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_other_smi(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_interrupt_window(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_nmi_window(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_task_switch(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_cpuid(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_getsec(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_hlt(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_invd(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_invlpg(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rdpmc(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rdtsc(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rsm(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmcall(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmclear(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmlaunch(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmptrld(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmptrst(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmread(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmresume(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmwrite(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmxoff(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmxon(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_cr_access(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_mov_dr(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_io_instruction(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rdmsr(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_wrmsr(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmentry_failure_invalid_guest_state(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmentry_failure_msr_loading(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_mwait(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_monitor_trap_flag(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_monitor(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_pause(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmentry_failure_machine_check_event(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_tpr_below_threshold(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_apic_access(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_virtualized_eoi(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_access_to_gdtr_or_idtr(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_access_to_ldtr_or_tr(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_ept_violation(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_ept_misconfiguration(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_invept(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rdtscp(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmx_preemption_timer_expired(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_invvpid(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_wbinvd_or_wbnoinvd(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_xsetbv(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_apic_write(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rdrand(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_invpcid(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_vmfunc(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_encls(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_rdseed(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_page_modification_log_full(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_xsaves(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_xrstors(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_spp_related_event(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_umwait(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_tpause(guest_state_vmx guest_state, bool& increment_rip) noexcept;
	void vmexit_loadiwkey(guest_state_vmx guest_state, bool& increment_rip) noexcept;
};
