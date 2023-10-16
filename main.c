#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26

typedef struct {
    int num_states;
    int num_symbols;
    int initial_state;
    int num_final_states;
    int final_states[MAX_STATES];
    int transitions[MAX_STATES][MAX_SYMBOLS];
} FiniteAutomaton;

bool readAutomaton(const char *filename, FiniteAutomaton *fa) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }
    fscanf(file, "%d", &fa->num_symbols);
    fscanf(file, "%d", &fa->num_states);
    fscanf(file, "%d", &fa->initial_state);
    fscanf(file, "%d", &fa->num_final_states);
    for (int i = 0; i < fa->num_final_states; i++) {
        fscanf(file, "%d", &fa->final_states[i]);
    }
    for (int i = 0; i < fa->num_states; i++) {
        for (int j = 0; j < fa->num_symbols; j++) {
            fa->transitions[i][j] = -1;
        }
    }
    while (!feof(file)) {
        int from, to;
        char symbol;
        fscanf(file, "%d %c %d", &from, &symbol, &to);
        if (symbol >= 'a' && symbol <= 'z') {
            fa->transitions[from][symbol - 'a'] = to;
        }
    }
    fclose(file);
    return true;
}

bool isAccepted(FiniteAutomaton *fa, const char *w0) {
    for (int startState = 0; startState < fa->num_states; startState++) {
        int currentState = startState;
        const char *ptr = w0;
        while (*ptr) {
            int nextTransition = fa->transitions[currentState][*ptr - 'a'];
            if (nextTransition == -1) {
                break;
            }
            currentState = nextTransition;
            ptr++;
        }
        if (!*ptr) {
            for (int i = 0; i < fa->num_final_states; i++) {
                if (currentState == fa->final_states[i]) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    FiniteAutomaton fa;
    if (!readAutomaton("data.txt", &fa)) {
        printf("Error reading the automaton\n");
        return 1;
    }
    char w0[100];
    printf("Enter word w0: ");
    scanf("%s", w0);

    char w1[] = "b";
    char w[200];
    snprintf(w, sizeof(w), "%s%s", w1, w0);

    if (isAccepted(&fa, w)) {
        printf("The automaton accepts the word w = %s (w1w0).\n", w);
    } else {
        printf("The automaton does not accept the word w = %s (w1w0).\n", w);
    }

    return 0;
}