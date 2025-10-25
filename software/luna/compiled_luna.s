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
	adrp	x10, _getchar@GOTPAGE
Lloh3:
	ldr	x8, [x8, _puts@GOTPAGEOFF]
Lloh4:
	ldr	x10, [x10, _getchar@GOTPAGEOFF]
	str	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh5:
	adrp	x9, _putchar@GOTPAGE
Lloh6:
	adrp	x8, ___unnamed_2@PAGE
Lloh7:
	ldr	x9, [x9, _putchar@GOTPAGEOFF]
	str	x10, [x8, ___unnamed_2@PAGEOFF]
Lloh8:
	adrp	x8, ___unnamed_3@PAGE
	str	x9, [x8, ___unnamed_3@PAGEOFF]
Lloh9:
	adrp	x8, ___unnamed_4@PAGE
Lloh10:
	adrp	x9, ___unnamed_5@PAGE
Lloh11:
	add	x9, x9, ___unnamed_5@PAGEOFF
	str	x9, [x8, ___unnamed_4@PAGEOFF]
Lloh12:
	adrp	x8, ___unnamed_6@PAGE
Lloh13:
	adrp	x9, ___unnamed_7@PAGE
Lloh14:
	add	x9, x9, ___unnamed_7@PAGEOFF
	str	x9, [x8, ___unnamed_6@PAGEOFF]
Lloh15:
	adrp	x8, ___unnamed_8@PAGE
Lloh16:
	adrp	x9, ___unnamed_9@PAGE
Lloh17:
	add	x9, x9, ___unnamed_9@PAGEOFF
	str	x9, [x8, ___unnamed_8@PAGEOFF]
	blr	x9
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh16, Lloh17
	.loh AdrpAdd	Lloh13, Lloh14
	.loh AdrpAdrp	Lloh12, Lloh15
	.loh AdrpAdd	Lloh10, Lloh11
	.loh AdrpAdrp	Lloh9, Lloh12
	.loh AdrpAdrp	Lloh8, Lloh9
	.loh AdrpAdrp	Lloh6, Lloh8
	.loh AdrpLdrGot	Lloh5, Lloh7
	.loh AdrpLdrGot	Lloh2, Lloh4
	.loh AdrpAdrp	Lloh1, Lloh5
	.loh AdrpLdrGot	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_5                    ; -- Begin function 
	.p2align	2
___unnamed_5:                           ; @11
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #10                         ; =0xa
Lloh18:
	adrp	x9, ___unnamed_1@PAGE
Lloh19:
	adrp	x0, l___unnamed_10@PAGE
Lloh20:
	add	x0, x0, l___unnamed_10@PAGEOFF
	stur	w8, [sp, #15]
Lloh21:
	ldr	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh22:
	adrp	x9, l___unnamed_11@PAGE
Lloh23:
	add	x9, x9, l___unnamed_11@PAGEOFF
	str	x9, [sp]
	blr	x8
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldr	x0, [sp]
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh22, Lloh23
	.loh AdrpAdd	Lloh19, Lloh20
	.loh AdrpAdrp	Lloh18, Lloh22
	.loh AdrpLdr	Lloh18, Lloh21
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_7                    ; -- Begin function 
	.p2align	2
___unnamed_7:                           ; @12
	.cfi_startproc
; %bb.0:                                ; %entry
	add	w0, w0, w1
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_9                    ; -- Begin function 
	.p2align	2
___unnamed_9:                           ; @13
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #64
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 64
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
Lloh24:
	adrp	x19, ___unnamed_4@PAGE
	ldr	x8, [x19, ___unnamed_4@PAGEOFF]
	blr	x8
Lloh25:
	adrp	x8, ___unnamed_12@PAGE
Lloh26:
	add	x8, x8, ___unnamed_12@PAGEOFF
	str	x8, [sp, #8]
	blr	x8
	adrp	x20, ___unnamed_1@PAGE
	ldr	x8, [x19, ___unnamed_4@PAGEOFF]
	ldr	x21, [x20, ___unnamed_1@PAGEOFF]
	blr	x8
	blr	x21
	ldr	x8, [x20, ___unnamed_1@PAGEOFF]
Lloh27:
	adrp	x0, l___unnamed_13@PAGE
Lloh28:
	add	x0, x0, l___unnamed_13@PAGEOFF
	blr	x8
Lloh29:
	adrp	x8, ___unnamed_6@PAGE
	mov	w0, #2                          ; =0x2
	mov	w1, #3                          ; =0x3
Lloh30:
	ldr	x8, [x8, ___unnamed_6@PAGEOFF]
	blr	x8
Lloh31:
	adrp	x19, ___unnamed_3@PAGE
	str	w0, [sp, #4]
	mov	w0, #52                         ; =0x34
	ldr	x8, [x19, ___unnamed_3@PAGEOFF]
	blr	x8
	ldr	x8, [x20, ___unnamed_1@PAGEOFF]
Lloh32:
	adrp	x0, l___unnamed_14@PAGE
Lloh33:
	add	x0, x0, l___unnamed_14@PAGEOFF
	blr	x8
	ldr	w9, [sp, #4]
	ldr	x8, [x19, ___unnamed_3@PAGEOFF]
	add	w0, w9, #48
	blr	x8
	ldr	x8, [x19, ___unnamed_3@PAGEOFF]
	mov	w0, wzr
	blr	x8
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	mov	w0, #1                          ; =0x1
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.loh AdrpAdd	Lloh32, Lloh33
	.loh AdrpLdr	Lloh29, Lloh30
	.loh AdrpAdd	Lloh27, Lloh28
	.loh AdrpAdd	Lloh25, Lloh26
	.loh AdrpAdrp	Lloh24, Lloh31
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_12                   ; -- Begin function 
	.p2align	2
___unnamed_12:                          ; @14
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh34:
	adrp	x8, ___unnamed_15@PAGE
Lloh35:
	add	x8, x8, ___unnamed_15@PAGEOFF
	str	x8, [sp, #8]
	blr	x8
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh34, Lloh35
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_15                   ; -- Begin function 
	.p2align	2
___unnamed_15:                          ; @15
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh36:
	adrp	x8, ___unnamed_1@PAGE
Lloh37:
	adrp	x0, l___unnamed_16@PAGE
Lloh38:
	add	x0, x0, l___unnamed_16@PAGEOFF
Lloh39:
	ldr	x8, [x8, ___unnamed_1@PAGEOFF]
	blr	x8
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh37, Lloh38
	.loh AdrpLdr	Lloh36, Lloh39
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
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_11:                         ; @4
	.asciz	"abc"

l___unnamed_10:                         ; @5
	.asciz	"hello world!"

	.globl	___unnamed_6                    ; @6
.zerofill __DATA,__common,___unnamed_6,8,3
	.globl	___unnamed_8                    ; @7
.zerofill __DATA,__common,___unnamed_8,8,3
l___unnamed_16:                         ; @8
	.asciz	"deepinner"

l___unnamed_13:                         ; @9
	.asciz	"computing.."

l___unnamed_14:                         ; @10
	.asciz	"sum of 2+3="

.subsections_via_symbols
