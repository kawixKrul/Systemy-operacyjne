//
// Created by Michał Kawa on 08/03/2024.
//
int collatz_conjecture(int input) {
    if (input % 2 == 0) {
        return input/2;
    } else {
        return 3*input + 1;
    }
}

int test_collatz_convergence(int input, int max_iter) {
    int i = 0;
    while (input != 1 && i < max_iter) {
        input = collatz_conjecture(input);
        i++;
    }
    return i == max_iter && input != 1 ? -1 : i;
}