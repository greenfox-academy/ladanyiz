#include <stdio.h>
#include <string.h>

struct Pirate {
    char name[256];
    int has_wooden_leg;
    short int gold_count;
};

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the sum of the gold of all pirates
int Gold(struct Pirate* _pirates, int size) {

    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += _pirates[i].gold_count;
    }

    return sum;
}

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the average of the gold / pirate
float Average(struct Pirate* _pirates, int size) {

    float avg = Gold(_pirates, size) / (float)size;

    return avg;
}

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the name of the
// richest that has wooden leg
char* RichestWoodenLegged(struct Pirate* _pirates, int size) {

    int gld = 0;
    int index = 0;

    for (int i = 0; i < size; i++){
        if (_pirates[i].has_wooden_leg==1 && _pirates[i].gold_count>gld) {
            index = i;
            gld = _pirates[i].gold_count;
        }
    }

    return _pirates[index].name;
}

int main() {

    struct Pirate pirates[] = {
        {"Jack", 0, 18},
        {"Uwe", 1, 8},
        {"Hook", 1, 12},
        {"Halloween kid", 0, 0},
        {"Sea Wolf", 1, 14},
        {"Morgan", 0, 1}
    };

    printf("The sum all the pirates' gold is %d\n", Gold(pirates, 6));
    printf("The average amount of gold one pirate has is %.2f\n", Average(pirates, 6));
    printf("The richest wooden-legged pirate is %s\n", RichestWoodenLegged(pirates, 6));

    return 0;
}
