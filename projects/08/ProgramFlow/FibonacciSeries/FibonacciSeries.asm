@1
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
@R13
M=D
@1
D=A
@3
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
@THAT
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
@R13
M=D
@1
D=A
@THAT
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
@2
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
(MAIN_LOOP_START)
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
@COMPUTE_ELEMENT
D;JNE
@END_PROGRAM
0;JMP
(COMPUTE_ELEMENT)
@0
D=A
@THAT
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
@THAT
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
@2
D=A
@THAT
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
@1
D=A
@3
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
M=D+M
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@R13
M=D
@1
D=A
@3
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
@MAIN_LOOP_START
0;JMP
(END_PROGRAM)
