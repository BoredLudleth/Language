call :enter
call :disqr
call :xone
call :xtwo
hlt
:enter
in
in
in
pop cx
pop bx
pop ax
ret
:disqr
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
:xone
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
:xtwo
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