	.file	"m"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	printf@GOTPCREL(%rip), %rax
	movq	__unnamed_1@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movq	__unnamed_3@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	callq	*%rax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_2                     # -- Begin function 
	.p2align	4
	.type	__unnamed_2,@function
__unnamed_2:                            # @5
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movl	$21, 3(%rsp)
	leaq	3(%rsp), %rsi
	movq	%rsi, 8(%rsp)
	leaq	8(%rsp), %rcx
	movq	%rcx, 16(%rsp)
	movq	__unnamed_1@GOTPCREL(%rip), %r14
	movq	3(%rsp), %rdx
	movl	$.L__unnamed_4, %edi
	xorl	%eax, %eax
	callq	*(%r14)
	movl	$200, 4(%rsp)
	leaq	4(%rsp), %rbx
	movl	$.L__unnamed_5, %edi
	movl	$200, %esi
	movq	%rbx, %rdx
	xorl	%eax, %eax
	callq	*(%r14)
	movl	$30, 4(%rsp)
	movl	$.L__unnamed_6, %edi
	movl	$30, %esi
	movq	%rbx, %rdx
	xorl	%eax, %eax
	callq	*(%r14)
	movl	$1, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	__unnamed_2, .Lfunc_end1-__unnamed_2
	.cfi_endproc
                                        # -- End function
	.type	__unnamed_1,@object             # @0
	.bss
	.globl	__unnamed_1
	.p2align	3, 0x0
__unnamed_1:
	.quad	0
	.size	__unnamed_1, 8

	.type	__unnamed_3,@object             # @1
	.globl	__unnamed_3
	.p2align	3, 0x0
__unnamed_3:
	.quad	0
	.size	__unnamed_3, 8

	.type	.L__unnamed_4,@object           # @2
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_4:
	.asciz	"a: %d -- %d -- %p\n"
	.size	.L__unnamed_4, 19

	.type	.L__unnamed_5,@object           # @3
.L__unnamed_5:
	.asciz	"b: %d -- %p\n"
	.size	.L__unnamed_5, 13

	.type	.L__unnamed_6,@object           # @4
.L__unnamed_6:
	.asciz	"b: %d -- %p\n"
	.size	.L__unnamed_6, 13

	.section	".note.GNU-stack","",@progbits
