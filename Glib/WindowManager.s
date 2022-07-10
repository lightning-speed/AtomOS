	.file	"WindowManager.cpp"
	.text
	.align 16
	.globl	_ZN13WindowManager7repaintEP8window_t
	.type	_ZN13WindowManager7repaintEP8window_t, @function
_ZN13WindowManager7repaintEP8window_t:
.LFB0:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %eax
	movl	4(%eax), %ecx
	imull	(%eax), %ecx
	testl	%ecx, %ecx
	je	.L1
	movl	_ZN2FB4addrE, %esi
	movl	16(%eax), %ebx
	xorl	%eax, %eax
	.align 16
.L4:
	movl	(%ebx,%eax,4), %edx
	movl	%edx, (%esi,%eax,4)
	addl	$1, %eax
	cmpl	%ecx, %eax
	jne	.L4
.L1:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE0:
	.size	_ZN13WindowManager7repaintEP8window_t, .-_ZN13WindowManager7repaintEP8window_t
	.align 16
	.globl	_ZN13WindowManager6createE6Stringtti
	.type	_ZN13WindowManager6createE6Stringtti, @function
_ZN13WindowManager6createE6Stringtti:
.LFB1:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	$20, (%esp)
	movl	36(%esp), %edi
	movl	40(%esp), %esi
	call	_Z6mallocj
	movl	44(%esp), %ecx
	xorl	%edx, %edx
	movzwl	%di, %edi
	movzwl	%si, %esi
	movl	%eax, %ebx
	movl	32(%esp), %eax
	movl	8(%eax), %eax
	movl	%edi, (%ebx)
	movl	%esi, 4(%ebx)
	movw	%dx, 10(%ebx)
	movl	%eax, 12(%ebx)
	xorl	%eax, %eax
	testl	%ecx, %ecx
	movw	%ax, 8(%ebx)
	jne	.L8
	movl	_ZN2FB4addrE, %eax
	movl	%eax, 16(%ebx)
	addl	$16, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L8:
	.cfi_restore_state
	movl	$1228800, (%esp)
	call	_Z6mallocj
	movl	%eax, 16(%ebx)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE1:
	.size	_ZN13WindowManager6createE6Stringtti, .-_ZN13WindowManager6createE6Stringtti
	.align 16
	.globl	_ZN13WindowManager8drawCharEP8window_tyyci
	.type	_ZN13WindowManager8drawCharEP8window_tyyci, @function
_ZN13WindowManager8drawCharEP8window_tyyci:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	xorl	%ebx, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %eax
	movsbl	52(%esp), %esi
	movl	%eax, (%esp)
	movl	44(%esp), %eax
	sall	$4, %esi
	addl	_ZN2FB4fontE, %esi
	movl	%eax, 4(%esp)
	movl	32(%esp), %eax
	movl	16(%eax), %edi
	.align 16
.L17:
	movl	4(%esp), %eax
	xorl	%ecx, %ecx
	addl	%ebx, %eax
	imull	_ZN2FB5widthE, %eax
	addl	(%esp), %eax
	sall	$2, %eax
	jmp	.L15
	.align 16
.L20:
	movl	56(%esp), %edx
	addl	$1, %ecx
	movl	%edx, (%edi,%eax)
	addl	$4, %eax
	cmpl	$8, %ecx
	je	.L19
.L15:
	movsbl	(%esi,%ebx), %edx
	movl	$128, %ebp
	sarl	%cl, %ebp
	testl	%edx, %ebp
	jne	.L20
	movl	backColor, %edx
	addl	$1, %ecx
	movl	%edx, (%edi,%eax)
	addl	$4, %eax
	cmpl	$8, %ecx
	jne	.L15
.L19:
	addl	$1, %ebx
	cmpl	$16, %ebx
	jne	.L17
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE2:
	.size	_ZN13WindowManager8drawCharEP8window_tyyci, .-_ZN13WindowManager8drawCharEP8window_tyyci
	.align 16
	.globl	_ZN13WindowManager21drawTerminalAsciiCharEP8window_tiici
	.type	_ZN13WindowManager21drawTerminalAsciiCharEP8window_tiici, @function
_ZN13WindowManager21drawTerminalAsciiCharEP8window_tiici:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$36, %esp
	.cfi_def_cfa_offset 56
	movl	56(%esp), %edx
	movl	60(%esp), %ebp
	movl	64(%esp), %ebx
	movl	68(%esp), %ecx
	testl	%edx, %edx
	je	.L21
	testb	%cl, %cl
	movsbl	%cl, %edi
	movl	72(%esp), %esi
	movl	$32, %eax
	cmovne	%edi, %eax
	movl	%eax, 20(%esp)
	leal	(%ebx,%ebx,8), %eax
	addl	%eax, %eax
	movl	%eax, 12(%esp)
	sarl	$31, %eax
	movl	%eax, 16(%esp)
	leal	0(%ebp,%ebp,8), %eax
	movl	%eax, 4(%esp)
	sarl	$31, %eax
	movl	%esi, 24(%esp)
	movl	%eax, 8(%esp)
	movl	%edx, (%esp)
	call	_ZN13WindowManager8drawCharEP8window_tyyci
.L21:
	addl	$36, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE3:
	.size	_ZN13WindowManager21drawTerminalAsciiCharEP8window_tiici, .-_ZN13WindowManager21drawTerminalAsciiCharEP8window_tiici
	.align 16
	.globl	_ZN13WindowManager7repaintEP8window_ttttt
	.type	_ZN13WindowManager7repaintEP8window_ttttt, @function
_ZN13WindowManager7repaintEP8window_ttttt:
.LFB4:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	%eax, 4(%esp)
	jmp	_ZN2FB7repaintEPc
	.cfi_endproc
.LFE4:
	.size	_ZN13WindowManager7repaintEP8window_ttttt, .-_ZN13WindowManager7repaintEP8window_ttttt
	.align 16
	.globl	_ZN13WindowManager5clearEP8window_t
	.type	_ZN13WindowManager5clearEP8window_t, @function
_ZN13WindowManager5clearEP8window_t:
.LFB5:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %eax
	movl	4(%eax), %ecx
	imull	(%eax), %ecx
	testl	%ecx, %ecx
	jle	.L30
	movl	16(%eax), %ebx
	xorl	%eax, %eax
	.align 16
.L33:
	movl	backColor, %edx
	movl	%edx, (%ebx,%eax,4)
	addl	$1, %eax
	cmpl	%eax, %ecx
	jne	.L33
.L30:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE5:
	.size	_ZN13WindowManager5clearEP8window_t, .-_ZN13WindowManager5clearEP8window_t
	.ident	"GCC: (GNU) 4.8.2"
