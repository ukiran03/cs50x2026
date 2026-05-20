#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SET_SIZE 10

// struct to hold both arrays and their respective sizes
typedef struct {
    // contains second-last, fourth-last, etc.
    int setA[MAX_SET_SIZE];
    int sizeA;

    // contains all other numbers
    int setB[MAX_SET_SIZE];
    int sizeB;
} Split;

Split split_long(long num) {
    Split result;
    result.sizeA = 0;
    result.sizeB = 0;

    int temp_digits[2 * MAX_SET_SIZE];
    int total_digits = 0;

    // extract digits from right to left
    if (num == 0) {
        temp_digits[total_digits++] = 0;
    } else {
        while (num > 0) {
            temp_digits[total_digits++] = num % 10;
            num /= 10;
        }
    }

    // distribute digits based on their position from the right.
    // index 0 is the last digit, Index 1 is the second-last digit, etc.
    for (int i = 0; i < total_digits; i++) {
        if (i % 2 == 1) {
            // Second-last (i=1), fourth-last (i=3), etc. goes to setA
            result.setA[result.sizeA++] = temp_digits[i];
        } else {
            result.setB[result.sizeB++] = temp_digits[i];
        }
    }

    return result;
}

int setA_double_digits_sum(int *arr, size_t n);
int setB_digits_sum(int *arr, size_t n);
int first_2d(long num);
void validate_card(int num, int length);
int get_long_length(long num);

int main(void) {
    long seq;
    do {
        seq = get_long("Number: ");
    } while (seq < 0); // check for -ve, other long checks done by get_long

    Split sets = split_long(seq);

    int aSum = setA_double_digits_sum(sets.setA, sets.sizeA);
    int bSum = setB_digits_sum(sets.setB, sets.sizeB);

    if ((aSum + bSum) % 10 == 0) {
        validate_card(first_2d(seq), get_long_length(seq));
    } else {
        printf("INVALID\n");
    }
}

int get_long_length(long num) {
    int count = 0;

    // Handle the edge case where the number is exactly 0
    if (num == 0)
        return 1;

    // Convert negative numbers to positive to count digits accurately
    num = labs(num);

    while (num > 0) {
        num /= 10; // Remove the last digit
        count++;   // Increment the digit counter
    }

    return count;
}

int first_2d(long num) {
    // reduce the number until only 2 digits are left
    while (num >= 100) {
        num /= 10;
    }

    return (int)num;
}

void validate_card(int num, int length) {
    // AMEX uses 34 or 37 and is 15 digits long
    if ((num == 34 || num == 37) && (length == 15)) {
        printf("AMEX\n");
    }
    // Mastercard uses 51 through 55 and is 16 digits long
    else if ((num >= 51 && num <= 55) && (length == 16)) {
        printf("MASTERCARD\n");
    }
    // Visa starts with 4 (2-digit equivalent: 40 to 49) and is 13 or 16 digits
    // long
    else if ((num >= 40 && num <= 49) && ((length == 13) || (length == 16))) {
        printf("VISA\n");
    } else {
        printf("INVALID\n");
    }
}

int setA_double_digits_sum(int *arr, size_t n) {
    if (!arr || n == 0)
        return 0;

    int digits_sum = 0;

    for (size_t i = 0; i < n; i++) {
        int num = arr[i] * 2;

        while (num >= 10) {
            digits_sum += num % 10;
            num /= 10;
        }
        digits_sum += num;
    }

    return digits_sum;
}

int setB_digits_sum(int *arr, size_t n) {
    int digits_sum = 0;

    for (size_t i = 0; i < n; i++) {
        digits_sum += arr[i];
    }

    return digits_sum;
}
