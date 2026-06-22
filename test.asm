# =========================================================
# DIRECTIVES
# =========================================================

.global start, testSymbol
.global sym1, sym2, sym3, DATAGLOB

.extern ext1 
.extern ext2, ext3, ext4

.extern symbol4


.section .text

p:
.skip 9
q:
.word testSymbol
testSymbol:
    .word 0xEEFABDCF
label_only:

another_label:


data_label: .skip 15
third_label:
.word 2
.section .data



DATAGLOB:
iret
.word 0 
.word 1, 2, 3
.word 0x10, 0x20, 0x30
.word sym1
.word sym1, sym2, sym3
.word 1, sym1, 2, sym2
.word testSymbol
post:
.skip 0
.skip 4
.skip 16

.ascii ""
.ascii "A"
.ascii "Hello"
.ascii "Assembler test 123"

.section .text

# =========================================================
# NO OPERAND INSTRUCTIONS
# =========================================================
sym2:
halt
int
iret
iret
iret
iret
ret

# =========================================================
# CALL / JMP
# =========================================================

call 123
call 0x100
call symbol_dest
halt
halt
halt
halt
halt
halt
symbol_dest_two:

jmp 456
jmp 0x200
jmp symbol_dest
jmp symbol_dest_two
jmp 0x200
jmp 0x200
jmp 0x200
jmp symbol_dest
jmp post
# =========================================================
# CONDITIONAL BRANCHES
# =========================================================

beq %r1, %r2, 100
beq %r3, %r4, branch_target

bne %r5, %r6, 200
bne %r7, %r8, branch_target

bgt %r9, %r10, 300
bgt %r11, %r12, branch_target

# =========================================================
# STACK
# =========================================================

push %r0
push %r1
push %r14
push %sp

pop %r2
pop %r3
pop %r15
pop %pc

# =========================================================
# XCHG
# =========================================================

xchg %r1, %r2
xchg %r14, %r15
xchg %sp, %pc
.ascii "text"

# =========================================================
# ARITHMETIC
# =========================================================

add %r4, %r15
sub %r3, %r4
mul %r5, %r6
div %r7, %r8

# =========================================================
# LOGICAL
# =========================================================

not %r9

and %r1, %r2
or  %r13, %r4
xor %r5, %r15

# =========================================================
# SHIFTS
# =========================================================

shl %r13, %r2
shr %r3, %r15

# =========================================================
# LD OPERANDS
# =========================================================
symbol3:

ld $5, %r1
ld $0x1234, %r2

ld $symbol1, %r3

ld 100, %r4
ld 0x200, %r5

ld symbol2, %r6

ld %r7, %r8

ld [%r1], %r2
ld [%r3], %r4

ld [%r5 + 4], %r6
ld [%r7 + 16], %r8

ld [%r9 + symbol3], %r10
# ld [%r11 + symbol4], %r12

# =========================================================
# ST OPERANDS
# =========================================================

st %r1, 100
st %r2, 0x300

st %r3, symbol5

st %r4, %r5

st %r6, [%r7]

st %r8, [%r9 + 8]
st %r10, [%r11 + 32]

st %r12, [%r13 + symbol6]
st %r14, [%sp + symbol7]

# =========================================================
# CSR
# =========================================================

csrrd %status, %r1
csrrd %handler, %r2
csrrd %cause, %r3

csrwr %r4, %status
csrwr %r5, %handler
csrwr %r6, %cause

# =========================================================
# LABEL + INSTRUCTION SAME LINE
# =========================================================

start: halt

lbl1: add %r1, %r2

lbl2: ld $123, %r3

lbl3: st %r4, [%r5 + 8]

# =========================================================
# COMMENTS
# =========================================================

# whole line comment

add %r1, %r2      # trailing comment

label_comment:    # comment after label

# =========================================================
# BRANCH TARGETS
# =========================================================

branch_target:
    halt

symbol_dest:
    ret

# =========================================================
# END
# =========================================================



halt
add %r1, %r2

.end

