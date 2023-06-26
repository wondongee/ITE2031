	lw 0 1 ten	Load 10 to reg 1 (numeric)
	lw 0 2 neg1	load -2 to reg 2 (symbolic)
	lw 1 3 2	load -1 to reg 3 (numeric)
	sw 0 4 zero	store 0 to reg 4 (symbolic)
start	add 2 1 1	add %2 + %1 -> %1
	beq 1 3 done	if %1 == %3 break
	beq 0 0 start go to loop
	noop 
done halt 
zero .fill 0 
one .fill 1 
ten .fill 10 
neg1 .fill -1
