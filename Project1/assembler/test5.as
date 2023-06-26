	lw 0 1 Steen	Load 16 to reg 1 (symbolic)
	lw 0 2 neg2	load -2 to reg 2 (symbolic)
	lw 0 3 11	load 0 to reg 3 (numeric)
	sw 0 4 zero	store 0 to reg 4 (symbolic)
	sw 0 5 one	store 1 to reg 5 (symbolic)
start	add 1 2 1	add %1 + %2 -> %1
	beq 1 2 2	if %1 == 1 break
	beq 0 0 -3       go to loop
	add 4 1 4	add %4 + %1 = %4
	noop 
done halt 
zero .fill 0 
one .fill 1 
Steen .fill 16
neg2 .fill -2
