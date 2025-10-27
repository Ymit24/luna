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
	movq	__unnamed_5@GOTPCREL(%rip), %rax
	movq	__unnamed_6@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	__unnamed_7@GOTPCREL(%rip), %rax
	movq	__unnamed_8@GOTPCREL(%rip), %rcx
	movq	%rax, (%rcx)
	movq	__unnamed_9@GOTPCREL(%rip), %rax
	movq	__unnamed_10@GOTPCREL(%rip), %rcx
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
__unnamed_3:                            # @21
	.cfi_startproc
# %bb.0:                                # %entry
	cmpl	$1, %edi
	jg	.LBB1_2
# %bb.1:                                # %if.then
	movl	$1, %eax
	retq
.LBB1_2:                                # %if.else
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	movq	__unnamed_4@GOTPCREL(%rip), %rax
	leal	-1(%rbx), %edi
	callq	*(%rax)
	imull	%ebx, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	__unnamed_3, .Lfunc_end1-__unnamed_3
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_5                     # -- Begin function 
	.p2align	4
	.type	__unnamed_5,@function
__unnamed_5:                            # @22
	.cfi_startproc
# %bb.0:                                # %entry
	cmpl	$1, %edi
	jg	.LBB2_2
# %bb.1:                                # %if.then
	movl	$1, %eax
	retq
.LBB2_2:                                # %if.else
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movl	%edi, %ebx
	movq	__unnamed_6@GOTPCREL(%rip), %r14
	leal	-1(%rbx), %edi
	callq	*(%r14)
	movl	%eax, %ebp
	addl	$-2, %ebx
	movl	%ebx, %edi
	callq	*(%r14)
	addl	%ebp, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	__unnamed_5, .Lfunc_end2-__unnamed_5
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_7                     # -- Begin function 
	.p2align	4
	.type	__unnamed_7,@function
__unnamed_7:                            # @23
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	pushq	%rax
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	cmpl	%esi, %edi
	jge	.LBB3_2
# %bb.1:                                # %if.then
	movq	__unnamed_1@GOTPCREL(%rip), %rcx
	movl	%edi, %ebx
	movl	$.L__unnamed_11, %edi
	movl	%esi, %ebp
	movl	%ebx, %esi
	xorl	%eax, %eax
	movq	%rdx, %r14
	callq	*(%rcx)
	movq	__unnamed_2@GOTPCREL(%rip), %r15
	movl	$.L__unnamed_12, %edi
	callq	*(%r15)
	callq	*(%r14)
	movl	$.L__unnamed_13, %edi
	callq	*(%r15)
	movq	__unnamed_8@GOTPCREL(%rip), %rax
	incl	%ebx
	movl	%ebx, %edi
	movl	%ebp, %esi
	movq	%r14, %rdx
	callq	*(%rax)
	jmp	.LBB3_3
.LBB3_2:                                # %if.else
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_14, %edi
	callq	*(%rax)
.LBB3_3:                                # %if.else
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	__unnamed_7, .Lfunc_end3-__unnamed_7
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_9                     # -- Begin function 
	.p2align	4
	.type	__unnamed_9,@function
__unnamed_9:                            # @24
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
	movl	$3, -28(%rbp)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB4_3
# %bb.1:                                # %if.then
	movq	__unnamed_2@GOTPCREL(%rip), %rbx
	movl	$.L__unnamed_15, %edi
	callq	*(%rbx)
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movb	$100, -16(%rax)
	movq	%rsp, %r14
	leaq	-16(%r14), %rsp
	movq	__unnamed_16@GOTPCREL(%rip), %rcx
	movq	%rcx, -16(%r14)
	movq	__unnamed_1@GOTPCREL(%rip), %rcx
	movzbl	-16(%rax), %esi
	xorl	%r15d, %r15d
	movl	$.L__unnamed_17, %edi
	xorl	%eax, %eax
	callq	*(%rcx)
	callq	*-16(%r14)
	movl	$0, -28(%rbp)
	testb	%r15b, %r15b
	je	.LBB4_4
# %bb.2:                                # %if.then1
	movl	$.L__unnamed_18, %edi
	jmp	.LBB4_5
.LBB4_3:                                # %if.else
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_19, %edi
	callq	*(%rax)
	jmp	.LBB4_6
.LBB4_4:                                # %if.else2
	movl	$.L__unnamed_20, %edi
.LBB4_5:                                # %if.end
	callq	*(%rbx)
.LBB4_6:                                # %if.end
	movq	__unnamed_1@GOTPCREL(%rip), %rbx
	movq	(%rbx), %r14
	movq	__unnamed_4@GOTPCREL(%rip), %rax
	movl	$8, %edi
	callq	*(%rax)
	movl	$.L__unnamed_21, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	*%r14
	movq	(%rbx), %rbx
	movq	__unnamed_6@GOTPCREL(%rip), %rax
	movl	$8, %edi
	callq	*(%rax)
	movl	$.L__unnamed_22, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	*%rbx
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_23, %edi
	callq	*(%rax)
	movq	%rsp, %rax
	leaq	-16(%rax), %rdx
	movq	%rdx, %rsp
	movq	__unnamed_24@GOTPCREL(%rip), %rcx
	movq	%rcx, -16(%rax)
	movq	__unnamed_8@GOTPCREL(%rip), %rax
	xorl	%edi, %edi
	movl	$3, %esi
	callq	*(%rax)
	xorl	%eax, %eax
	leaq	-24(%rbp), %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end4:
	.size	__unnamed_9, .Lfunc_end4-__unnamed_9
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_16                    # -- Begin function 
	.p2align	4
	.type	__unnamed_16,@function
__unnamed_16:                           # @25
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_25, %edi
	callq	*(%rax)
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end5:
	.size	__unnamed_16, .Lfunc_end5-__unnamed_16
	.cfi_endproc
                                        # -- End function
	.globl	__unnamed_24                    # -- Begin function 
	.p2align	4
	.type	__unnamed_24,@function
__unnamed_24:                           # @26
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	__unnamed_2@GOTPCREL(%rip), %rax
	movl	$.L__unnamed_26, %edi
	callq	*(%rax)
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end6:
	.size	__unnamed_24, .Lfunc_end6-__unnamed_24
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

	.type	__unnamed_6,@object             # @3
	.globl	__unnamed_6
	.p2align	3, 0x0
__unnamed_6:
	.quad	0
	.size	__unnamed_6, 8

	.type	__unnamed_8,@object             # @4
	.globl	__unnamed_8
	.p2align	3, 0x0
__unnamed_8:
	.quad	0
	.size	__unnamed_8, 8

	.type	.L__unnamed_11,@object          # @5
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_11:
	.asciz	"%d\n"
	.size	.L__unnamed_11, 4

	.type	.L__unnamed_12,@object          # @6
.L__unnamed_12:
	.asciz	"prebodycall"
	.size	.L__unnamed_12, 12

	.type	.L__unnamed_13,@object          # @7
.L__unnamed_13:
	.asciz	"called body done."
	.size	.L__unnamed_13, 18

	.type	.L__unnamed_14,@object          # @8
.L__unnamed_14:
	.asciz	"done."
	.size	.L__unnamed_14, 6

	.type	__unnamed_10,@object            # @9
	.bss
	.globl	__unnamed_10
	.p2align	3, 0x0
__unnamed_10:
	.quad	0
	.size	__unnamed_10, 8

	.type	.L__unnamed_15,@object          # @10
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_15:
	.asciz	"it worked!"
	.size	.L__unnamed_15, 11

	.type	.L__unnamed_25,@object          # @11
.L__unnamed_25:
	.asciz	"test"
	.size	.L__unnamed_25, 5

	.type	.L__unnamed_17,@object          # @12
.L__unnamed_17:
	.asciz	"foo is %d\n"
	.size	.L__unnamed_17, 11

	.type	.L__unnamed_18,@object          # @13
.L__unnamed_18:
	.asciz	"other"
	.size	.L__unnamed_18, 6

	.type	.L__unnamed_27,@object          # @14
.L__unnamed_27:
	.asciz	"hmm"
	.size	.L__unnamed_27, 4

	.type	.L__unnamed_20,@object          # @15
.L__unnamed_20:
	.asciz	"not other"
	.size	.L__unnamed_20, 10

	.type	.L__unnamed_19,@object          # @16
.L__unnamed_19:
	.asciz	"lame"
	.size	.L__unnamed_19, 5

	.type	.L__unnamed_21,@object          # @17
.L__unnamed_21:
	.asciz	"factorial of 3 is %d\n"
	.size	.L__unnamed_21, 22

	.type	.L__unnamed_22,@object          # @18
.L__unnamed_22:
	.asciz	"fib(6) is %d\n"
	.size	.L__unnamed_22, 14

	.type	.L__unnamed_23,@object          # @19
.L__unnamed_23:
	.asciz	"calling while"
	.size	.L__unnamed_23, 14

	.type	.L__unnamed_26,@object          # @20
.L__unnamed_26:
	.asciz	"test"
	.size	.L__unnamed_26, 5

	.section	".note.GNU-stack","",@progbits
