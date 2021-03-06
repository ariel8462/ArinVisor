#pragma once
#pragma pack(push, 1)

constexpr unsigned long kTag = 'nira';

extern "C" void _sgdt(void*);

namespace arch
{
	enum class Msr : unsigned long
	{
		IA32_FEATURE_CONTROL = 0x3a,
		IA32_VMX_BASIC = 0x480,
		IA32_VMX_CR0_FIXED0 = 0x486,
		IA32_VMX_CR0_FIXED1 = 0x487,
		IA32_VMX_CR4_FIXED0 = 0x488,
		IA32_VMX_CR4_FIXED1 = 0x489,
		IA32_DEBUGCTL = 0x1d9,
		IA32_SYSENTER_CS = 0x174,
		IA32_SYSENTER_ESP = 0x175,
		IA32_SYSENTER_EIP = 0x176,
		IA32_EFER = 0xc0000080,
		IA32_FS_BASE = 0xc0000100,
		IA32_GS_BASE = 0xc0000101,
		IA32_VMX_PINBASED_CTLS = 0x481,
		IA32_VMX_TRUE_PINBASED_CTLS = 0x48d,
		IA32_VMX_PROCBASED_CTLS = 0x482,
		IA32_VMX_TRUE_PROCBASED_CTLS = 0x48e,
		IA32_VMX_PROCBASED_CTLS2 = 0x48b,
		IA32_VMX_ENTRY_CTLS = 0x484,
		IA32_VMX_TRUE_ENTRY_CTLS= 0x490,
		IA32_VMX_EXIT_CTLS = 0x483,
		IA32_VMX_TRUE_EXIT_CTLS = 0x48f
	};

	union Cr0
	{
		unsigned long long raw;

		struct
		{
			unsigned long long pe : 1;
			unsigned long long mp : 1;
			unsigned long long em : 1;
			unsigned long long ts : 1;
			unsigned long long et : 1;
			unsigned long long ne : 1;
			unsigned long long reserved1 : 10;
			unsigned long long wp : 1;
			unsigned long long reserved2 : 1;
			unsigned long long am : 1;
			unsigned long long reserved3 : 10;
			unsigned long long nw : 1;
			unsigned long long cd : 1;
			unsigned long long pg : 1;
			unsigned long long reserved4 : 32;
		} bits;
	};

	union Cr4
	{
		unsigned long long raw;

		struct
		{
			unsigned long long vme : 1;
			unsigned long long pvi : 1;
			unsigned long long tsd : 1;
			unsigned long long de : 1;
			unsigned long long pse : 1;
			unsigned long long pae : 1;
			unsigned long long mce : 1;
			unsigned long long pge : 1;
			unsigned long long pce : 1;
			unsigned long long osfxsr : 1;
			unsigned long long osxmmexcpt : 1;
			unsigned long long umip : 1;
			unsigned long long reserved1 : 1;
			unsigned long long vmxe : 1;
			unsigned long long smxe : 1;
			unsigned long long reserved2 : 1;
			unsigned long long fsgsbase : 1;
			unsigned long long pcide : 1;
			unsigned long long osxsave : 1;
			unsigned long long reserved3 : 1;
			unsigned long long smep : 1;
			unsigned long long smap : 1;
			unsigned long long pke : 1;
			unsigned long long reserved4 : 41;
		} bits;
	};

	struct CpuFeatures
	{
		union
		{
			unsigned int raw;
		} eax;

		union
		{
			unsigned int raw;
		} ebx;

		union
		{
			struct
			{
				unsigned int sse3 : 1;
				unsigned int pclmulqdq : 1;
				unsigned int dtes64 : 1;
				unsigned int monitor : 1;
				unsigned int ds_cpl : 1;
				unsigned int vmx : 1;
				unsigned int smx : 1;
				unsigned int eist : 1;
				unsigned int tm2 : 1;
				unsigned int ssse3 : 1;
				unsigned int cnxt_id : 1;
				unsigned int sdbg : 1;
				unsigned int fma : 1;
				unsigned int cmpxchg16b : 1;
				unsigned int xtpr : 1;
				unsigned int pdcm : 1;
				unsigned int reserved : 1;
				unsigned int pcid : 1;
				unsigned int dca : 1;
				unsigned int sse4_1 : 1;
				unsigned int sse4_2 : 1;
				unsigned int x2apis : 1;
				unsigned int movbe : 1;
				unsigned int popcnt : 1;
				unsigned int tsc_deadline : 1;
				unsigned int aesni : 1;
				unsigned int xsave : 1;
				unsigned int sxsave : 1;
				unsigned int avx : 1;
				unsigned int f16c : 1;
				unsigned int rdrand : 1;
				unsigned int not_used : 1;
			};

			unsigned int raw;
		} ecx;

		union
		{
			struct
			{
				unsigned int fpu : 1;
				unsigned int vme : 1;
				unsigned int de : 1;
				unsigned int pse : 1;
				unsigned int tsc : 1;
				unsigned int msr : 1;
				unsigned int pae : 1;
				unsigned int mce : 1;
				unsigned int cx8 : 1;
				unsigned int apic : 1;
				unsigned int reserved1 : 1;
				unsigned int sep : 1;
				unsigned int mtrr : 1;
				unsigned int pge : 1;
				unsigned int mca : 1;
				unsigned int cmov : 1;
				unsigned int pat : 1;
				unsigned int pse_36 : 1;
				unsigned int psn : 1;
				unsigned int clfsh : 1;
				unsigned int reserved2 : 1;
				unsigned int ds : 1;
				unsigned int acpi : 1;
				unsigned int mmx : 1;
				unsigned int fxsr : 1;
				unsigned int sse : 1;
				unsigned int sse2 : 1;
				unsigned int ss : 1;
				unsigned int htt : 1;
				unsigned int tm : 1;
				unsigned int reserved3 : 1;
				unsigned int pbe : 1;
			};

			unsigned int raw;
		} edx;
	};

	union FeatureControlMsr
	{
		unsigned long long raw;

		struct
		{
			unsigned long long lock : 1;
			unsigned long long enable_vmx_in_smx : 1;
			unsigned long long enable_vmx : 1;
		} bits;
	};
	
	constexpr unsigned int VMX_REGION_SIZE = 0x1000;

	union Ia32VmxBasicMsr
	{
		unsigned long long raw;

		struct
		{
			unsigned long long revision_identifier : 31;
			unsigned long long reserved1 : 1;
			unsigned long long region_size : 12;
			unsigned long long region_clear : 1;
			unsigned long long reserved2 : 3;
			unsigned long long ia64_supported : 1;
			unsigned long long supported_dual_monitor : 1;
			unsigned long long memory_type : 4;
			unsigned long long vmexit_repot : 1;
			unsigned long long vmx_capability_hint : 1;
			unsigned long long reserved3 : 8;
		} bits;
	};

	struct VmmRegions
	{
		union
		{
			struct
			{
				unsigned int revision_identifier : 31;
				unsigned int shadow_vmcs_indicator : 1;
			} bits;

			unsigned int raw;
		} header;

		unsigned int abort_indicator;
		char data[VMX_REGION_SIZE - 2 * sizeof(unsigned int)];
	};

	constexpr int CPUID_BASIC_INFO = 0x0;
	constexpr int CPUID_VERSION_INFO = 0x1;

	enum class VmcsFields : unsigned long
	{
		VMCS_CTRL_VIRTUAL_PROCESSOR_IDENTIFIER = 0x0000,
		VMCS_CTRL_POSTED_INTERRUPT_NOTIFICATION_VECTOR = 0x0002,
		VMCS_CTRL_EPTP_INDEX = 0x0004,
		VMCS_GUEST_ES_SELECTOR = 0x0800,
		VMCS_GUEST_CS_SELECTOR = 0x0802,
		VMCS_GUEST_SS_SELECTOR = 0x0804,
		VMCS_GUEST_DS_SELECTOR = 0x0806,
		VMCS_GUEST_FS_SELECTOR = 0x0808,
		VMCS_GUEST_GS_SELECTOR = 0x080a,
		VMCS_GUEST_LDTR_SELECTOR = 0x080c,
		VMCS_GUEST_TR_SELECTOR = 0x080e,
		VMCS_GUEST_INTERRUPT_STATUS = 0x0810,
		VMCS_GUEST_PML_INDEX = 0x0812,
		VMCS_HOST_ES_SELECTOR = 0x0c00,
		VMCS_HOST_CS_SELECTOR = 0x0c02,
		VMCS_HOST_SS_SELECTOR = 0x0c04,
		VMCS_HOST_DS_SELECTOR = 0x0c06,
		VMCS_HOST_FS_SELECTOR = 0x0c08,
		VMCS_HOST_GS_SELECTOR = 0x0c0a,
		VMCS_HOST_TR_SELECTOR = 0x0c0c,
		VMCS_CTRL_IO_BITMAP_A_ADDRESS = 0x2000,
		VMCS_CTRL_IO_BITMAP_B_ADDRESS = 0x2002,
		VMCS_CTRL_MSR_BITMAP_ADDRESS = 0x2004,
		VMCS_CTRL_VMEXIT_MSR_STORE_ADDRESS = 0x2006,
		VMCS_CTRL_VMEXIT_MSR_LOAD_ADDRESS = 0x2008,
		VMCS_CTRL_VMENTRY_MSR_LOAD_ADDRESS = 0x200a,
		VMCS_CTRL_EXECUTIVE_VMCS_POINTER = 0x200c,
		VMCS_CTRL_PML_ADDRESS = 0x200e,
		VMCS_CTRL_TSC_OFFSET = 0x2010,
		VMCS_CTRL_VIRTUAL_APIC_ADDRESS = 0x2012,
		VMCS_CTRL_APIC_ACCESS_ADDRESS = 0x2014,
		VMCS_CTRL_POSTED_INTERRUPT_DESCRIPTOR_ADDRESS = 0x2016,
		VMCS_CTRL_VMFUNC_CONTROLS = 0x2018,
		VMCS_CTRL_EPT_POINTER = 0x201a,
		VMCS_CTRL_EOI_EXIT_BITMAP_0 = 0x201c,
		VMCS_CTRL_EOI_EXIT_BITMAP_1 = 0x201e,
		VMCS_CTRL_EOI_EXIT_BITMAP_2 = 0x2020,
		VMCS_CTRL_EOI_EXIT_BITMAP_3 = 0x2022,
		VMCS_CTRL_EPT_POINTER_LIST_ADDRESS = 0x2024,
		VMCS_CTRL_VMREAD_BITMAP_ADDRESS = 0x2026,
		VMCS_CTRL_VMWRITE_BITMAP_ADDRESS = 0x2028,
		VMCS_CTRL_VIRTUALIZATION_EXCEPTION_INFORMATION_ADDRESS = 0x202a,
		VMCS_CTRL_XSS_EXITING_BITMAP = 0x202c,
		VMCS_CTRL_ENCLS_EXITING_BITMAP = 0x202e,
		VMCS_CTRL_TSC_MULTIPLIER = 0x2032,
		VMCS_GUEST_PHYSICAL_ADDRESS = 0x2400,
		VMCS_GUEST_VMCS_LINK_POINTER = 0x2800,
		VMCS_GUEST_DEBUGCTL = 0x2802,
		VMCS_GUEST_PAT = 0x2804,
		VMCS_GUEST_EFER = 0x2806,
		VMCS_GUEST_PERF_GLOBAL_CTRL = 0x2808,
		VMCS_GUEST_PDPTE0 = 0x280a,
		VMCS_GUEST_PDPTE1 = 0x280c,
		VMCS_GUEST_PDPTE2 = 0x280e,
		VMCS_GUEST_PDPTE3 = 0x2810,
		VMCS_HOST_PAT = 0x2c00,
		VMCS_HOST_EFER = 0x2c02,
		VMCS_HOST_PERF_GLOBAL_CTRL = 0x2c04,
		VMCS_CTRL_PIN_BASED_VM_EXECUTION_CONTROLS = 0x4000,
		VMCS_CTRL_PROCESSOR_BASED_VM_EXECUTION_CONTROLS = 0x4002,
		VMCS_CTRL_EXCEPTION_BITMAP = 0x4004,
		VMCS_CTRL_PAGEFAULT_ERROR_CODE_MASK = 0x4006,
		VMCS_CTRL_PAGEFAULT_ERROR_CODE_MATCH = 0x4008,
		VMCS_CTRL_CR3_TARGET_COUNT = 0x400a,
		VMCS_CTRL_VMEXIT_CONTROLS = 0x400c,
		VMCS_CTRL_VMEXIT_MSR_STORE_COUNT = 0x400e,
		VMCS_CTRL_VMEXIT_MSR_LOAD_COUNT = 0x4010,
		VMCS_CTRL_VMENTRY_CONTROLS = 0x4012,
		VMCS_CTRL_VMENTRY_MSR_LOAD_COUNT = 0x4014,
		VMCS_CTRL_VMENTRY_INTERRUPTION_INFORMATION_FIELD = 0x4016,
		VMCS_CTRL_VMENTRY_EXCEPTION_ERROR_CODE = 0x4018,
		VMCS_CTRL_VMENTRY_INSTRUCTION_LENGTH = 0x401a,
		VMCS_CTRL_TPR_THRESHOLD = 0x401c,
		VMCS_CTRL_SECONDARY_PROCESSOR_BASED_VM_EXECUTION_CONTROLS = 0x401e,
		VMCS_CTRL_PLE_GAP = 0x4020,
		VMCS_CTRL_PLE_WINDOW = 0x4022,
		VMCS_VM_INSTRUCTION_ERROR = 0x4400,
		VMCS_EXIT_REASON = 0x4402,
		VMCS_VMEXIT_INTERRUPTION_INFORMATION = 0x4404,
		VMCS_VMEXIT_INTERRUPTION_ERROR_CODE = 0x4406,
		VMCS_IDT_VECTORING_INFORMATION = 0x4408,
		VMCS_IDT_VECTORING_ERROR_CODE = 0x440a,
		VMCS_VMEXIT_INSTRUCTION_LENGTH = 0x440c,
		VMCS_VMEXIT_INSTRUCTION_INFO = 0x440e,
		VMCS_GUEST_ES_LIMIT = 0x4800,
		VMCS_GUEST_CS_LIMIT = 0x4802,
		VMCS_GUEST_SS_LIMIT = 0x4804,
		VMCS_GUEST_DS_LIMIT = 0x4806,
		VMCS_GUEST_FS_LIMIT = 0x4808,
		VMCS_GUEST_GS_LIMIT = 0x480a,
		VMCS_GUEST_LDTR_LIMIT = 0x480c,
		VMCS_GUEST_TR_LIMIT = 0x480e,
		VMCS_GUEST_GDTR_LIMIT = 0x4810,
		VMCS_GUEST_IDTR_LIMIT = 0x4812,
		VMCS_GUEST_ES_ACCESS_RIGHTS = 0x4814,
		VMCS_GUEST_CS_ACCESS_RIGHTS = 0x4816,
		VMCS_GUEST_SS_ACCESS_RIGHTS = 0x4818,
		VMCS_GUEST_DS_ACCESS_RIGHTS = 0x481a,
		VMCS_GUEST_FS_ACCESS_RIGHTS = 0x481c,
		VMCS_GUEST_GS_ACCESS_RIGHTS = 0x481e,
		VMCS_GUEST_LDTR_ACCESS_RIGHTS = 0x4820,
		VMCS_GUEST_TR_ACCESS_RIGHTS = 0x4822,
		VMCS_GUEST_INTERRUPTIBILITY_STATE = 0x4824,
		VMCS_GUEST_ACTIVITY_STATE = 0x4826,
		VMCS_GUEST_SMBASE = 0x4828,
		VMCS_GUEST_SYSENTER_CS = 0x482a,
		VMCS_GUEST_VMX_PREEMPTION_TIMER_VALUE = 0x482e,
		VMCS_HOST_SYSENTER_CS = 0x4c00,
		VMCS_CTRL_CR0_GUEST_HOST_MASK = 0x6000,
		VMCS_CTRL_CR4_GUEST_HOST_MASK = 0x6002,
		VMCS_CTRL_CR0_READ_SHADOW = 0x6004,
		VMCS_CTRL_CR4_READ_SHADOW = 0x6006,
		VMCS_CTRL_CR3_TARGET_VALUE_0 = 0x6008,
		VMCS_CTRL_CR3_TARGET_VALUE_1 = 0x600a,
		VMCS_CTRL_CR3_TARGET_VALUE_2 = 0x600c,
		VMCS_CTRL_CR3_TARGET_VALUE_3 = 0x600e,
		VMCS_EXIT_QUALIFICATION = 0x6400,
		VMCS_IO_RCX = 0x6402,
		VMCS_IO_RSX = 0x6404,
		VMCS_IO_RDI = 0x6406,
		VMCS_IO_RIP = 0x6408,
		VMCS_EXIT_GUEST_LINEAR_ADDRESS = 0x640a,
		VMCS_GUEST_CR0 = 0x6800,
		VMCS_GUEST_CR3 = 0x6802,
		VMCS_GUEST_CR4 = 0x6804,
		VMCS_GUEST_ES_BASE = 0x6806,
		VMCS_GUEST_CS_BASE = 0x6808,
		VMCS_GUEST_SS_BASE = 0x680a,
		VMCS_GUEST_DS_BASE = 0x680c,
		VMCS_GUEST_FS_BASE = 0x680e,
		VMCS_GUEST_GS_BASE = 0x6810,
		VMCS_GUEST_LDTR_BASE = 0x6812,
		VMCS_GUEST_TR_BASE = 0x6814,
		VMCS_GUEST_GDTR_BASE = 0x6816,
		VMCS_GUEST_IDTR_BASE = 0x6818,
		VMCS_GUEST_DR7 = 0x681a,
		VMCS_GUEST_RSP = 0x681c,
		VMCS_GUEST_RIP = 0x681e,
		VMCS_GUEST_RFLAGS = 0x6820,
		VMCS_GUEST_PENDING_DEBUG_EXCEPTIONS = 0x6822,
		VMCS_GUEST_SYSENTER_ESP = 0x6824,
		VMCS_GUEST_SYSENTER_EIP = 0x6826,
		VMCS_HOST_CR0 = 0x6c00,
		VMCS_HOST_CR3 = 0x6c02,
		VMCS_HOST_CR4 = 0x6c04,
		VMCS_HOST_FS_BASE = 0x6c06,
		VMCS_HOST_GS_BASE = 0x6c08,
		VMCS_HOST_TR_BASE = 0x6c0a,
		VMCS_HOST_GDTR_BASE = 0x6c0c,
		VMCS_HOST_IDTR_BASE = 0x6c0e,
		VMCS_HOST_SYSENTER_ESP = 0x6c10,
		VMCS_HOST_SYSENTER_EIP = 0x6c12,
		VMCS_HOST_RIP = 0x6c16,
		VMCS_HOST_RSP = 0x6c14
	};

	union PinBasedVmExecutionControl
	{
		unsigned int raw;

		struct
		{
			unsigned int external_interrupt_exiting : 1;
			unsigned int reserved1 : 2;
			unsigned int nmi_exiting : 1;
			unsigned int reserved2 : 1;
			unsigned int virtual_nmis : 1;
			unsigned int activate_vmx_preemption_timer : 1;
			unsigned int process_posted_interrupts : 1;
			unsigned int reserved3 : 24;
		} bits;
	};

	union ProcessorBasedVmExecutionControl
	{
		unsigned int raw;

		struct
		{
			unsigned int reserved1 : 2;
			unsigned int interrupt_window_exiting : 1;
			unsigned int use_tsc_offseting : 1;
			unsigned int reserved2 : 3;
			unsigned int hlt_exiting : 1;
			unsigned int reserved3 : 1;
			unsigned int invlpg_exiting : 1;
			unsigned int mwait_exiting : 1;
			unsigned int rdpmc_exiting : 1;
			unsigned int rdtsc_exiting : 1;
			unsigned int reserved4 : 2;
			unsigned int cr3_load_exiting : 1;
			unsigned int cr3_store_exiting : 1;
			unsigned int reserved5 : 2;
			unsigned int cr8_load_exiting : 1;
			unsigned int cr8_store_exiting : 1;
			unsigned int use_tpr_shadow : 1;
			unsigned int nmi_window_exiting : 1;
			unsigned int mov_dr_exiting : 1;
			unsigned int unconditional_io_exiting : 1;
			unsigned int use_io_bitmaps : 1;
			unsigned int reserved6 : 1;
			unsigned int monitor_trap_flag : 1;
			unsigned int use_msr_bitmaps : 1;
			unsigned int monitor_exiting : 1;
			unsigned int pause_exiting : 1;
			unsigned int activate_secondary_controls : 1;
		} bits;
	};

	union SecondaryProcessorBasedVmExecutionControl
	{
		unsigned int raw;

		struct
		{
			unsigned int virtualize_apic_accesses : 1;
			unsigned int enable_ept : 1;
			unsigned int descriptor_table_exiting : 1;
			unsigned int enable_rdtscp : 1;
			unsigned int virtualize_x2_apic_mode : 1;
			unsigned int enable_vpid : 1;
			unsigned int wbinvd_exiting : 1;
			unsigned int unrestricted_guest : 1;
			unsigned int apic_register_virtualization : 1;
			unsigned int virtual_interrupt_delivery : 1;
			unsigned int pause_loop_exiting : 1;
			unsigned int rdrand_exiting : 1;
			unsigned int enable_invpcid : 1;
			unsigned int enable_vm_functions : 1;
			unsigned int vmcs_shadowing : 1;
			unsigned int enable_encls_exiting : 1;
			unsigned int rdseed_exiting : 1;
			unsigned int enable_pml : 1;
			unsigned int ept_violation_virtualization_exception : 1;
			unsigned int conceal_vmx_non_root : 1;
			unsigned int enable_xsave_xrstors : 1;
			unsigned int reserved1 : 1;
			unsigned int mode_based_execute_control_for_ept : 1;
			unsigned int reserved2 : 2;
			unsigned int use_tsc_scaling : 1;
			unsigned int reserved3 : 6;
		} bits;
	};

	union VmEntryControlField
	{
		unsigned int raw;

		struct
		{
			unsigned int reserved1 : 2;
			unsigned int load_debug_controls : 1;
			unsigned int reserved2 : 6;
			unsigned int ia32e_mode_guest : 1;
			unsigned int entry_to_smm : 1;
			unsigned int deactivate_dual_monitor_treatment : 1;
			unsigned int reserved3 : 1;
			unsigned int load_ia32_perf_global_ctrl : 1;
			unsigned int load_ia32_pat : 1;
			unsigned int load_ia32_efer : 1;
			unsigned int load_ia32_bndcfgs : 1;
			unsigned int conceal_vm_entries : 1;
			unsigned int reserved4 : 14;
		} bits;
	};

	union VmExitControlField
	{
		unsigned int raw;

		struct
		{
			unsigned int reserved1 : 2;
			unsigned int save_debug_controls : 1;
			unsigned int reserved2 : 6;
			unsigned int host_address_space_size : 1;
			unsigned int reserved3 : 2;
			unsigned int load_ia32_perf_global_ctrl : 1;
			unsigned int reserved4 : 2;
			unsigned int acknowledge_interrupt_on_exit : 1;
			unsigned int reserved5 : 2;
			unsigned int save_ia32_pat : 1;
			unsigned int load_ia32_pat : 1;
			unsigned int save_ia32_efer : 1;
			unsigned int load_ia32_efer : 1;
			unsigned int save_vmx_preemption_timer_value : 1;
			unsigned int clear_ia32_bndcfgs : 1;
			unsigned int conceal_vmx_exits : 1;
			unsigned int reserved6 : 7;
		} bits;
	};

	struct DescriptorTable
	{
		unsigned short limit;
		unsigned long long base;
	};

	struct SegmentDescriptor
	{
		unsigned short segment_limit_low;
		unsigned short base_address_low;

		union
		{
			struct
			{
				unsigned int base_address_mid : 8;
				unsigned int type : 4;
				unsigned int system : 1;
				unsigned int dpl : 2;
				unsigned int present : 1;
				unsigned int segment_limit_high : 4;
				unsigned int available : 1;
				unsigned int long_mode : 1;
				unsigned int default_size : 1;
				unsigned int granularity : 1;
				unsigned int base_address_high : 8;
			} bits;

			unsigned int flags_raw;
		};
	};

	struct SystemSegmentDescriptor
	{
		unsigned short segment_limit_low;
		unsigned short base_address_low;

		union
		{
			struct
			{
				unsigned int base_address_mid : 8;
				unsigned int type : 4;
				unsigned int system : 1;
				unsigned int dpl : 2;
				unsigned int present : 1;
				unsigned int segment_limit_high : 4;
				unsigned int available : 1;
				unsigned int long_mode : 1;
				unsigned int default_size : 1;
				unsigned int granularity : 1;
				unsigned int base_address_high : 8;
			} bits;

			unsigned int flags_raw;
		};

		unsigned int base_upper;
		unsigned int reserved;
	};

	union SegmentSelector
	{
		unsigned short raw;

		struct
		{
			unsigned short rpl : 2;
			unsigned short table_indicator : 1;
			unsigned short index : 13;
		} bits;
	};

	union SegmentAccessRights
	{
		unsigned int raw;

		struct
		{
			unsigned int type : 4;
			unsigned int system : 1;
			unsigned int dpl : 2;
			unsigned int present : 1;
			unsigned int reserved1 : 4;
			unsigned int available : 1;
			unsigned int long_mode : 1;
			unsigned int default_size : 1;
			unsigned int granularity : 1;
			unsigned int unusable : 1;
			unsigned int reserved2 : 15;
		} bits;
	};

	unsigned int get_segment_access_rights(unsigned short segment_selector);
	unsigned long long get_segment_base(unsigned long long gdt_base, unsigned short segment_selector);

	union ControlSetting
	{
		unsigned long long raw;

		struct
		{
			unsigned long long low_part : 32;
			unsigned long long high_part : 32;
		};
	};

	union Eptp
	{
		unsigned long long raw;

		struct
		{
			unsigned long long memory_type : 3;
			unsigned long long walk_length : 3;
			unsigned long long enable_access_dirty : 1;
			unsigned long long reserved1 : 5;
			unsigned long long pml4 : 36;
			unsigned long long reserved2 : 16;
		} bits;
	};

	union EptPml4e
	{
		unsigned long long raw;

		struct
		{
			unsigned long long read_access : 1;
			unsigned long long write_access : 1;
			unsigned long long execute_access : 1; // if 'mode-based execute control for EPT' is 1 it acts like SMEP
			unsigned long long reserved1 : 5;
			unsigned long long accessed : 1; // only if 6th bit of eptp is 1, else this field is ignored
			unsigned long long ignored1 : 1;
			unsigned long long execute_access_from_user_mode : 1; // only if 'mode-based execute control for EPT' is 1
			unsigned long long ignored2 : 1;
			unsigned long long pfn : 36;
			unsigned long long reserved2 : 4;
			unsigned long long ignored3 : 12;
		} bits;
	};

	union EptPdpte
	{
		unsigned long long raw;

		struct
		{
			unsigned long long read_access : 1;
			unsigned long long write_access : 1;
			unsigned long long execute_access : 1; // if 'mode-based execute control for EPT' is 1 it acts like SMEP
			unsigned long long reserved1 : 5;
			unsigned long long accessed : 1; // only if 6th bit of eptp is 1, else this field is ignored
			unsigned long long ignored1 : 1;
			unsigned long long execute_access_from_user_mode : 1; // only if 'mode-based execute control for EPT' is 1
			unsigned long long ignored2 : 1;
			unsigned long long pfn : 36;
			unsigned long long reserved2 : 4;
			unsigned long long ignored3 : 12;
		} bits;
	};

	union EptLargePde
	{
		unsigned long long raw;

		struct
		{
			unsigned long long read_access : 1;
			unsigned long long write_access : 1;
			unsigned long long execute_access : 1; // if 'mode-based execute control for EPT' is 1 it acts like SMEP
			unsigned long long memory_type : 3;
			unsigned long long ignore_pat : 1;
			unsigned long long large_page : 1; // must be 1
			unsigned long long accessed : 1; // only if 6th bit of eptp is 1, else this field is ignored
			unsigned long long dirty : 1;
			unsigned long long execute_access_from_user_mode : 1; // only if 'mode-based execute control for EPT' is 1
			unsigned long long ignored1 : 1;
			unsigned long long reserved1 : 9;
			unsigned long long pfn : 27;
			unsigned long long reserved2 : 4;
			unsigned long long ignored2 : 11;
			unsigned long long supress_ve : 1;
		} bits;
	};

	union EptPde
	{
		unsigned long long raw;

		struct
		{
			unsigned long long read_access : 1;
			unsigned long long write_access : 1;
			unsigned long long execute_access : 1; // if 'mode-based execute control for EPT' is 1 it acts like SMEP
			unsigned long long reserved1 : 4;
			unsigned long long large_page : 1; // must be 0
			unsigned long long accessed : 1; // only if 6th bit of eptp is 1, else this field is ignored
			unsigned long long ignored1 : 1;
			unsigned long long execute_access_from_user_mode : 1; // only if 'mode-based execute control for EPT' is 1
			unsigned long long ignored2 : 1;
			unsigned long long pfn : 36;
			unsigned long long reserved2 : 4;
			unsigned long long ignored3 : 12;
		} bits;
	};

	union EptPte
	{
		unsigned long long raw;

		struct
		{
			unsigned long long read_access : 1;
			unsigned long long write_access : 1;
			unsigned long long execute_access : 1; // if 'mode-based execute control for EPT' is 1 it acts like SMEP
			unsigned long long memory_type : 3;
			unsigned long long ignore_pat : 1;
			unsigned long long ignored1 : 1;
			unsigned long long accessed : 1; // only if 6th bit of eptp is 1, else this field is ignored
			unsigned long long dirty : 1; // only if 6th bit of eptp is 1, else this field is ignored
			unsigned long long execute_access_from_user_mode : 1; // only if 'mode-based execute control for EPT' is 1
			unsigned long long ignored2 : 1;
			unsigned long long pfn : 36;
			unsigned long long reserved1 : 4;
			unsigned long long ignored3 : 11;
			unsigned long long supress_ve : 1;
		} bits;
	};

	struct InveptDescriptor
	{
		unsigned long long eptp;
		unsigned long long reserved;
	};
}
#pragma pack(pop)
