#include <stdio.h>
#include <stdint.h>

int main() {
    int high_number = 6655;
    int low_number = 2;

    int* pHigh_number = &high_number;
    int* other_high_number_pointer;
    //TODO:
    // The "other_high_number_pointer" should point to the same memory address
    // without using the "&" operator.
    other_high_number_pointer = pHigh_number;
    printf("The high number is %d\n", *other_high_number_pointer);

    return 0;
}
