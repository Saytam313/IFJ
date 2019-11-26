#1.
a = 7
if a == 6:
	print('NUM JE 6ka')
else:
	print('NUM NENI 6ka')

#2.
a = 7
if a == 6:
	print('NUM JE 6ka')
else:
	a = 2



#	1. 3 builtint_print error type: 2
#	2. 3 infix_postfix error type: 2
#	jiná chyba, ale stejný důvod řekl bych
#	jakmile je tělo else jako poslední řádek tak problém
# 	v případě že dá člověk něco ještě za to tělo else tak to funguje ok viz 3.

#3.
a = 7
if a == 6:
	print('NUM JE 6ka')
else:
	a = 2
print(a)