#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// index = 0.0588 * L - 0.296 * S - 15.8
// where
//  L is the average number of letters per 100 words in the text,
//  S is the average number of sentences per 100 words in the text.

float average(int s, int words) { return ((float)s / words) * 100; }

int main(void) {
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = 0;
    int words = 0;
    int sentences = 0;
    bool inside_word = false;

    // NOTE: In a string like "hello world", the character after the 'o' in
    // "hello" is a space (' '), not the null terminator ('\0'). The '\0'
    // character is only placed at the absolute end of the entire string, not
    // between words.

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        // count letters
        if (isalpha(c)) {
            letters++;
        }

        // count sentences
        if (c == '.' || c == '!' || c == '?') {
            sentences++;
        }

        // count words (state machine approach)
        if (isspace(c)) {
            inside_word = false;
        } else if (!inside_word) {
            inside_word = true;
            words++;
        }
    }

    float letter_avg = average(letters, words);     // L
    float sentence_avg = average(sentences, words); // S

    // Compute the Coleman-Liau index
    float coleman_index = (0.0588 * letter_avg) - (0.296 * sentence_avg) - 15.8;

    // Print the grade level
    int grade = (int)round(coleman_index);

    if (grade < 1) {
        printf("Before Grade 1\n");
    } else if (grade >= 16) {
        printf("Grade 16+\n");
    } else {
        printf("Grade %i\n", grade);
    }
}
