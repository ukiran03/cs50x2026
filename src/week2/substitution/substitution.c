#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[]) {

    // Get key
    if (argc != 2) {
        printf("Usage: %s KEY\n", argv[0]);
        return 1;
    }
    string key = argv[1];

    // Validate key
    int key_length = strlen(key);
    if (key_length != 26) {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    int seen_charmap[26] = {0}; // seen characters map

    for (int i = 0; i < key_length; i++) {
        char c = key[i];
        if (!isalpha(c)) {
            printf("Key must only contain alphabetic characters\n");
            return 1;
        }

        // Map 'a'-'z' or 'A'-'Z' to an index of 0-25
        int index = tolower(c) - 'a';
        if (seen_charmap[index] == 1) {
            printf("Key must not contain repeated characters\n");
            return 1;
        } else {
            seen_charmap[index] = 1;
        }
    }

    // Get Plaintext msg
    string plaintxt = get_string("plaintext: ");
    int plain_len = strlen(plaintxt);

    // Encipher and Print ciphertext instantly
    printf("ciphertext: ");
    for (int i = 0; i < plain_len; i++) {
        char pchar = plaintxt[i];

        // NOTE: key is case-insensitive, but the given plaintext is strictly
        // case-sensitive.
        if (isalpha(pchar)) {
            if (isupper(pchar)) {
                // find alphabet index (e.g., 'C' - 'A' = 2)
                int idx = pchar - 'A';
                printf("%c", toupper(key[idx]));
            } else {
                // find alphabet index (e.g., 'c' - 'a' = 2)
                int idx = pchar - 'a';
                printf("%c", tolower(key[idx]));
            }
        } else {
            // Non-alphabetical characters persist
            printf("%c", pchar);
        }
    }

    printf("\n");
    return 0;
}

// Encipher text rules:
//   - Non-Alphabetic chars should persist (spaces, marks etc)
//   - case of letters should persist

// Key is case-insensitive.
// Validate key rules:
//   - check key length (must be 26)
//   - check for non-alphabetic chars
//   - check for repeated chars (case-insensitive)
