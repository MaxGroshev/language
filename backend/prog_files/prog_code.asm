call :0
hlt
:1
pop  [0]
push [0]
out
push [0]
push 1
jbe :2
push 1
ret
:2
push [1]
push [0]
push [0]
push 1
sub
call :1
mul
pop  [1]
pop  ax
push [1]
out
push [1]
ret
:0
push [2]
push 4
pop  [2]
pop  ax
push [3]
push 10
call :1
pop  [3]
pop  ax
push [1]
out
push 5
ret
