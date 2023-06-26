	lw 0 1 eight	load 8 to reg 1 (symbolic)
	lw 1 2 7	load -2 to reg 2 (numeric)
	lw 0 3 stAddr	load 4 to reg 3 (symbolic)
	lw 0 4 zero	load 0 to reg 4 (symbolic)
start	add 1 2 1	add %1 + %2 -> %1
	beq 4 1 3	if %4 == %1 go to noop
	beq 0 0 1       go to jalr
	add 4 1 4	add %4 + %1 = %4
	jalr 3 5	go to start
	noop 
done halt
stAddr .fill start
zero .fill 0 
one .fill 1 
eight .fill 8 
neg2 .fill -2
