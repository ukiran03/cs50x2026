#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int POINTS[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                        1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int count_word(int length, string word);

int main(void) {
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    int score1 = count_word(strlen(word1), word1);
    int score2 = count_word(strlen(word2), word2);

    if (score1 > score2) {
        printf("Player 1 wins!\n");
    } else if (score1 < score2) {
        printf("Player 2 wins!\n");
    } else {
        printf("Tie!\n");
    }
}

int count_word(int length, string word) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (isupper(word[i])) {
            count += POINTS[word[i] - 'A'];
        } else if (islower(word[i])) {
            count += POINTS[word[i] - 'a'];
        }
    }
    return count;
}
