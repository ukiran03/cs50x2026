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
    } while (seq < 10); // eases catching first two digits

    long temp = seq;
    int long_len = 0;
    int luhn_sum = 0;
    int first_twod = 0;

    while (temp > 0) {
        // get digits from backward
        int digit = temp % 10;

        // catch the odd digits from backward (eg., 1, 3, etc)
        if (long_len % 2 == 1) {
            int doubled = digit * 2; // we get 1 or 2 digits number
            // if its two, we agian split digits and sum them.
            luhn_sum += (doubled % 10) + (doubled / 10);
        } else {
            // catches even digits from backward, and sum.
            luhn_sum += digit;
        }

        // capture the first 2 digits as we divide down
        if (temp > 10 && temp < 100) {
            first_twod = temp;
        }

        long_len++; // advancement
        temp = temp / 10;
    }

    // final validation and determination
    if (luhn_sum % 10 == 0 && long_len > 0) {
        validate_card(first_twod, long_len);
    } else {
        printf("INVALID\n");
    }
    return 0;
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
