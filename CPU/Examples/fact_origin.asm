push 5.5
in
pop ax
call :1
call :2
push dx
out
hlt
:2
push dx
push ax
mul
pop dx
push ax
push 1
sub
pop ax
push ax
push 0
jne :2
ret
:1
push 1
pop dx
ret
