#pragma once

#include <ntddk.h>

void* operator new(size_t size, POOL_TYPE type, ULONG tag);
void* operator new[](size_t size, POOL_TYPE type, ULONG tag);

void operator delete(void* address, unsigned __int64);
void operator delete[](void* address, unsigned __int64);
