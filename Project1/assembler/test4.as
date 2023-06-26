	lw 0 1 ten	reg 1 has a 8
	lw 1 2 -2	reg 2 has a -10
	lw 0 3 neg1	reg 3 has a -1
	nor 1 2 4	reg 4 has a %1 nor %2
	beq 1 4 done	if %1 == %4 break
	add 1 3 1	add %1 + %3 -> %1
	beq 1 1 -3	go to loop
done	halt
neg10	.fill -10
ten	.fill 10
neg1	.fill -1
