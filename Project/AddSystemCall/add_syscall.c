#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include "add_syscall.h"

/* 
	PreCondition:
	(1) num1 < 0 and num2 < 0
	(2) floats corresponding to each of the num must not be inf
	(3) floats corresponding to each of the num must not be nan
*/

/*
	PostCondition:
	(1) Return the int corresponding to the float addition of two floats
*/

SYSCALL_DEFINE2(add_syscall, int, n1, int, n2){
 
	// If any of the numbers is negative, return -1 as exit code
	if(n1 < 0 || n2 < 0){
		return -1;
	}
 
	// Calculate mantissa for n1 and n2
	// mantissa is first 23 bits of the representation
	int man1 = (1 << 23) - 1;
	man1 = man1 & n1;
	
	int man2 = (1 << 23) - 1;
	man2 = man2 & n2;
 
	// Calculate the exponent value for both n1 and n2
	// exponent is bit[23..30]
	int exp1;
	exp1 = n1 - man1;
	exp1 = exp1 >> 23;
	
	int exp2;
	exp2 = n2 - man2;
	exp2 = exp2 >> 23;
 
	// Case 1 : When one of the two numbers is infinity, return -2 as exit code
	// Condition : exp = 111.111, man = 000.000
	if((exp1 == ((1<<8)-1) && man1 == 0) || (exp2 == ((1<<8)-1) && man2 == 0)){
		return -2;
	} 
 
	// Case 2 : When one of two numbers is not a number, return -3 as exit code
	// Condition : exp = 111.111, man != 000.000
	if((exp1 == ((1<<8)-1) && man1 != 0) || (exp2 == ((1<<8)-1) && man2 != 0)){
		return -3;
	}
 
	// Case 3 : Regular encoding
	// Result has exponent bits exp1 (or exp2) and mantissa bits as (man1 + man2)
	if(exp1 == exp2){
		int tmp = (man1 + man2);
		exp1++;
		int res = (exp1 << 23) + (tmp >> 1);
		return res;
	}
 
	// We need to equalize the exp1 and exp2
	// Right shift man2 with exp1 - exp2 steps
	// Make exp2 = exp1
	// Result has exponent bits exp1 and mantissa bits as (man1 + man2)
	else if(exp1 > exp2){
		int res = 0;
 
		//Shift the man2 to the right with (exp1 - exp2)
		man2 = man2 + (1<<23);
		man2 = man2 >> (exp1 - exp2);
 
		// Equalize the exponents
		exp2 = exp1;
 
		//Add the two mantissa's
		int tmp = man1 + man2;
 
		// If carry occurs
		if(tmp >= (1<<23)){
			exp1++;
			tmp = tmp - (1<<23);
			res = (exp1 << 23) + (tmp >> 1);
		}
 
		// else simply add
		else{
			res = (exp1 << 23) + tmp;
		}
 
		return res;
	}
 
	// Similar to previous case
	else{			
		int res = 0;
 
		//Shift the man2 to the right with (exp1 - exp2)
		man1 = man1 + (1<<23);
		man1 = man1 >> (exp2 - exp1);
 
		// Equalize the exponents
		exp1 = exp2;
 
		//Add the two mantissa's
		int tmp = man1 + man2;
 
		// If carry occurs
		if(tmp >= (1<<23)){
			exp2++;
			tmp = tmp - (1<<23);
			res = (exp2 << 23) + (tmp >> 1);
		}
 
		// else simply add
		else{
			res = (exp2 << 23) + tmp;
		}	
 
		return res;	
	}
	return -4;
}
