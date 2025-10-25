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
	str	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh5:
	adrp	x9, _printf@GOTPAGE
Lloh6:
	adrp	x8, ___unnamed_2@PAGE
Lloh7:
	ldr	x9, [x9, _printf@GOTPAGEOFF]
	str	x10, [x8, ___unnamed_2@PAGEOFF]
Lloh8:
	adrp	x8, ___unnamed_3@PAGE
	str	x9, [x8, ___unnamed_3@PAGEOFF]
Lloh9:
	adrp	x8, ___unnamed_4@PAGE
	add	x8, x8, ___unnamed_4@PAGEOFF
	mov	w9, #123                        ; =0x7b
	str	w9, [x8]
Lloh10:
	adrp	x8, ___unnamed_5@PAGE
Lloh11:
	adrp	x9, l___unnamed_6@PAGE
Lloh12:
	add	x9, x9, l___unnamed_6@PAGEOFF
	str	x9, [x8, ___unnamed_5@PAGEOFF]
Lloh13:
	adrp	x8, ___unnamed_7@PAGE
Lloh14:
	adrp	x9, ___unnamed_8@PAGE
Lloh15:
	add	x9, x9, ___unnamed_8@PAGEOFF
	str	x9, [x8, ___unnamed_7@PAGEOFF]
Lloh16:
	adrp	x8, ___unnamed_9@PAGE
Lloh17:
	adrp	x9, ___unnamed_10@PAGE
Lloh18:
	add	x9, x9, ___unnamed_10@PAGEOFF
	str	x9, [x8, ___unnamed_9@PAGEOFF]
	blr	x9
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh17, Lloh18
	.loh AdrpAdd	Lloh14, Lloh15
	.loh AdrpAdrp	Lloh13, Lloh16
	.loh AdrpAdd	Lloh11, Lloh12
	.loh AdrpAdrp	Lloh10, Lloh13
	.loh AdrpAdrp	Lloh8, Lloh9
	.loh AdrpAdrp	Lloh6, Lloh8
	.loh AdrpLdrGot	Lloh5, Lloh7
	.loh AdrpLdrGot	Lloh2, Lloh4
	.loh AdrpAdrp	Lloh1, Lloh5
	.loh AdrpLdrGot	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_8                    ; -- Begin function 
	.p2align	2
___unnamed_8:                           ; @16
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
	adrp	x20, ___unnamed_3@PAGE
	adrp	x8, ___unnamed_4@PAGE
	mov	x19, x0
	ldr	x9, [x20, ___unnamed_3@PAGEOFF]
	ldrb	w8, [x8, ___unnamed_4@PAGEOFF]
Lloh19:
	adrp	x0, l___unnamed_11@PAGE
Lloh20:
	add	x0, x0, l___unnamed_11@PAGEOFF
	str	x8, [sp]
	blr	x9
	ldr	x8, [x20, ___unnamed_3@PAGEOFF]
	ldr	x9, [x19]
Lloh21:
	adrp	x0, l___unnamed_12@PAGE
Lloh22:
	add	x0, x0, l___unnamed_12@PAGEOFF
	str	x9, [sp]
	blr	x8
	mov	w8, #52                         ; =0x34
Lloh23:
	adrp	x0, l___unnamed_13@PAGE
Lloh24:
	add	x0, x0, l___unnamed_13@PAGEOFF
	str	w8, [x19]
	ldr	x8, [x20, ___unnamed_3@PAGEOFF]
	ldr	x9, [x19]
	str	x9, [sp]
	blr	x8
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.loh AdrpAdd	Lloh23, Lloh24
	.loh AdrpAdd	Lloh21, Lloh22
	.loh AdrpAdd	Lloh19, Lloh20
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_10                   ; -- Begin function 
	.p2align	2
___unnamed_10:                          ; @17
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #64
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 64
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	adrp	x19, ___unnamed_1@PAGE
Lloh25:
	adrp	x0, l___unnamed_14@PAGE
Lloh26:
	add	x0, x0, l___unnamed_14@PAGEOFF
	ldr	x8, [x19, ___unnamed_1@PAGEOFF]
	blr	x8
Lloh27:
	adrp	x8, ___unnamed_7@PAGE
	mov	w9, #48                         ; =0x30
	add	x0, sp, #31
Lloh28:
	ldr	x8, [x8, ___unnamed_7@PAGEOFF]
	stur	w9, [sp, #31]
	blr	x8
	adrp	x20, ___unnamed_3@PAGE
Lloh29:
	adrp	x8, ___unnamed_5@PAGE
Lloh30:
	adrp	x0, l___unnamed_15@PAGE
Lloh31:
	add	x0, x0, l___unnamed_15@PAGEOFF
	ldr	x9, [x20, ___unnamed_3@PAGEOFF]
Lloh32:
	ldr	x8, [x8, ___unnamed_5@PAGEOFF]
	str	x8, [sp]
	blr	x9
Lloh33:
	adrp	x8, ___unnamed_2@PAGE
	ldrb	w0, [sp, #31]
Lloh34:
	ldr	x8, [x8, ___unnamed_2@PAGEOFF]
	blr	x8
	ldr	x8, [x19, ___unnamed_1@PAGEOFF]
Lloh35:
	adrp	x0, l___unnamed_16@PAGE
Lloh36:
	add	x0, x0, l___unnamed_16@PAGEOFF
	str	x0, [sp, #16]
	blr	x8
	ldr	x8, [x20, ___unnamed_3@PAGEOFF]
Lloh37:
	adrp	x0, l___unnamed_17@PAGE
Lloh38:
	add	x0, x0, l___unnamed_17@PAGEOFF
	blr	x8
	ldr	x8, [x20, ___unnamed_3@PAGEOFF]
	mov	w9, #3                          ; =0x3
Lloh39:
	adrp	x0, l___unnamed_18@PAGE
Lloh40:
	add	x0, x0, l___unnamed_18@PAGEOFF
	mov	w10, #1                         ; =0x1
	stp	x10, x9, [sp]
	blr	x8
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	mov	w0, #1                          ; =0x1
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.loh AdrpAdd	Lloh39, Lloh40
	.loh AdrpAdd	Lloh37, Lloh38
	.loh AdrpAdd	Lloh35, Lloh36
	.loh AdrpLdr	Lloh33, Lloh34
	.loh AdrpAdd	Lloh30, Lloh31
	.loh AdrpLdr	Lloh29, Lloh32
	.loh AdrpLdr	Lloh27, Lloh28
	.loh AdrpAdd	Lloh25, Lloh26
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_2                    ; @1
.zerofill __DATA,__common,___unnamed_2,8,3
	.globl	___unnamed_3                    ; @2
.zerofill __DATA,__common,___unnamed_3,8,3
	.globl	___unnamed_4                    ; @3
.zerofill __DATA,__common,___unnamed_4,1,0
	.globl	___unnamed_5                    ; @4
.zerofill __DATA,__common,___unnamed_5,8,3
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_6:                          ; @5
	.asciz	"bar"

	.globl	___unnamed_7                    ; @6
.zerofill __DATA,__common,___unnamed_7,8,3
l___unnamed_11:                         ; @7
	.asciz	"abc: %d\n"

l___unnamed_12:                         ; @8
	.asciz	"foo pre change: %d\n"

l___unnamed_13:                         ; @9
	.asciz	"foo post change: %d\n"

	.globl	___unnamed_9                    ; @10
.zerofill __DATA,__common,___unnamed_9,8,3
l___unnamed_14:                         ; @11
	.asciz	"pointer test."

l___unnamed_15:                         ; @12
	.asciz	"bar is '%s'\n"

l___unnamed_16:                         ; @13
	.asciz	"hello world!"

l___unnamed_17:                         ; @14
	.asciz	"THIS IS REAL COOL\n"

l___unnamed_18:                         ; @15
	.asciz	"two %d is %d\n"

.subsections_via_symbols
