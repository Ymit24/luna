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
__unnamed_2:                            # @3
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	$100, 8(%rsp)
	movq	__unnamed_1@GOTPCREL(%rip), %rcx
	movl	$.L__unnamed_4, %edi
	movl	$100, %esi
	movl	$100, %edx
	xorl	%eax, %eax
	callq	*(%rcx)
	movq	$50, 8(%rsp)
	movl	$30, 20(%rsp)
	movl	$1, %eax
	addq	$24, %rsp
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
	.asciz	"a: %d -- %p\n"
	.size	.L__unnamed_4, 13

	.section	".note.GNU-stack","",@progbits
