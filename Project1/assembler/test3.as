	lw 0 1 15	Load 16 to reg 1 (numeric)
	lw 0 2 neg4	load -4 to reg 2 (symbolic)
	lw 1 3 -2	load 1 to reg 3 (numeric)
	lw 0 4 nineT	load 19 to reg 4 (symbolic)
	lw 0 5 seven	load 7 to reg 5 (symbolic)
start	add 1 2 1	add %1 + %2 -> %1
	jalr 4 5	go to jalr
	beq 1 2 done	if %1 == %2 break
	beq 0 0 start   go to start
	add 4 1 4	add %4 + %1 = %4
	noop 
done halt
six .fill 6
seven .fill 7
one .fill 1
sixT .fill 16
nineT .fill 19
neg4 .fill -4
	sw 0 1 one	store 1 to reg 1 (symbolic)
	jalr 5 6	go to start
