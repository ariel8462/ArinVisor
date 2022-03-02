.code

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

end
