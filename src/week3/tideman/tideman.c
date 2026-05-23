#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct {
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[]) {
    // Check for invalid usage
    if (argc < 2) {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX) {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++) {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++) {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks)) {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// The function takes arguments rank, name, and ranks. If name is a match for
// the name of a valid candidate, then you should update the ranks array to
// indicate that the voter has the candidate as their rank preference (where 0
// is the first preference, 1 is the second preference, etc.)
//
// Recall that ranks[i] here represents the user’s ith preference.
//
// The function should return true if the rank was successfully recorded, and
// false otherwise (if, for instance, name is not the name of one of the
// candidates).
//
// You may assume that no two candidates will have the same name.

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) {
    for (int i = 0; i < candidate_count; i++) {
        // strcmp returns 0 if the two strings are an exact match
        if (strcmp(candidates[i], name) == 0) {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// ranks[0] is preferred over ranks[1], ranks[2], ranks[3], etc.
// Then, ranks[1] is preferred over ranks[2], ranks[3], etc.
//
// Update preferences given one voter's ranks
void record_preferences(int ranks[]) {
    // loop through each candidate in the ranks array
    for (int i = 0; i < candidate_count; i++) {

        // loop through all the candidates that come AFTER candidate 'i' in
        // preference
        for (int j = i + 1; j < candidate_count; j++) {

            // candidate at ranks[i] is preferred over candidate at ranks[j]
            int preferred = ranks[i];
            int over_whom = ranks[j];

            preferences[preferred][over_whom]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) {
    // Start j at i + 1 so we only check each pair once
    for (int i = 0; i < candidate_count; i++) {
        for (int j = i + 1; j < candidate_count; j++) {

            // If there's a tie, do nothing and move to the next pair
            if (preferences[i][j] == preferences[j][i]) {
                continue;
            }
            // If i is preferred over j
            else if (preferences[i][j] > preferences[j][i]) {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            // If j is preferred over i
            else {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

int strength(pair p) {
    // look up how many voters preferred the winner over the loser
    return preferences[p.winner][p.loser];
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void) {
    int i, j;
    pair key;

    for (i = 1; i < pair_count; i++) {
        key = pairs[i];
        j = i - 1;

        while (j >= 0 && strength(pairs[j]) < strength(key)) {
            pairs[j + 1] = pairs[j];
            j = j - 1;
        }
        pairs[j + 1] = key;
    }
    return;
}

// Returns true if a path exists from 'target' back to 'start'
bool has_cycle(int target, int start) {
    // Base Case: if the target is the start, we've looped back! It's a cycle.
    if (target == start) {
        return true;
    }

    // check all possible candidates to see who 'target' is locked over
    for (int i = 0; i < candidate_count; i++) {
        if (locked[target][i]) {
            // if target points to 'i', see if 'i' can eventually lead back to
            // 'start'
            if (has_cycle(i, start)) {
                return true;
            }
        }
    }
    // if we checked all paths and none lead back to 'start', no cycle found
    return false;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
    // loop through every single pair in order of strength
    for (int i = 0; i < pair_count; i++) {
        int w = pairs[i].winner;
        int l = pairs[i].loser;

        // check if locking 'w' to 'l' creates a path from 'l' back to 'w'
        if (!has_cycle(l, w)) {
            // if it doesn't create a cycle, lock them in!
            locked[w][l] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void) {
    // loop through every candidate to check if they are the 'soruce'
    for (int i = 0; i < candidate_count; i++) {
        bool has_loser_arrows = false;

        // check if anyone has locked a victory over candidate 'i
        for (int j = 0; j < candidate_count; j++) {
            if (locked[j][i] == true) {
                // if someone beat them, they aren't the source. Move to next
                // candidate.
                has_loser_arrows = true;
                break;
            }
        }

        // if we checked everyone and nobody has an arrow pointing to candidate
        // 'i'
        if (!has_loser_arrows) {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
