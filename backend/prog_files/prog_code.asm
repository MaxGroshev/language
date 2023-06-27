push [0]
push 1
pop [0]
push [1]
push 2
pop [1]
push [2]
push -1
push 3
mul
pop [2]
push [0]
push [1]
je  :0
push [0]
push [1]
jb  :1
push [1]
out
:1
push [0]
out
:0
push [2]
push 0
jb  :2
push [2]
push -1
push 1
mul
jb  :3
push [2]
out
:3
push [0]
out
:2
