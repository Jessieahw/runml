#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void custom_print(double value) {
    double int_part, frac_part;
    frac_part = modf(value, &int_part);
    if (frac_part == 0.0) {
        printf("%.0f\n", value);
    } else {
        printf("%.6f\n", value);
    }
}
double  multiply (double a,double b){
double 	x = a*b
;
return 0.0;
}
double  mul (double a,double b){
	return a + b
 ;

}

 int main() { 
multiply(1,2);
multiply(3,2);
multiply(3,2;
custom_print(mul(10,5));
}
