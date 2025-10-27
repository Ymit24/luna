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
	movq	puts@GOTPCREL(%rip), %rax
	movq	__unnamed_2@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	__unnamed_3@GOTPCREL(%rip), %rax
	movq	__unnamed_4@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	callq	*%rax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_3                     # -- Begin function 
	.p2align	4
	.type	__unnamed_3,@function
__unnamed_3:                            # @11
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
	movl	$21, 12(%rsp)
	leaq	12(%rsp), %rsi
	movq	%rsi, (%rsp)
	movq	%rsp, %rcx
	movq	%rcx, 16(%rsp)
	movq	__unnamed_1@GOTPCREL(%rip), %r14
	movl	$.L__unnamed_5, %edi
	movl	$21, %edx
	movq	%rsi, %r8
	movl	$21, %r9d
	xorl	%eax, %eax
	callq	*(%r14)
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_6, %edi
	callq	*(%rax)
	movq	(%rsp), %rsi
	movl	(%rsi), %edx
	movl	$.L__unnamed_7, %edi
	xorl	%eax, %eax
	callq	*(%r14)
	movq	(%rsp), %rax
	movl	$102, (%rax)
	movq	(%rsp), %rsi
	movl	(%rsi), %edx
	movl	$.L__unnamed_8, %edi
	xorl	%eax, %eax
	callq	*(%r14)
	movq	$101, (%rsp)
	movl	$.L__unnamed_9, %edi
	movl	$101, %esi
	xorl	%eax, %eax
	callq	*(%r14)
	movl	12(%rsp), %esi
	movl	$.L__unnamed_10, %edi
	xorl	%eax, %eax
	callq	*(%r14)
	movl	$200, 8(%rsp)
	leaq	8(%rsp), %rbx
	movl	$.L__unnamed_11, %edi
	movl	$200, %esi
	movq	%rbx, %rdx
	xorl	%eax, %eax
	callq	*(%r14)
	movl	8(%rsp), %esi
	movl	$.L__unnamed_12, %edi
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
	.size	__unnamed_3, .Lfunc_end1-__unnamed_3
	.cfi_endproc
                                        # -- End function
	.type	__unnamed_1,@object             # @0
	.bss
	.globl	__unnamed_1
	.p2align	3, 0x0
__unnamed_1:
	.quad	0
	.size	__unnamed_1, 8

	.type	__unnamed_2,@object             # @1
	.globl	__unnamed_2
	.p2align	3, 0x0
__unnamed_2:
	.quad	0
	.size	__unnamed_2, 8

	.type	__unnamed_4,@object             # @2
	.globl	__unnamed_4
	.p2align	3, 0x0
__unnamed_4:
	.quad	0
	.size	__unnamed_4, 8

	.type	.L__unnamed_5,@object           # @3
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_5:
	.asciz	"a: %p -- %d -- %p -- %p ( %d )\n"
	.size	.L__unnamed_5, 32

	.type	.L__unnamed_6,@object           # @4
.L__unnamed_6:
	.asciz	"test"
	.size	.L__unnamed_6, 5

	.type	.L__unnamed_7,@object           # @5
.L__unnamed_7:
	.asciz	"1. a: %p %d\n"
	.size	.L__unnamed_7, 13

	.type	.L__unnamed_8,@object           # @6
.L__unnamed_8:
	.asciz	"2. a: %d %d\n"
	.size	.L__unnamed_8, 13

	.type	.L__unnamed_9,@object           # @7
.L__unnamed_9:
	.asciz	"3. a: %d\n"
	.size	.L__unnamed_9, 10

	.type	.L__unnamed_10,@object          # @8
.L__unnamed_10:
	.asciz	"foo: %d\n"
	.size	.L__unnamed_10, 9

	.type	.L__unnamed_11,@object          # @9
.L__unnamed_11:
	.asciz	"b: %d -- %p\n"
	.size	.L__unnamed_11, 13

	.type	.L__unnamed_12,@object          # @10
.L__unnamed_12:
	.asciz	"b: %d -- %p\n"
	.size	.L__unnamed_12, 13

	.section	".note.GNU-stack","",@progbits
