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
	movq	putchar@GOTPCREL(%rip), %rax
	movq	__unnamed_2@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	printf@GOTPCREL(%rip), %rax
	movq	__unnamed_3@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	strlen@GOTPCREL(%rip), %rax
	movq	__unnamed_4@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	__unnamed_5@GOTPCREL(%rip), %rax
	movq	__unnamed_6@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	callq	*%rax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_5                     # -- Begin function 
	.p2align	4
	.type	__unnamed_5,@function
__unnamed_5:                            # @5
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$50, -8(%rsp)
	movl	$30, -12(%rsp)
	movl	$1, %eax
	retq
.Lfunc_end1:
	.size	__unnamed_5, .Lfunc_end1-__unnamed_5
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
	.p2align	3, 0x0
__unnamed_3:
	.quad	0
	.size	__unnamed_3, 8

	.type	__unnamed_4,@object             # @3
	.globl	__unnamed_4
	.p2align	3, 0x0
__unnamed_4:
	.quad	0
	.size	__unnamed_4, 8

	.type	__unnamed_6,@object             # @4
	.globl	__unnamed_6
	.p2align	3, 0x0
__unnamed_6:
	.quad	0
	.size	__unnamed_6, 8

	.section	".note.GNU-stack","",@progbits
