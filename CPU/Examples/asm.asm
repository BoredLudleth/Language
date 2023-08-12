jump :main
:fact
var x
pop x
var y
push 1.000000
pop y
var z
push 1.000000
pop z
:while1
push z
push x
jbe :do_while1
jump :while_ex1
:do_while1
push y
push z
mul
pop y
push z
push 1.000000
add
pop z
jump :while1
:while_ex1
push z
pop ax
ret
:main
var x
push 0
pop x
var y
push 0
pop y
in
pop x
push x
push x
call :fact
pop x
push ax
pop y
push y
out
hlt
