	.build_version macos, 26, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh0:
	adrp	x8, ___unnamed_1@PAGE
Lloh1:
	adrp	x9, ___unnamed_2@PAGE
Lloh2:
	add	x9, x9, ___unnamed_2@PAGEOFF
	adrp	x10, ___unnamed_3@PAGE
	str	x9, [x8, ___unnamed_1@PAGEOFF]
Lloh3:
	adrp	x8, ___unnamed_4@PAGE
Lloh4:
	add	x8, x8, ___unnamed_4@PAGEOFF
	str	x8, [x10, ___unnamed_3@PAGEOFF]
	blr	x8
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh3, Lloh4
	.loh AdrpAdd	Lloh1, Lloh2
	.loh AdrpAdrp	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_2                    ; -- Begin function 
	.p2align	2
___unnamed_2:                           ; @2
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, #10                         ; =0xa
	str	w8, [sp, #12]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_4                    ; -- Begin function 
	.p2align	2
___unnamed_4:                           ; @3
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh5:
	adrp	x8, ___unnamed_1@PAGE
Lloh6:
	ldr	x8, [x8, ___unnamed_1@PAGEOFF]
	blr	x8
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpLdr	Lloh5, Lloh6
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_3                    ; @1
.zerofill __DATA,__common,___unnamed_3,8,3
.subsections_via_symbols
