#include "arch.h"
#include "helpers.h"

unsigned int arch::get_segment_access_rights(unsigned short segment_selector)
{
	SegmentSelector selector = { 0 };
	SegmentAccessRights access_rights = { 0 };

	selector.raw = segment_selector;

	if (selector.bits.index == 0 && selector.bits.table_indicator == 0)
	{
		access_rights.bits.unusable = true;
	}
	else
	{
		access_rights.raw = (_read_access_rights(segment_selector) >> 8);
	}

	return access_rights.raw;
}

unsigned long long arch::get_segment_base(unsigned long long gdt_base, unsigned short segment_selector)
{
	SegmentSelector selector = { 0 };
	unsigned long long segment_address = 0;

	selector.raw = segment_selector;

	if (selector.bits.index == 0 && selector.bits.table_indicator == 0)
	{
		return 0;
	}

	auto index = segment_selector >> 3;
	auto segment_descriptor = reinterpret_cast<SegmentDescriptor*>(gdt_base + index * sizeof(SegmentDescriptor));
	
	if (!segment_descriptor->bits.system)
	{
		auto system_segment_descriptor = reinterpret_cast<SystemSegmentDescriptor*>(segment_descriptor);

		segment_address = static_cast<unsigned long long>(system_segment_descriptor->base_address_low);
		segment_address |= static_cast<unsigned long long>(system_segment_descriptor->bits.base_address_mid) << 16;
		segment_address |= static_cast<unsigned long long>(system_segment_descriptor->bits.base_address_high) << 24;
		segment_address |= static_cast<unsigned long long>(system_segment_descriptor->base_upper) << 32;
	}
	else
	{
		segment_address = static_cast<unsigned long long>(segment_descriptor->base_address_low);
		segment_address |= static_cast<unsigned long long>(segment_descriptor->bits.base_address_mid) << 16;
		segment_address |= static_cast<unsigned long long>(segment_descriptor->bits.base_address_high) << 24;
	}

	return segment_address;
}
