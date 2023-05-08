call :1
call :2
call :3
call :4
hlt
:1
in
in
in
pop cx
pop bx
pop ax
ret
:2
push bx
push bx
mul
push 4
push ax
push cx
mul
mul
sub
sqrt
pop dx
ret
:3
push 0
push bx
sub
push dx
add
push ax
div
push 2
div
out
ret
:4
push 0
push bx
sub
push dx
sub
push ax
div
push 2
div
out
ret
