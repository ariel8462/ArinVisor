#pragma once

#include "arch.h"

extern "C" unsigned short _read_ldtr();
extern "C" unsigned short _read_tr();
extern "C" unsigned short _read_cs();
extern "C" unsigned short _read_ds();
extern "C" unsigned short _read_ss();
extern "C" unsigned short _read_gs();
extern "C" unsigned short _read_fs();
extern "C" unsigned short _read_es();
extern "C" unsigned int _read_access_rights(unsigned short selector);
extern "C" void _enter_guest();
extern "C" void _vm_exit_handler();
extern "C" int _invept(int type, arch::InveptDescriptor* descriptor);
