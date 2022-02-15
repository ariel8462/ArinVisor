#pragma once

namespace arch
{
	enum class Msr : unsigned long
	{
		IA32_FEATURE_CONTROL = 0x3a
	};

	struct cpu_features
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

	union feature_control_msr
	{
		struct
		{
			unsigned long long lock : 1;
			unsigned long long enable_smx : 1;
			unsigned long long enable_vmx : 1;
		};

		unsigned long long raw;
	};
}