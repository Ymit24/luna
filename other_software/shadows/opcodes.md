op    | zx zy no al op |
------------------------
X + 0 |  0  1  0  0  0 |
Y + 0 |  1  0  0  0  0 |
X & Y |  0  0  0  1  0 |
X | Y |  0  0  0  1  1 |
   ~X |  0  1  1  0  0 |
   ~Y |  1  0  1  0  0 |
X + Y |  0  0  0  0  0 |
X - Y |  0  0  0  0  1 |
Y - X |  0  1  1  0  1 | -> ~(x + ~y) !!!
    0 |  1  1  0  0  0 |
   -1 |  1  1  0  0  1 |
   -X |  0  1  1  0  0 |




HACK lang

 a=0 | a=1 | c c c c c c
------------------------
  0  |     | 1 0 1 0 1 0
  1  |     | 1 1 1 1 1 1
 –1  |     | 1 1 1 0 1 0
  D  |     | 0 0 1 1 0 0
  A  |   M | 1 1 0 0 0 0
 !D  |     | 0 0 1 1 0 1
 !A  |  !M | 1 1 0 0 0 1
 –D  |     | 0 0 1 1 1 1
 –A  |  -M | 1 1 0 0 1 1
D+1  |     | 0 1 1 1 1 1
A+1  | M+1 | 1 1 0 1 1 1
D–1  |     | 0 0 1 1 1 0
A–1  | M–1 | 1 1 0 0 1 0
D+A  | D+M | 0 0 0 0 1 0
D–A  | D–M | 0 1 0 0 1 1
A–D  | M–D | 0 0 0 1 1 1
D&A  | D&M | 0 0 0 0 0 0
D|A  | D|M | 0 1 0 1 0 1

enum ComputeInstructionType {
  Zero,
  One,
  NegOne,
  D,
  A,
  NotD,
  NotA,
  NegD,
  NegA,
  DPlus1,
  APlus1,
  DMin1,
  AMin1,
  DPlusA,
  DMinA,
  AMinD,
  DAndA,
  DOrA,
  M,
  NotM,
  NegM,
  MPlus1,
  MMin1,
  DPlusM,
  DMinM,
  MMinD,
  DAndM,
  DOrM,
}

enum Destination {
  Null,
  A,
  D,
  M,
  AM,
  DM,
  AD,
  AMD
}

enum Jump {
  Null,
  JGT,
  JEQ,
  JGE,
  JLT,
  JNE,
  JLE,
  JMP,
}
