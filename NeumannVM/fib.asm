fbeg 1
goto r1 16
fill r2 1
sub r1 r2
goto r1 13
pushreg r1
call 1
top r1
pop
fill r2 1
sub r1 r2
pushreg r6
call 1
top r5
pop
add r6 r5
goto 0 2
fill r6 1
fend
putstr Программа возвращает элемент последовательности Фибоначчи под заданным номером
putstr Введите номер:
read r1
call 1
putstr Число под этим номером равно:
print r6
stop
