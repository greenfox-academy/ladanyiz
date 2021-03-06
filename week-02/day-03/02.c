#include <stdio.h>
#include <math.h>

/* Write a void function which has two float parameters. The function can do the
totoro operation on those parameters and it also prints out the result of the operation
on the screen.
The totoro operation does the following on two variables (let's say we have "a"
 and "b")
result = a*b+(a+b)+a*a*a+b*b*b+3.14159265358979
*/
void totoro(float a, float b) {

    float result = a*b + (a+b) + a*a*a + b*b*b + M_PI;

    printf("%.2f\n", result);
}

int main(){

    totoro(100, 902);

    return 0;
}
