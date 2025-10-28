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
	movq	puts@GOTPCREL(%rip), %rax
	movq	__unnamed_1@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	printf@GOTPCREL(%rip), %rax
	movq	__unnamed_2@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	__unnamed_3@GOTPCREL(%rip), %rax
	movl	$100, (%rax)
	movq	__unnamed_4@GOTPCREL(%rip), %rax
	movq	%rax, (%rsp)
	movq	__unnamed_5@GOTPCREL(%rip), %rax
	movq	%rsp, %rcx
	movq	%rcx, (%rax)
	callq	*%rcx
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_4                     # -- Begin function 
	.p2align	4
	.type	__unnamed_4,@function
__unnamed_4:                            # @9
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$64, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -16
	movq	__unnamed_1@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_6, %edi
	callq	*(%rax)
	movl	$30, 12(%rsp)
	movq	__unnamed_2@GOTPCREL(%rip), %rbx
	movl	$.L__unnamed_7, %edi
	movl	$30, %esi
	xorl	%eax, %eax
	callq	*(%rbx)
	movl	$25, 12(%rsp)
	movl	$.L__unnamed_8, %edi
	movl	$25, %esi
	xorl	%eax, %eax
	callq	*(%rbx)
	movq	__unnamed_3@GOTPCREL(%rip), %rax
	movl	(%rax), %esi
	addl	12(%rsp), %esi
	movl	%esi, 44(%rsp)
	movl	$.L__unnamed_9, %edi
	xorl	%eax, %eax
	callq	*(%rbx)
	movq	__unnamed_10@GOTPCREL(%rip), %rax
	movq	%rax, 32(%rsp)
	leaq	32(%rsp), %rax
	movq	%rax, 56(%rsp)
	movq	__unnamed_11@GOTPCREL(%rip), %rax
	movq	%rax, 24(%rsp)
	leaq	24(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	(%rbx), %rbx
	movq	__unnamed_12@GOTPCREL(%rip), %rcx
	movq	%rcx, 16(%rsp)
	leaq	16(%rsp), %rdi
	callq	*%rax
	movl	$.L__unnamed_13, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	*%rbx
	addq	$64, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	__unnamed_4, .Lfunc_end1-__unnamed_4
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_10                    # -- Begin function 
	.p2align	4
	.type	__unnamed_10,@function
__unnamed_10:                           # @10
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$3, %eax
	retq
.Lfunc_end2:
	.size	__unnamed_10, .Lfunc_end2-__unnamed_10
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_11                    # -- Begin function 
	.p2align	4
	.type	__unnamed_11,@function
__unnamed_11:                           # @11
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	callq	*(%rdi)
	incl	%eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	__unnamed_11, .Lfunc_end3-__unnamed_11
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_12                    # -- Begin function 
	.p2align	4
	.type	__unnamed_12,@function
__unnamed_12:                           # @12
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$7, %eax
	retq
.Lfunc_end4:
	.size	__unnamed_12, .Lfunc_end4-__unnamed_12
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

	.type	__unnamed_3,@object             # @2
	.globl	__unnamed_3
	.p2align	2, 0x0
__unnamed_3:
	.long	0                               # 0x0
	.size	__unnamed_3, 4

	.type	__unnamed_5,@object             # @3
	.globl	__unnamed_5
	.p2align	3, 0x0
__unnamed_5:
	.quad	0
	.size	__unnamed_5, 8

	.type	.L__unnamed_6,@object           # @4
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_6:
	.asciz	"hello world"
	.size	.L__unnamed_6, 12

	.type	.L__unnamed_7,@object           # @5
.L__unnamed_7:
	.asciz	"1. b: %d\n"
	.size	.L__unnamed_7, 10

	.type	.L__unnamed_8,@object           # @6
.L__unnamed_8:
	.asciz	"2. b: %d\n"
	.size	.L__unnamed_8, 10

	.type	.L__unnamed_9,@object           # @7
.L__unnamed_9:
	.asciz	"3. c: %d\n"
	.size	.L__unnamed_9, 10

	.type	.L__unnamed_13,@object          # @8
.L__unnamed_13:
	.asciz	"4. f: %d\n"
	.size	.L__unnamed_13, 10

	.section	".note.GNU-stack","",@progbits
