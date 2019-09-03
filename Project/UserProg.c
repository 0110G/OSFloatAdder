#include <stdio.h>
#include "MyHeader.h"

int main(){
	float f1;
	float f2;
	scanf("%f %f",&f1,&f2);
	float addResult = GiveSum(f1,f2);
	printf("%f\n",addResult);
	return 0;
}
