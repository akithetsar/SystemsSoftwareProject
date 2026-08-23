.global start
.section text
start:
 ld $5, %r1
 ld $3, %r2
 add %r2, %r1
 sub %r2, %r1
 mul %r2, %r1
 div %r2, %r1
 not %r1
 ld $0xFF, %r3
 and %r3, %r1
 or %r3, %r1
 xor %r3, %r1
 ld $1, %r4
 shl %r4, %r3
 shr %r4, %r3
 halt
.end