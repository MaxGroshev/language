call :0
hlt
:1
pop  [0]
push [0]
push 1
jbe :2
push 1
pop ax
ret
:2
push [0]
push [0]
push 1
sub
call :1
push ax
mul
pop ax
ret
:0
push [1]
push 6
call :1
push ax
pop  [1]
pop
push [1]
out
push 5
pop ax
ret
