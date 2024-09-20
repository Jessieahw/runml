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
void return 0.0; multiply (double a,double b){
double 	x = a*b
;

}
double  mul (double a,double b){
	return a + b
 ;

}

 int main() { 
custom_print(mul(12.0,6));
custom_print(multiply(10,5));
}
