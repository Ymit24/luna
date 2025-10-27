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
__unnamed_3:                            # @10
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	$1, -28(%rbp)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB1_3
# %bb.1:                                # %if.then
	movq	__unnamed_2@GOTPCREL(%rip), %rbx
	movl	$.L__unnamed_5, %edi
	callq	*(%rbx)
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movb	$100, -16(%rax)
	movq	%rsp, %r14
	leaq	-16(%r14), %rsp
	movq	__unnamed_6@GOTPCREL(%rip), %rcx
	movq	%rcx, -16(%r14)
	movq	__unnamed_1@GOTPCREL(%rip), %rcx
	movzbl	-16(%rax), %esi
	xorl	%r15d, %r15d
	movl	$.L__unnamed_7, %edi
	xorl	%eax, %eax
	callq	*(%rcx)
	callq	*-16(%r14)
	movl	$0, -28(%rbp)
	testb	%r15b, %r15b
	je	.LBB1_4
# %bb.2:                                # %if.then1
	movl	$.L__unnamed_8, %edi
	jmp	.LBB1_5
.LBB1_3:                                # %if.else
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_9, %edi
	callq	*(%rax)
	jmp	.LBB1_6
.LBB1_4:                                # %if.else2
	movl	$.L__unnamed_10, %edi
.LBB1_5:                                # %if.end
	callq	*(%rbx)
.LBB1_6:                                # %if.end
	xorl	%eax, %eax
	leaq	-24(%rbp), %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	__unnamed_3, .Lfunc_end1-__unnamed_3
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_6                     # -- Begin function 
	.p2align	4
	.type	__unnamed_6,@function
__unnamed_6:                            # @11
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_11, %edi
	callq	*(%rax)
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	__unnamed_6, .Lfunc_end2-__unnamed_6
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
	.asciz	"it worked!"
	.size	.L__unnamed_5, 11

	.type	.L__unnamed_11,@object          # @4
.L__unnamed_11:
	.asciz	"test"
	.size	.L__unnamed_11, 5

	.type	.L__unnamed_7,@object           # @5
.L__unnamed_7:
	.asciz	"foo is %d\n"
	.size	.L__unnamed_7, 11

	.type	.L__unnamed_8,@object           # @6
.L__unnamed_8:
	.asciz	"other"
	.size	.L__unnamed_8, 6

	.type	.L__unnamed_12,@object          # @7
.L__unnamed_12:
	.asciz	"hmm"
	.size	.L__unnamed_12, 4

	.type	.L__unnamed_10,@object          # @8
.L__unnamed_10:
	.asciz	"not other"
	.size	.L__unnamed_10, 10

	.type	.L__unnamed_9,@object           # @9
.L__unnamed_9:
	.asciz	"lame"
	.size	.L__unnamed_9, 5

	.section	".note.GNU-stack","",@progbits
