@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@R13
M=D
@0
D=A
@LCL
A=M
A=D+A
D=A
@14
M=D
@13
D=M
@14
A=M
M=D
(LOOP_START)
@0
D=A
@ARG
A=M
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@LCL
A=M
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=D+M
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@R13
M=D
@0
D=A
@LCL
A=M
A=D+A
D=A
@14
M=D
@13
D=M
@14
A=M
M=D
@0
D=A
@ARG
A=M
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M-D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@R13
M=D
@0
D=A
@ARG
A=M
A=D+A
D=A
@14
M=D
@13
D=M
@14
A=M
M=D
@0
D=A
@ARG
A=M
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@LOOP_START
D;JNE
@0
D=A
@LCL
A=M
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
