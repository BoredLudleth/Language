call :prepare
in
pop cx
push cx
push 1
je :answer1
push cx
push 2
je :answer2
push cx
push cx
push 2
sub
pop cx
call :fibo
push bx
out
hlt

:prepare
push 1
push 0
pop ax
pop bx
ret

:fibo
push ax
push bx
add
push bx
pop ax
pop bx
push cx
push 1
sub
pop cx
push cx
push 0
jne :fibo
ret

:answer1
push 0
out
hlt

:answer2
push 1
out
hlt