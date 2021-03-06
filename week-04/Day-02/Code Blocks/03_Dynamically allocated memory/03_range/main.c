#include <stdio.h>
#include <stdlib.h>

/** TODO:
 * Create a function called "range" that creates a new array and returns a pointer to it.
 * It should fill the array with numbers based on it's parameters.
 * It should take 3 parameters:
 * int from: the first number in the array
 * int to: it will fill the array till it would reach this number (so this number is not in the array)
 * int step: the step between the numbers
 *
 * Examples:
 * range(0, 10, 2) -> {0, 2, 4, 6, 8}
 * range(1, -8, -3) -> {1, -2, -5}
 *
 * It should delete any dynamically allocated resource before the program exits.
 */

int size;

int *range(int from, int to, int step)
{
    size = (to - from) / step;
    if ((to - from) % step != 0)
        size++;

    // test
    // printf("%d\n", size);

    int *array = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++)
        array[i] = from + (step * i);

    return array;
}

int main()
{
    int *array = range(1, -8, -2);

    for (int i = 0; i < size; i++)
        printf("%d\t", array[i]);

    free(array);

    return 0;
}
