.global start
.section text
start:
 ld $0x40010000, %sp
 ld $10, %r1
 ld $99, %r2
 xchg %r1, %r2
 push %r1
 push %r2
 pop %r3
 pop %r4
 st %r1, value
 ld value, %r5
 halt
.section data
value:
 .word 0
.end