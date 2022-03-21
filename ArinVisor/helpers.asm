.code

extern vm_exit_handler:proc
extern read_rip:proc
extern read_rsp:proc

_read_ldtr proc
	sldt ax
	ret
_read_ldtr endp

_read_tr proc
	str ax
	ret
_read_tr endp

_read_cs proc
	mov ax, cs
	ret
_read_cs endp

_read_ds proc
	mov ax, ds
	ret
_read_ds endp

_read_ss proc
	mov ax, ss
	ret
_read_ss endp

_read_gs proc
	mov ax, gs
	ret
_read_gs endp

_read_fs proc
	mov ax, fs
	ret
_read_fs endp

_read_es proc
	mov ax, es
	ret
_read_es endp

_read_access_rights proc
	lar eax, ecx
	ret
_read_access_rights endp

_enter_guest proc
	cli

	mov rcx, rsp
	sub rcx, 05FF8h
	
	movaps xmm0, [rcx+01a0h]
	movaps xmm1, [rcx+01b0h]
	movaps xmm2, [rcx+01c0h]
	movaps xmm3, [rcx+01d0h]
	movaps xmm4, [rcx+01e0h]
	movaps xmm5, [rcx+01f0h]
	movaps xmm6, [rcx+0200h]
	movaps xmm7, [rcx+0210h]
	movaps xmm8, [rcx+0220h]
	movaps xmm9, [rcx+0230h]
	movaps xmm10, [rcx+0240h]
	movaps xmm11, [rcx+0250h]
	movaps xmm12, [rcx+0260h]
	movaps xmm13, [rcx+0270h]
	movaps xmm14, [rcx+0280h]
	movaps xmm15, [rcx+0290h]

	push [rcx+044h]
	popfq

	mov rax, [rcx+078h]
	mov rdx, [rcx+088h]
	mov rbx, [rcx+090h]
	mov rbp, [rcx+0a0h]
	mov rsi, [rcx+0a8h]
	mov rdi, [rcx+0b0h]
	mov r8, [rcx+0b8h]
	mov r9, [rcx+0c0h]
	mov r10, [rcx+0c8h]
	mov r11, [rcx+0d0h]
	mov r12, [rcx+0d8h]
	mov r13, [rcx+0e0h]
	mov r14, [rcx+0e8h]
	mov r15, [rcx+0f0h]

	mov rsp, [rcx+098h]
	push [rcx+0f8h]
	mov rcx, [rcx+080h]

	sti
	ret
_enter_guest endp

_vm_exit_handler proc
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
	push rbp
	push rsp

	mov rcx, rsp
	sub rsp, 028h

	call vm_exit_handler

	add rsp, 028h

	cmp eax, 1
	je _vmxoff_handler

	pop rsp
	pop rbp
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	vmresume

	int 3 ; should not execute if 'vmresume' worked
	nop
_vm_exit_handler endp

_vmxoff_handler proc
	sub rsp, 028h
	call read_rsp
	add rsp, 028h

	mov [rsp+080h], rax
	
	sub rsp, 028h
	call read_rip
	add rsp, 028h

	mov rdx, rsp ; save rsp

	mov rbx, [rsp+080h]
	mov rsp, rbx
	push rax ; push the address from read_rip to the stack we got from read_rsp

	mov rsp, rdx ; restore rsp

	sub rbx, 8 ; because we pushed the rip address, need to substract 8 bytes from the stack (stack grows downwards)
	mov [rsp+080h], rbx

	pop rsp
	pop rbp
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	pop rsp
	ret
_vmxoff_handler endp

_invept proc
	invept rcx, oword ptr [rdx]
	
	jz failed ; failed valid
	jc failed ; failed invalid

	xor rax, rax
	ret

failed:
	mov rax, 1
	ret

_invept endp

end
