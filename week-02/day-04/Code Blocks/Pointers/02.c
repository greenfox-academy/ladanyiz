#include <stdio.h>
#include <stdint.h>

int main() {
    int number = 1234;
    int* pNumber = &number;
    //TODO:
    // print the value of number using the "number_pointer"
    printf("The number is %d\n", *pNumber);
    return 0;
}
