in
pop ax
call :prepare
call :factoria
push dx
out
hlt
:factoria
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
jne :factoria
ret
:prepare
push 1
pop dx
ret