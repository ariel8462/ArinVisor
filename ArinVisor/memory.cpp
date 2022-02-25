#include "memory.h"

void* operator new(size_t size, POOL_TYPE type, ULONG tag)
{
	return ExAllocatePoolWithTag(type, size, tag);
}

void* operator new[](size_t size, POOL_TYPE type, ULONG tag)
{
	return ExAllocatePoolWithTag(type, size, tag);
}

void operator delete(void* address, unsigned __int64)
{
	ExFreePool(address);
}

void operator delete[](void* address, unsigned __int64)
{
	ExFreePool(address);
}
