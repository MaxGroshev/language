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
push 4
call :1
pop  [3]
pop
push [3]
out
push 0
ret
