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
	adrp	x9, ___unnamed_1@PAGE
Lloh2:
	adrp	x10, _putchar@GOTPAGE
Lloh3:
	ldr	x8, [x8, _puts@GOTPAGEOFF]
Lloh4:
	ldr	x10, [x10, _putchar@GOTPAGEOFF]
	adrp	x12, ___unnamed_2@PAGE
	adrp	x11, ___unnamed_3@PAGE
	str	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh5:
	adrp	x8, ___unnamed_4@PAGE
Lloh6:
	add	x8, x8, ___unnamed_4@PAGEOFF
	str	x8, [x12, ___unnamed_2@PAGEOFF]
	adrp	x8, ___unnamed_5@PAGE
Lloh7:
	adrp	x9, ___unnamed_6@PAGE
Lloh8:
	add	x9, x9, ___unnamed_6@PAGEOFF
	str	x10, [x11, ___unnamed_3@PAGEOFF]
	str	x9, [x8, ___unnamed_5@PAGEOFF]
	blr	x9
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh7, Lloh8
	.loh AdrpAdd	Lloh5, Lloh6
	.loh AdrpLdrGot	Lloh2, Lloh4
	.loh AdrpAdrp	Lloh1, Lloh7
	.loh AdrpLdrGot	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_4                    ; -- Begin function 
	.p2align	2
___unnamed_4:                           ; @6
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w8, #52                         ; =0x34
	str	w8, [x0]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_6                    ; -- Begin function 
	.p2align	2
___unnamed_6:                           ; @7
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	adrp	x19, ___unnamed_1@PAGE
Lloh9:
	adrp	x0, l___unnamed_7@PAGE
Lloh10:
	add	x0, x0, l___unnamed_7@PAGEOFF
	ldr	x8, [x19, ___unnamed_1@PAGEOFF]
	blr	x8
	adrp	x20, ___unnamed_3@PAGE
	mov	w8, #48                         ; =0x30
	mov	w0, #48                         ; =0x30
	ldr	x9, [x20, ___unnamed_3@PAGEOFF]
	stur	w8, [sp, #15]
	blr	x9
Lloh11:
	adrp	x8, ___unnamed_2@PAGE
	add	x0, sp, #15
Lloh12:
	ldr	x8, [x8, ___unnamed_2@PAGEOFF]
	blr	x8
	ldr	x8, [x20, ___unnamed_3@PAGEOFF]
	ldrb	w0, [sp, #15]
	blr	x8
	ldr	x8, [x19, ___unnamed_1@PAGEOFF]
Lloh13:
	adrp	x0, l___unnamed_8@PAGE
Lloh14:
	add	x0, x0, l___unnamed_8@PAGEOFF
	str	x0, [sp]
	blr	x8
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	mov	w0, #1                          ; =0x1
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.loh AdrpAdd	Lloh13, Lloh14
	.loh AdrpLdr	Lloh11, Lloh12
	.loh AdrpAdd	Lloh9, Lloh10
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_3                    ; @1
.zerofill __DATA,__common,___unnamed_3,8,3
	.globl	___unnamed_2                    ; @2
.zerofill __DATA,__common,___unnamed_2,8,3
	.globl	___unnamed_5                    ; @3
.zerofill __DATA,__common,___unnamed_5,8,3
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_7:                          ; @4
	.asciz	"pointer test."

l___unnamed_8:                          ; @5
	.asciz	"hello world!"

.subsections_via_symbols
