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
	adrp	x9, _puts@GOTPAGE
Lloh1:
	adrp	x8, ___unnamed_1@PAGE
Lloh2:
	adrp	x10, ___unnamed_2@PAGE
Lloh3:
	ldr	x9, [x9, _puts@GOTPAGEOFF]
Lloh4:
	adrp	x11, ___unnamed_3@PAGE
Lloh5:
	add	x11, x11, ___unnamed_3@PAGEOFF
	str	x11, [x10, ___unnamed_2@PAGEOFF]
Lloh6:
	adrp	x10, ___unnamed_4@PAGE
	str	x9, [x8, ___unnamed_1@PAGEOFF]
Lloh7:
	adrp	x8, ___unnamed_5@PAGE
Lloh8:
	add	x8, x8, ___unnamed_5@PAGEOFF
	str	x8, [x10, ___unnamed_4@PAGEOFF]
	blr	x8
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh7, Lloh8
	.loh AdrpAdd	Lloh4, Lloh5
	.loh AdrpAdrp	Lloh2, Lloh6
	.loh AdrpAdrp	Lloh1, Lloh7
	.loh AdrpLdrGot	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_3                    ; -- Begin function 
	.p2align	2
___unnamed_3:                           ; @4
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, #10                         ; =0xa
	mov	w0, #10                         ; =0xa
	stur	w8, [sp, #15]
Lloh9:
	adrp	x8, l___unnamed_6@PAGE
Lloh10:
	add	x8, x8, l___unnamed_6@PAGEOFF
	str	x8, [sp], #16
	ret
	.loh AdrpAdd	Lloh9, Lloh10
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_5                    ; -- Begin function 
	.p2align	2
___unnamed_5:                           ; @5
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh11:
	adrp	x8, ___unnamed_2@PAGE
Lloh12:
	ldr	x8, [x8, ___unnamed_2@PAGEOFF]
	blr	x8
	mov	w0, #1                          ; =0x1
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpLdr	Lloh11, Lloh12
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_2                    ; @1
.zerofill __DATA,__common,___unnamed_2,8,3
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_6:                          ; @2
	.asciz	"abc"

	.globl	___unnamed_4                    ; @3
.zerofill __DATA,__common,___unnamed_4,8,3
.subsections_via_symbols
