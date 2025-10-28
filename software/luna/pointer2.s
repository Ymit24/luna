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
	adrp	x8, _printf@GOTPAGE
Lloh1:
	adrp	x10, _puts@GOTPAGE
Lloh2:
	adrp	x9, ___unnamed_1@PAGE
Lloh3:
	ldr	x8, [x8, _printf@GOTPAGEOFF]
Lloh4:
	ldr	x10, [x10, _puts@GOTPAGEOFF]
	adrp	x11, ___unnamed_2@PAGE
	str	x8, [x9, ___unnamed_1@PAGEOFF]
	adrp	x8, ___unnamed_3@PAGE
Lloh5:
	adrp	x9, ___unnamed_4@PAGE
Lloh6:
	add	x9, x9, ___unnamed_4@PAGEOFF
	str	x10, [x11, ___unnamed_2@PAGEOFF]
	str	x9, [x8, ___unnamed_3@PAGEOFF]
	blr	x9
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh5, Lloh6
	.loh AdrpAdrp	Lloh2, Lloh5
	.loh AdrpLdrGot	Lloh1, Lloh4
	.loh AdrpLdrGot	Lloh0, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_4                    ; -- Begin function 
	.p2align	2
___unnamed_4:                           ; @12
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #112
	stp	x20, x19, [sp, #80]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 112
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	adrp	x20, ___unnamed_2@PAGE
Lloh7:
	adrp	x0, l___unnamed_5@PAGE
Lloh8:
	add	x0, x0, l___unnamed_5@PAGEOFF
	ldr	x8, [x20, ___unnamed_2@PAGEOFF]
	blr	x8
	adrp	x19, ___unnamed_1@PAGE
	mov	w8, #21                         ; =0x15
	add	x9, sp, #76
	ldr	x11, [x19, ___unnamed_1@PAGEOFF]
	add	x10, sp, #64
Lloh9:
	adrp	x0, l___unnamed_6@PAGE
Lloh10:
	add	x0, x0, l___unnamed_6@PAGEOFF
	str	w8, [sp, #76]
	stp	x10, x9, [sp, #56]
	stp	x9, x8, [sp, #24]
	stp	x8, x10, [sp, #8]
	str	x9, [sp]
	blr	x11
	ldr	x8, [x20, ___unnamed_2@PAGEOFF]
Lloh11:
	adrp	x0, l___unnamed_7@PAGE
Lloh12:
	add	x0, x0, l___unnamed_7@PAGEOFF
	blr	x8
	ldr	x8, [sp, #64]
	ldr	x9, [x19, ___unnamed_1@PAGEOFF]
Lloh13:
	adrp	x0, l___unnamed_8@PAGE
Lloh14:
	add	x0, x0, l___unnamed_8@PAGEOFF
	ldr	w10, [x8]
	stp	x8, x10, [sp]
	blr	x9
	ldr	x8, [sp, #64]
	mov	w9, #102                        ; =0x66
Lloh15:
	adrp	x0, l___unnamed_9@PAGE
Lloh16:
	add	x0, x0, l___unnamed_9@PAGEOFF
	str	w9, [x8]
	ldr	x8, [sp, #64]
	ldr	x9, [x19, ___unnamed_1@PAGEOFF]
	ldr	w10, [x8]
	stp	x8, x10, [sp]
	blr	x9
	ldr	x9, [x19, ___unnamed_1@PAGEOFF]
	mov	w8, #101                        ; =0x65
Lloh17:
	adrp	x0, l___unnamed_10@PAGE
Lloh18:
	add	x0, x0, l___unnamed_10@PAGEOFF
	str	x8, [sp, #64]
	str	x8, [sp]
	blr	x9
	ldr	x8, [x19, ___unnamed_1@PAGEOFF]
	ldr	w9, [sp, #76]
Lloh19:
	adrp	x0, l___unnamed_11@PAGE
Lloh20:
	add	x0, x0, l___unnamed_11@PAGEOFF
	str	x9, [sp]
	blr	x8
	ldr	x9, [x19, ___unnamed_1@PAGEOFF]
	mov	w8, #200                        ; =0xc8
	add	x20, sp, #52
Lloh21:
	adrp	x0, l___unnamed_12@PAGE
Lloh22:
	add	x0, x0, l___unnamed_12@PAGEOFF
	str	w8, [sp, #52]
	stp	x8, x20, [sp]
	blr	x9
	ldr	x8, [x19, ___unnamed_1@PAGEOFF]
	ldr	w9, [sp, #52]
Lloh23:
	adrp	x0, l___unnamed_13@PAGE
Lloh24:
	add	x0, x0, l___unnamed_13@PAGEOFF
	stp	x9, x20, [sp]
	blr	x8
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	mov	w0, #1                          ; =0x1
	ldp	x20, x19, [sp, #80]             ; 16-byte Folded Reload
	add	sp, sp, #112
	ret
	.loh AdrpAdd	Lloh23, Lloh24
	.loh AdrpAdd	Lloh21, Lloh22
	.loh AdrpAdd	Lloh19, Lloh20
	.loh AdrpAdd	Lloh17, Lloh18
	.loh AdrpAdd	Lloh15, Lloh16
	.loh AdrpAdd	Lloh13, Lloh14
	.loh AdrpAdd	Lloh11, Lloh12
	.loh AdrpAdd	Lloh9, Lloh10
	.loh AdrpAdd	Lloh7, Lloh8
	.cfi_endproc
                                        ; -- End function
	.globl	___unnamed_1                    ; @0
.zerofill __DATA,__common,___unnamed_1,8,3
	.globl	___unnamed_2                    ; @1
.zerofill __DATA,__common,___unnamed_2,8,3
	.globl	___unnamed_3                    ; @2
.zerofill __DATA,__common,___unnamed_3,8,3
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_5:                          ; @3
	.asciz	"hello world"

l___unnamed_6:                          ; @4
	.asciz	"a: %p -- %d -- %p -- %p ( %d )\n"

l___unnamed_7:                          ; @5
	.asciz	"test"

l___unnamed_8:                          ; @6
	.asciz	"1. a: %p %d\n"

l___unnamed_9:                          ; @7
	.asciz	"2. a: %d %d\n"

l___unnamed_10:                         ; @8
	.asciz	"3. a: %d\n"

l___unnamed_11:                         ; @9
	.asciz	"foo: %d\n"

l___unnamed_12:                         ; @10
	.asciz	"b: %d -- %p\n"

l___unnamed_13:                         ; @11
	.asciz	"b: %d -- %p\n"

.subsections_via_symbols
