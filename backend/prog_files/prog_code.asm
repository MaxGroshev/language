call :0
hlt
:1
push [0]
push [1]
push 90
pop  [1]
push [1]
out
push [1]
ret
:0
push [2]
push 91
pop  [2]
push [3]
push 2
pop  [3]
push [4]
push -1
push 300
mul
pop  [4]
call :1
push [2]
push [1]
je  :3
push [3]
call :1
out
:3
push 0
ret
