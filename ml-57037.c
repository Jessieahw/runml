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
double one = 1;
double  increment (double value){
return value + 0.0 ;

}

 int main() { 
custom_print(0.00.00.0);
}
