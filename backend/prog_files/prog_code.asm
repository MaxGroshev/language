push [0]
push 1
pop [0]
push [1]
push 2
pop [1]
push [2]
push 1
pop [2]
push [3]
push [1]
push [1]
mul
push 4
push [0]
mul
push [2]
mul
sub
pop [3]
push [4]
push -1
push [1]
mul
push [3]
sqrt
add
push 2
push [0]
mul
div
pop [4]
push [5]
push -1
push [1]
mul
push [3]
sqrt
sub
push 2
push [0]
mul
div
pop [5]
push [4]
push [5]
je :1 
push [4]
out
:1
push [5]
out
