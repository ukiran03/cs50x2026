#include <cs50.h>
#include <stdio.h>

#define AMEX_LENGTH 15
#define VISA_SHORT_LENGTH 13
#define VISA_LONG_LENGTH 16
#define MASTERCARD_LENGTH 16

void validate_card(int prefix, int length);

int main(void) {
    long seq;
    do {
        seq = get_long("Number: ");
    } while (seq < 0);

    long temp = seq;
    int length = 0;
    int luhn_sum = 0;
    int first_two = 0;

    // Single-pass loop: extracts length, prefix, and checksum all at once
    while (temp > 0) {
        int digit = temp % 10;

        // Luhn's Algorithm: length tracks position from the right (0-indexed)
        if (length % 2 == 1) {
            // Alternating digits (second-to-last, fourth-to-last, etc.)
            int doubled = digit * 2;
            luhn_sum += (doubled % 10) + (doubled / 10);
        } else {
            // All other digits
            luhn_sum += digit;
        }

        // Capture the first two digits as we divide down
        if (temp >= 10 && temp < 100) {
            first_two = temp;
        }

        length++;
        temp /= 10;
    }

    // Handle edge case for single-digit inputs (like 4) where temp never falls
    // between 10 and 100
    if (length == 1) {
        first_two = seq;
    }

    // Final Validation check
    if (luhn_sum % 10 == 0 && length > 0) {
        validate_card(first_two, length);
    } else {
        printf("INVALID\n");
    }
}

void validate_card(int prefix, int length) {
    // AMEX: Starts with 34 or 37, 15 digits long
    if ((prefix == 34 || prefix == 37) && (length == AMEX_LENGTH)) {
        printf("AMEX\n");
    }
    // MASTERCARD: Starts with 51 through 55, 16 digits long
    else if ((prefix >= 51 && prefix <= 55) && (length == MASTERCARD_LENGTH)) {
        printf("MASTERCARD\n");
    }
    // VISA: Starts with 4 (prefix 40 through 49), 13 or 16 digits long
    else if ((prefix >= 40 && prefix <= 49) &&
             (length == VISA_SHORT_LENGTH || length == VISA_LONG_LENGTH)) {
        printf("VISA\n");
    } else {
        printf("INVALID\n");
    }
}
