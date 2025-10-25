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
	adrp	x8, _puts@GOTPAGE
Lloh1:
	adrp	x10, _putchar@GOTPAGE
Lloh2:
	adrp	x9, ___unnamed_1@PAGE
Lloh3:
	ldr	x8, [x8, _puts@GOTPAGEOFF]
Lloh4:
	ldr	x10, [x10, _putchar@GOTPAGEOFF]
	str	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh5:
	adrp	x8, ___unnamed_2@PAGE
Lloh6:
	adrp	x9, ___unnamed_3@PAGE
	str	x10, [x8, ___unnamed_2@PAGEOFF]
Lloh7:
	adrp	x8, _printf@GOTPAGE
Lloh8:
	adrp	x10, _strlen@GOTPAGE
Lloh9:
	ldr	x8, [x8, _printf@GOTPAGEOFF]
Lloh10:
	ldr	x10, [x10, _strlen@GOTPAGEOFF]
	str	x8, [x9, ___unnamed_3@PAGEOFF]
Lloh11:
	adrp	x8, ___unnamed_4@PAGE
Lloh12:
	adrp	x9, ___unnamed_5@PAGE
Lloh13:
	add	x9, x9, ___unnamed_5@PAGEOFF
	str	x10, [x8, ___unnamed_4@PAGEOFF]
Lloh14:
	adrp	x8, ___unnamed_6@PAGE
	str	x9, [x8, ___unnamed_6@PAGEOFF]
	blr	x9
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh12, Lloh13
	.loh AdrpAdrp	Lloh11, Lloh14
	.loh AdrpLdrGot	Lloh8, Lloh10
	.loh AdrpLdrGot	Lloh7, Lloh9
	.loh AdrpAdrp	Lloh6, Lloh12
	.loh AdrpAdrp	Lloh5, Lloh7
	.loh AdrpAdrp	Lloh2, Lloh6
	.loh AdrpLdrGot	Lloh1, Lloh4
	.loh AdrpLdrGot	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_5                    ; -- Begin function 
	.p2align	2
___unnamed_5:                           ; @5
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, #100                        ; =0x64
	mov	w9, #50                         ; =0x32
	mov	w0, #1                          ; =0x1
	str	x8, [sp, #8]
	mov	w8, #30                         ; =0x1e
	stp	w8, w9, [sp, #4]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_2                    ; @1
.zerofill __DATA,__common,___unnamed_2,8,3
	.globl	___unnamed_3                    ; @2
.zerofill __DATA,__common,___unnamed_3,8,3
	.globl	___unnamed_4                    ; @3
.zerofill __DATA,__common,___unnamed_4,8,3
	.globl	___unnamed_6                    ; @4
.zerofill __DATA,__common,___unnamed_6,8,3
.subsections_via_symbols
