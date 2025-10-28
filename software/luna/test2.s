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
	adrp	x9, ___unnamed_1@PAGE
Lloh1:
	adrp	x10, _printf@GOTPAGE
Lloh2:
	ldr	x8, [x8, _puts@GOTPAGEOFF]
Lloh3:
	ldr	x10, [x10, _printf@GOTPAGEOFF]
	adrp	x11, ___unnamed_2@PAGE
	str	x8, [x9, ___unnamed_1@PAGEOFF]
Lloh4:
	adrp	x8, ___unnamed_3@PAGE
	mov	w9, #100                        ; =0x64
	str	w9, [x8, ___unnamed_3@PAGEOFF]
Lloh5:
	adrp	x8, ___unnamed_4@PAGE
Lloh6:
	adrp	x9, ___unnamed_5@PAGE
Lloh7:
	add	x9, x9, ___unnamed_5@PAGEOFF
	str	x10, [x11, ___unnamed_2@PAGEOFF]
	str	x9, [x8, ___unnamed_4@PAGEOFF]
	blr	x9
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh6, Lloh7
	.loh AdrpAdrp	Lloh4, Lloh5
	.loh AdrpLdrGot	Lloh1, Lloh3
	.loh AdrpLdrGot	Lloh0, Lloh2
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_5                    ; -- Begin function 
	.p2align	2
___unnamed_5:                           ; @11
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
Lloh8:
	adrp	x8, ___unnamed_1@PAGE
Lloh9:
	adrp	x0, l___unnamed_6@PAGE
Lloh10:
	add	x0, x0, l___unnamed_6@PAGEOFF
Lloh11:
	ldr	x8, [x8, ___unnamed_1@PAGEOFF]
	blr	x8
	adrp	x19, ___unnamed_2@PAGE
	mov	w8, #30                         ; =0x1e
Lloh12:
	adrp	x0, l___unnamed_7@PAGE
Lloh13:
	add	x0, x0, l___unnamed_7@PAGEOFF
	ldr	x9, [x19, ___unnamed_2@PAGEOFF]
	str	w8, [sp, #28]
	str	x8, [sp]
	blr	x9
	ldr	x9, [x19, ___unnamed_2@PAGEOFF]
	mov	w8, #25                         ; =0x19
Lloh14:
	adrp	x0, l___unnamed_8@PAGE
Lloh15:
	add	x0, x0, l___unnamed_8@PAGEOFF
	str	w8, [sp, #28]
	str	x8, [sp]
	blr	x9
Lloh16:
	adrp	x8, ___unnamed_3@PAGE
	ldr	w9, [sp, #28]
Lloh17:
	adrp	x0, l___unnamed_9@PAGE
Lloh18:
	add	x0, x0, l___unnamed_9@PAGEOFF
Lloh19:
	ldr	w8, [x8, ___unnamed_3@PAGEOFF]
	add	w8, w8, w9
	ldr	x9, [x19, ___unnamed_2@PAGEOFF]
	str	w8, [sp, #24]
	str	x8, [sp]
	blr	x9
Lloh20:
	adrp	x8, ___unnamed_10@PAGE
Lloh21:
	add	x9, x8, ___unnamed_10@PAGEOFF
	ldr	x20, [x19, ___unnamed_2@PAGEOFF]
Lloh22:
	adrp	x8, ___unnamed_11@PAGE
Lloh23:
	add	x8, x8, ___unnamed_11@PAGEOFF
Lloh24:
	adrp	x0, ___unnamed_12@PAGE
Lloh25:
	add	x0, x0, ___unnamed_12@PAGEOFF
	stp	x8, x9, [sp, #8]
	blr	x8
	mov	w8, w0
Lloh26:
	adrp	x0, l___unnamed_13@PAGE
Lloh27:
	add	x0, x0, l___unnamed_13@PAGEOFF
	str	x8, [sp]
	blr	x20
	ldp	x8, x0, [sp, #8]
	ldr	x19, [x19, ___unnamed_2@PAGEOFF]
	blr	x8
	mov	w8, w0
Lloh28:
	adrp	x0, l___unnamed_14@PAGE
Lloh29:
	add	x0, x0, l___unnamed_14@PAGEOFF
	str	x8, [sp]
	blr	x19
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.loh AdrpAdd	Lloh28, Lloh29
	.loh AdrpAdd	Lloh26, Lloh27
	.loh AdrpAdd	Lloh24, Lloh25
	.loh AdrpAdd	Lloh22, Lloh23
	.loh AdrpAdrp	Lloh20, Lloh22
	.loh AdrpAdd	Lloh20, Lloh21
	.loh AdrpAdd	Lloh17, Lloh18
	.loh AdrpLdr	Lloh16, Lloh19
	.loh AdrpAdd	Lloh14, Lloh15
	.loh AdrpAdd	Lloh12, Lloh13
	.loh AdrpAdd	Lloh9, Lloh10
	.loh AdrpLdr	Lloh8, Lloh11
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_10                   ; -- Begin function 
	.p2align	2
___unnamed_10:                          ; @12
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w0, #3                          ; =0x3
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_11                   ; -- Begin function 
	.p2align	2
___unnamed_11:                          ; @13
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh30:
	adrp	x8, ___unnamed_1@PAGE
	str	x0, [sp, #8]
Lloh31:
	adrp	x0, l___unnamed_15@PAGE
Lloh32:
	add	x0, x0, l___unnamed_15@PAGEOFF
Lloh33:
	ldr	x8, [x8, ___unnamed_1@PAGEOFF]
	blr	x8
	ldr	x8, [sp, #8]
	blr	x8
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	w0, w0, #1
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh31, Lloh32
	.loh AdrpLdr	Lloh30, Lloh33
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_12                   ; -- Begin function 
	.p2align	2
___unnamed_12:                          ; @14
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w0, #7                          ; =0x7
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_2                    ; @1
.zerofill __DATA,__common,___unnamed_2,8,3
	.globl	___unnamed_3                    ; @2
.zerofill __DATA,__common,___unnamed_3,4,2
	.globl	___unnamed_4                    ; @3
.zerofill __DATA,__common,___unnamed_4,8,3
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_6:                          ; @4
	.asciz	"hello world"

l___unnamed_7:                          ; @5
	.asciz	"1. b: %d\n"

l___unnamed_8:                          ; @6
	.asciz	"2. b: %d\n"

l___unnamed_9:                          ; @7
	.asciz	"3. c: %d\n"

l___unnamed_15:                         ; @8
	.asciz	"inner"

l___unnamed_13:                         ; @9
	.asciz	"4. f: %d\n"

l___unnamed_14:                         ; @10
	.asciz	"5. %d\n"

.subsections_via_symbols
