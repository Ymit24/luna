# ALU instruction
A/D/M = comp ; jmp

# Arith
form: dest = reg1 op reg2 ; jmp
ops = + , -
reg1 = A, D, M
reg2 = A, D, M
zx, zy, sw, no

# JMP
form: dest = reg1 op reg2 ; jmp
ops = GT, EQ, LT => GTE, LSE
reg1 = A, D, M, 1
reg2 = A, D, M, 1

# Logic
ops = & , |


Data Instruction
Instr Type literal---------------------------
15         14 13 12 11 10 9 8 7 6 5 4 3 2 1 0

Compute Instruction
Instr Type a  d  m  | r1a r1b r2a r2b | zx zy no AL | op j0 j1 j2
15         14 13 12 | 11  10  9   8   | 7  6  5  4  | 3  2  1  0

a: dest includes addr reg
d: dest includes data reg
m: dest includes mem reg (write to memory)

AL: arith or logic operation:

0 1
---
A L

r1a/r1b: two bit value determining register 1:

00 01 10 11
-----------
A  D  M  1

r2a/r2b: two bit value determining register 2:

00 01 10 11
-----------
A  D  M  1

zx: zero x input
zy: zero y input
no: negate output (logical negate)

op: op code for arith or logic operation

0 1
---
+& -|

j0/j1/j2: jump codes

000 001 010 011 100 101 110 111
-------------------------------
no  gt  eq  gte lt  neq leq unconditional

Examples:

A = 0d15      => 1000 0000 0000 1111
A = A+1       => 0100 0011 0000 0000
D = A-D       => 0010 0001 0000 0100
D = D-1       => 0010 0111 0000 0100
A = 0xff # load where we would jump too
_ = D&1 ; JGT => 0000 0111 0000 0010

