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
___unnamed_3:                           ; @5
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #10                         ; =0xa
Lloh9:
	adrp	x9, ___unnamed_1@PAGE
Lloh10:
	adrp	x0, l___unnamed_6@PAGE
Lloh11:
	add	x0, x0, l___unnamed_6@PAGEOFF
	stur	w8, [sp, #15]
Lloh12:
	ldr	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh13:
	adrp	x9, l___unnamed_7@PAGE
Lloh14:
	add	x9, x9, l___unnamed_7@PAGEOFF
	str	x9, [sp]
	blr	x8
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldr	x0, [sp]
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh13, Lloh14
	.loh AdrpAdd	Lloh10, Lloh11
	.loh AdrpAdrp	Lloh9, Lloh13
	.loh AdrpLdr	Lloh9, Lloh12
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_5                    ; -- Begin function 
	.p2align	2
___unnamed_5:                           ; @6
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	adrp	x19, ___unnamed_2@PAGE
	ldr	x8, [x19, ___unnamed_2@PAGEOFF]
	blr	x8
Lloh15:
	adrp	x8, ___unnamed_1@PAGE
Lloh16:
	ldr	x20, [x8, ___unnamed_1@PAGEOFF]
	ldr	x8, [x19, ___unnamed_2@PAGEOFF]
	blr	x8
	blr	x20
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	mov	w0, #1                          ; =0x1
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.loh AdrpLdr	Lloh15, Lloh16
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_2                    ; @1
.zerofill __DATA,__common,___unnamed_2,8,3
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_7:                          ; @2
	.asciz	"abc"

l___unnamed_6:                          ; @3
	.asciz	"hello world!"

	.globl	___unnamed_4                    ; @4
.zerofill __DATA,__common,___unnamed_4,8,3
.subsections_via_symbols
