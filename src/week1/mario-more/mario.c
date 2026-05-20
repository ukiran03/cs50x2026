#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

const char brick = '#';

string pre(int spaces, int bricks);
string post(int bricks);

int main(void) {

    int height; // get height from the user, between 1-9
    do {
        height = get_int("Height: ");
    } while (height < 1 || height > 8);

    for (int i = 1; i <= height; i++) {
        int spaces = height - i;

        string left_side = pre(spaces, i);
        string right_side = post(i);

        // print them together on one line with the gap in the middle
        printf("%s  %s\n", left_side, right_side);

        free(left_side);
        free(right_side);
    }
}

// pre: returns string with len(spaces) of trailing spaces, and bricks
string pre(int spaces, int bricks) {
    int length = spaces + bricks;
    string s = malloc(length + 1);
    if (s == NULL) {
        return NULL;
    }

    for (int i = 0; i < spaces; i++) {
        s[i] = ' ';
    }
    for (int i = 0; i < bricks; i++) {
        s[spaces + i] = '#';
    }

    s[length] = '\0';

    return s;
}

// post: return the len(bricks) without any trailing spaces
string post(int bricks) {
    string s = malloc(bricks + 1);
    if (s == NULL) {
        return NULL;
    }

    for (int i = 0; i < bricks; i++) {
        s[i] = '#';
    }

    s[bricks] = '\0';

    return s;
}
