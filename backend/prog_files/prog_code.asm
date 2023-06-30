call :0
hlt
:1
pop  [2]
pop  [1]
pop  [0]
push [2]
ret
:0
push [3]
push 9
push 8
push 7
call :1
pop  [3]
pop
:3
push [3]
push 2
je  :4
push [3]
push 2
mul
push 12
jne :5
push [3]
out
:5
push [3]
push [3]
push 1
sub
pop  [3]
pop
jmp :3
:4
push 0
ret
