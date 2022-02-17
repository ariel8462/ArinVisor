#pragma once

namespace arch
{
	enum class Msr : unsigned long
	{
		IA32_FEATURE_CONTROL = 0x3a,
		IA32_LSTAR = 0xc0000082,
		IA32_VMX_BASIC = 0x480,
		IA32_VMX_CR0_FIXED0 = 0x486,
		IA32_VMX_CR0_FIXED1 = 0x487,
		IA32_VMX_CR4_FIXED0 = 0x488,
		IA32_VMX_CR4_FIXED1 = 0x489
	};

	union Cr4
	{
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
		};

		unsigned long long raw;
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
		struct
		{
			unsigned long long lock : 1;
			unsigned long long enable_smx : 1;
			unsigned long long enable_vmx : 1;
		};

		unsigned long long raw;
	};
	
	constexpr unsigned int VMX_BASIC_MSR_SIZE = 0x1000;

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
		};
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
		char data[0x1000 - 2 * sizeof(unsigned int)];
	};
}
