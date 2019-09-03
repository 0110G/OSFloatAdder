#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
 
int GiveIntFromFloat(float f);
float GiveFloatFromInt(int n);
float GiveSum(float f1, float f2);
float Abs(float f);
void Test(void);
void PrintBits(int x1);
 
// Union stores the float and int in the same address.
// Hence an easy interconversion of float and int could
// be possible using unions
typedef union{
	float f;
	int x;
}int_cast;
 
// Prints the 32 bit integer. Used for debugging purposes
void PrintBits(int x1){
	int arr[32] = {0};
	int couter = 31;
	int tmp = x1;
	while(tmp){
		arr[couter] = tmp%2;
		tmp = tmp/2;
		couter--;
	}
	for(int i=0 ; i<32 ; i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}
 
// Used to convert given integer into a float with same
// bitset and following IEEE 754 format of representation.
float GiveFloatFromInt(int n){
	int_cast in1;
	in1.x = n;
	return in1.f;
}
 
// Used to convert given float - represented in into
// IEEE 754 format into an integer with the same bitset.
int GiveIntFromFloat(float f){
	int_cast in1;
	in1.f = f;
	return in1.x;
}

// Wrapper function to use given syscall
float GiveSum(float f1, float f2){
	int x1 = GiveIntFromFloat(f1);
    	int x2 = GiveIntFromFloat(f2);
 	int res = syscall(434,x1,x2);
 	if(res == -1){
 		return -1;
 	}
 	else if(res == -2){
 		return -2;
 	}
 	else if(res == -3){
 		return -3;
 	}
 	else{
 		return GiveFloatFromInt(res);
 	}
}

// Returns the absolute value of the given float
float Abs(float f){
	if(f < 0){return -f;}
	return f;
}

// Test the given function using 2 randomly generated
// floating point numbers
void Test(){
	float f1,f2;
	srand((int)time(NULL));
	float a = 254.1026;
	 for (int i=0 ; i<10000 ; i++){
	 	f1 = Abs(((float)rand()/(float)(RAND_MAX))*a);
	 	f2 = Abs(((float)rand()/(float)(RAND_MAX))*a);
	 	float ret = GiveSum(f1,f2);
	 	float exp = f1 + f2;
	 	float del = Abs(exp - ret);
	 	if(del > 0.001){
	 		printf("Error Occured!\nNum1: %f, Num2: %f ; Expected: %f, Returned: %f\n", f1,f2,exp,ret);
	 		break;
	 	}
	 	printf("Passed!\n");
	 }
}
