llvm-objdump --triple=aarch64-linux-gnu -S cfi

cfi:	file format ELF64-aarch64-little


Disassembly of section .init:

0000000000400418 _init:
  400418: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  40041c: fd 03 00 91                  	mov	x29, sp
  400420: 30 00 00 94                  	bl	#192 <call_weak_fn>

0000000000400424 $x:
  400424: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  400428: c0 03 5f d6                  	ret

Disassembly of section .plt:

0000000000400430 $x:
  400430: f0 7b bf a9                  	stp	x16, x30, [sp, #-16]!
  400434: 90 00 00 90                  	adrp	x16, #65536
  400438: 11 fe 47 f9                  	ldr	x17, [x16, #4088]
  40043c: 10 e2 3f 91                  	add	x16, x16, #4088
  400440: 20 02 1f d6                  	br	x17
  400444: 1f 20 03 d5                  	nop
  400448: 1f 20 03 d5                  	nop
  40044c: 1f 20 03 d5                  	nop

0000000000400450 __libc_start_main@plt:
  400450: 90 00 00 b0                  	adrp	x16, #69632
  400454: 11 02 40 f9                  	ldr	x17, [x16]
  400458: 10 02 00 91                  	add	x16, x16, #0
  40045c: 20 02 1f d6                  	br	x17

0000000000400460 __gmon_start__@plt:
  400460: 90 00 00 b0                  	adrp	x16, #69632
  400464: 11 06 40 f9                  	ldr	x17, [x16, #8]
  400468: 10 22 00 91                  	add	x16, x16, #8
  40046c: 20 02 1f d6                  	br	x17

0000000000400470 abort@plt:
  400470: 90 00 00 b0                  	adrp	x16, #69632
  400474: 11 0a 40 f9                  	ldr	x17, [x16, #16]
  400478: 10 42 00 91                  	add	x16, x16, #16
  40047c: 20 02 1f d6                  	br	x17

0000000000400480 printf@plt:
  400480: 90 00 00 b0                  	adrp	x16, #69632
  400484: 11 0e 40 f9                  	ldr	x17, [x16, #24]
  400488: 10 62 00 91                  	add	x16, x16, #24
  40048c: 20 02 1f d6                  	br	x17

Disassembly of section .text:

0000000000400490 _start:
  400490: 1d 00 80 d2                  	mov	x29, #0
  400494: 1e 00 80 d2                  	mov	x30, #0
  400498: e5 03 00 aa                  	mov	x5, x0
  40049c: e1 03 40 f9                  	ldr	x1, [sp]
  4004a0: e2 23 00 91                  	add	x2, sp, #8
  4004a4: e6 03 00 91                  	mov	x6, sp
  4004a8: 00 00 e0 d2                  	movz	x0, #0, lsl #48
  4004ac: 00 00 c0 f2                  	movk	x0, #0, lsl #32
  4004b0: 00 08 a0 f2                  	movk	x0, #64, lsl #16
  4004b4: 00 c8 80 f2                  	movk	x0, #1600
  4004b8: 03 00 e0 d2                  	movz	x3, #0, lsl #48
  4004bc: 03 00 c0 f2                  	movk	x3, #0, lsl #32
  4004c0: 03 08 a0 f2                  	movk	x3, #64, lsl #16
  4004c4: 03 d4 80 f2                  	movk	x3, #1696
  4004c8: 04 00 e0 d2                  	movz	x4, #0, lsl #48
  4004cc: 04 00 c0 f2                  	movk	x4, #0, lsl #32
  4004d0: 04 08 a0 f2                  	movk	x4, #64, lsl #16
  4004d4: 04 e4 80 f2                  	movk	x4, #1824
  4004d8: de ff ff 97                  	bl	#-136 <__libc_start_main@plt>
  4004dc: e5 ff ff 97                  	bl	#-108 <abort@plt>

00000000004004e0 call_weak_fn:
  4004e0: 80 00 00 90                  	adrp	x0, #65536
  4004e4: 00 f0 47 f9                  	ldr	x0, [x0, #4064]
  4004e8: 40 00 00 b4                  	cbz	x0, #8 <call_weak_fn+0x10>
  4004ec: dd ff ff 17                  	b	#-140 <__gmon_start__@plt>
  4004f0: c0 03 5f d6                  	ret
  4004f4: 1f 20 03 d5                  	nop

00000000004004f8 deregister_tm_clones:
  4004f8: 80 00 00 b0                  	adrp	x0, #69632
  4004fc: 00 c0 00 91                  	add	x0, x0, #48
  400500: 81 00 00 b0                  	adrp	x1, #69632
  400504: 21 c0 00 91                  	add	x1, x1, #48
  400508: 3f 00 00 eb                  	cmp	x1, x0
  40050c: c0 00 00 54                  	b.eq	#24 <deregister_tm_clones+0x2c>
  400510: 01 00 00 90                  	adrp	x1, #0
  400514: 21 a0 43 f9                  	ldr	x1, [x1, #1856]
  400518: 61 00 00 b4                  	cbz	x1, #12 <deregister_tm_clones+0x2c>
  40051c: f0 03 01 aa                  	mov	x16, x1
  400520: 00 02 1f d6                  	br	x16
  400524: c0 03 5f d6                  	ret

0000000000400528 register_tm_clones:
  400528: 80 00 00 b0                  	adrp	x0, #69632
  40052c: 00 c0 00 91                  	add	x0, x0, #48
  400530: 81 00 00 b0                  	adrp	x1, #69632
  400534: 21 c0 00 91                  	add	x1, x1, #48
  400538: 21 00 00 cb                  	sub	x1, x1, x0
  40053c: 22 fc 7f d3                  	lsr	x2, x1, #63
  400540: 41 0c 81 8b                  	add	x1, x2, x1, asr #3
  400544: ff 07 81 eb                  	cmp	xzr, x1, asr #1
  400548: 21 fc 41 93                  	asr	x1, x1, #1
  40054c: c0 00 00 54                  	b.eq	#24 <register_tm_clones+0x3c>
  400550: 02 00 00 90                  	adrp	x2, #0
  400554: 42 a4 43 f9                  	ldr	x2, [x2, #1864]
  400558: 62 00 00 b4                  	cbz	x2, #12 <register_tm_clones+0x3c>
  40055c: f0 03 02 aa                  	mov	x16, x2
  400560: 00 02 1f d6                  	br	x16
  400564: c0 03 5f d6                  	ret

0000000000400568 __do_global_dtors_aux:
  400568: fd 7b be a9                  	stp	x29, x30, [sp, #-32]!
  40056c: fd 03 00 91                  	mov	x29, sp
  400570: f3 0b 00 f9                  	str	x19, [sp, #16]
  400574: 93 00 00 b0                  	adrp	x19, #69632
  400578: 60 c2 40 39                  	ldrb	w0, [x19, #48]
  40057c: 80 00 00 35                  	cbnz	w0, #16 <__do_global_dtors_aux+0x24>
  400580: de ff ff 97                  	bl	#-136 <deregister_tm_clones>
  400584: 20 00 80 52                  	mov	w0, #1
  400588: 60 c2 00 39                  	strb	w0, [x19, #48]
  40058c: f3 0b 40 f9                  	ldr	x19, [sp, #16]
  400590: fd 7b c2 a8                  	ldp	x29, x30, [sp], #32
  400594: c0 03 5f d6                  	ret

0000000000400598 frame_dummy:
  400598: e4 ff ff 17                  	b	#-112 <register_tm_clones>

000000000040059c foo_impl.cfi:
; int foo_impl(void) {
  40059c: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  4005a0: fd 03 00 91                  	mov	x29, sp
; 	res = printf("\nfoo_impl has been called !\n\n");
  4005a4: 00 00 00 90                  	adrp	x0, #0
  4005a8: 00 40 1d 91                  	add	x0, x0, #1872
  4005ac: b5 ff ff 97                  	bl	#-300 <printf@plt>
; 	return res + 42; 
  4005b0: 00 a8 00 11                  	add	w0, w0, #42
  4005b4: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  4005b8: c0 03 5f d6                  	ret

00000000004005bc foo_impl_2.cfi:
; int foo_impl_2(void) {
  4005bc: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  4005c0: fd 03 00 91                  	mov	x29, sp
; 	res = printf("\nfoo_impl_2 has been called !\n\n");
  4005c4: 00 00 00 90                  	adrp	x0, #0
  4005c8: 00 b8 1d 91                  	add	x0, x0, #1902
  4005cc: ad ff ff 97                  	bl	#-332 <printf@plt>
; 	return res + 42 + 2; 
  4005d0: 00 b0 00 11                  	add	w0, w0, #44
  4005d4: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  4005d8: c0 03 5f d6                  	ret

00000000004005dc foo_impl_3.cfi:
; int foo_impl_3(void) {
  4005dc: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  4005e0: fd 03 00 91                  	mov	x29, sp
; 	res = printf("\nfoo_impl_3 has been called !\n\n");
  4005e4: 00 00 00 90                  	adrp	x0, #0
  4005e8: 00 38 1e 91                  	add	x0, x0, #1934
  4005ec: a5 ff ff 97                  	bl	#-364 <printf@plt>
; 	return res + 3000; 
  4005f0: 00 e0 2e 11                  	add	w0, w0, #3000
  4005f4: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  4005f8: c0 03 5f d6                  	ret

00000000004005fc foo_impl_4.cfi:
; int foo_impl_4(void) {
  4005fc: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  400600: fd 03 00 91                  	mov	x29, sp
; 	res = printf("\nfoo_impl_4 has been called !\n\n");
  400604: 00 00 00 90                  	adrp	x0, #0
  400608: 00 b8 1e 91                  	add	x0, x0, #1966
  40060c: 9d ff ff 97                  	bl	#-396 <printf@plt>
; 	return res + 4000; 
  400610: 00 80 3e 11                  	add	w0, w0, #4000
  400614: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  400618: c0 03 5f d6                  	ret

000000000040061c get_implementation:
; 	if (choice == 1)
  40061c: 08 04 00 51                  	sub	w8, w0, #1
  400620: 1f 0d 00 71                  	cmp	w8, #3
  400624: a8 00 00 54                  	b.hi	#20 <get_implementation+0x1c>
  400628: 09 00 00 90                  	adrp	x9, #0
  40062c: 29 01 20 91                  	add	x9, x9, #2048
  400630: 20 d9 68 f8                  	ldr	x0, [x9, w8, sxtw #3]
  400634: c0 03 5f d6                  	ret
; }
  400638: e0 03 1f aa                  	mov	x0, xzr
  40063c: c0 03 5f d6                  	ret

0000000000400640 main:
; int main(int argc, char **argv) {
  400640: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  400644: fd 03 00 91                  	mov	x29, sp
; 	foo = get_implementation(argc);
  400648: f5 ff ff 97                  	bl	#-44 <get_implementation>
; 	return printf("Indirect call of function foo returned: %d\n", foo());
  40064c: 0a 00 00 90                  	adrp	x10, #0
  400650: 4a 31 1a 91                  	add	x10, x10, #1676
  400654: 0a 00 0a cb                  	sub	x10, x0, x10
  400658: 4a 09 ca 93                  	ror	x10, x10, #2
; 	foo = get_implementation(argc);
  40065c: 89 00 00 b0                  	adrp	x9, #69632
; 	return printf("Indirect call of function foo returned: %d\n", foo());
  400660: 5f 11 00 f1                  	cmp	x10, #4
; 	foo = get_implementation(argc);
  400664: 20 1d 00 f9                  	str	x0, [x9, #56]
; 	return printf("Indirect call of function foo returned: %d\n", foo());
  400668: 02 01 00 54                  	b.hs	#32 <main+0x48>
  40066c: 00 00 3f d6                  	blr	x0
  400670: e1 03 00 2a                  	mov	w1, w0
  400674: 00 00 00 90                  	adrp	x0, #0
  400678: 00 38 1f 91                  	add	x0, x0, #1998
  40067c: 81 ff ff 97                  	bl	#-508 <printf@plt>
  400680: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  400684: c0 03 5f d6                  	ret
  400688: 20 00 20 d4                  	brk	#0x1

000000000040068c foo_impl:
  40068c: c4 ff ff 17                  	b	#-240 <foo_impl.cfi>

0000000000400690 foo_impl_2:
  400690: cb ff ff 17                  	b	#-212 <foo_impl_2.cfi>

0000000000400694 foo_impl_3:
  400694: d2 ff ff 17                  	b	#-184 <foo_impl_3.cfi>

0000000000400698 foo_impl_4:
  400698: d9 ff ff 17                  	b	#-156 <foo_impl_4.cfi>
  40069c: 1f 20 03 d5                  	nop

00000000004006a0 __libc_csu_init:
  4006a0: fd 7b bc a9                  	stp	x29, x30, [sp, #-64]!
  4006a4: fd 03 00 91                  	mov	x29, sp
  4006a8: f3 53 01 a9                  	stp	x19, x20, [sp, #16]
  4006ac: 94 00 00 90                  	adrp	x20, #65536
  4006b0: 94 02 38 91                  	add	x20, x20, #3584
  4006b4: f5 5b 02 a9                  	stp	x21, x22, [sp, #32]
  4006b8: 95 00 00 90                  	adrp	x21, #65536
  4006bc: b5 e2 37 91                  	add	x21, x21, #3576
  4006c0: 94 02 15 cb                  	sub	x20, x20, x21
  4006c4: f6 03 00 2a                  	mov	w22, w0
  4006c8: f7 63 03 a9                  	stp	x23, x24, [sp, #48]
  4006cc: f7 03 01 aa                  	mov	x23, x1
  4006d0: f8 03 02 aa                  	mov	x24, x2
  4006d4: 51 ff ff 97                  	bl	#-700 <_init>
  4006d8: ff 0f 94 eb                  	cmp	xzr, x20, asr #3
  4006dc: 60 01 00 54                  	b.eq	#44 <__libc_csu_init+0x68>
  4006e0: 94 fe 43 93                  	asr	x20, x20, #3
  4006e4: 13 00 80 d2                  	mov	x19, #0
  4006e8: a3 7a 73 f8                  	ldr	x3, [x21, x19, lsl #3]
  4006ec: e2 03 18 aa                  	mov	x2, x24
  4006f0: 73 06 00 91                  	add	x19, x19, #1
  4006f4: e1 03 17 aa                  	mov	x1, x23
  4006f8: e0 03 16 2a                  	mov	w0, w22
  4006fc: 60 00 3f d6                  	blr	x3
  400700: 9f 02 13 eb                  	cmp	x20, x19
  400704: 21 ff ff 54                  	b.ne	#-28 <__libc_csu_init+0x48>
  400708: f3 53 41 a9                  	ldp	x19, x20, [sp, #16]
  40070c: f5 5b 42 a9                  	ldp	x21, x22, [sp, #32]
  400710: f7 63 43 a9                  	ldp	x23, x24, [sp, #48]
  400714: fd 7b c4 a8                  	ldp	x29, x30, [sp], #64
  400718: c0 03 5f d6                  	ret
  40071c: 1f 20 03 d5                  	nop

0000000000400720 __libc_csu_fini:
  400720: c0 03 5f d6                  	ret

Disassembly of section .fini:

0000000000400724 _fini:
  400724: fd 7b bf a9                  	stp	x29, x30, [sp, #-16]!
  400728: fd 03 00 91                  	mov	x29, sp

000000000040072c $x:
  40072c: fd 7b c1 a8                  	ldp	x29, x30, [sp], #16
  400730: c0 03 5f d6                  	ret
