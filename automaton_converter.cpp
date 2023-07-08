#include <iostream>
#include <cstring>

using namespace std;

const int MAX_STATES = 100;
const int MAX_CHARACTERS = 100;
const char EPSILON = 'E';

// Structure representing a transition in the automaton
struct Transition {
    int nextState;
    char character;
    bool isFinal;
    Transition* next;
};

// Structure representing the automaton
struct Automaton {
    Transition* transitions[MAX_STATES];
    int initialState;
    int numStates;
    int numFinalStates;
    int numCharacters;
    char characters[MAX_CHARACTERS];
    int states[MAX_STATES];
    int finalStates[MAX_STATES];
};

// Function to initialize the automaton
void initializeAutomaton (Automaton* a, int initialState) {
    for (int i = 0; i < MAX_STATES; i++) {
        a->transitions[i] = NULL;
    }
    a->numStates = 0;
    a->numFinalStates = 0;
    a->numCharacters = 0;
    a->initialState = initialState;
}

// Function to add a transition to the automaton
void addTransition (Automaton* a, int fromState, char character, int toState, bool isFinal) {
    Transition* newNode = new Transition;
    newNode->character = character;
    newNode->nextState = toState;
    newNode->isFinal = isFinal;
    newNode->next = a->transitions[fromState];
    a->transitions[fromState] = newNode;

    // Update final states and states arrays
    if (isFinal) {
        if (a->numFinalStates == 0) {
            a->numFinalStates++;
            a->finalStates[a->numFinalStates-1] = toState;
        } else {
            bool flag = false;
            for (int i = 0; i < a->numFinalStates; i++) {
                if (toState == a->finalStates[i]) {
                    flag = true;
                    break;
                }
            }

            if (!flag) {
                a->numFinalStates++;
                a->finalStates[a->numFinalStates-1] = toState;
            }
        }
    }

    if (a->numStates==0) {
        a->numStates++;
        a->states[a->numStates-1] = fromState;

        if (fromState != toState) {
            a->numStates++;
            a->states[a->numStates-1] = toState;
        }

    } else {
        bool flag1 = false;
        bool flag2 = false;
        for (int i = 0; i < a->numStates; i++) {
            if (fromState == a->states[i]) {
                flag1 = true;
            }
            if (toState == a->states[i]) {
                flag2 = true;
            }
        }

        if (!flag1 && !flag2 && (fromState == toState)) {
            a->numStates++;
            a->states[a->numStates-1] = fromState;
        } else {
            if (!flag1) {
                a->numStates++;
                a->states[a->numStates-1] = fromState;
            }
            
            if (!flag2) {
                a->numStates++;
                a->states[a->numStates-1] = toState;
            }
        }
    }

    // Update characters array
    if (a->numCharacters==0) {
        a->numCharacters++;
        a->characters[a->numCharacters-1] = character;

    } else {
        bool flag = false;
        for (int i = 0; i < a->numCharacters; i++) {
            if(character == a->characters[i]){
                flag = true;
                break;
            }
        }

        if (!flag) {
            a->numCharacters++;
            a->characters[a->numCharacters-1] = character;
        }
    }
}

// Function to find transitions from a state with a specific character
void findTransitions(Automaton* nfa, int state, char character, bool* nextStates) {
    Transition* tmp = nfa->transitions[state];
    while (tmp != NULL) {
        if (tmp->character == character) {
            nextStates[tmp->nextState] = true;
        }
        tmp = tmp->next;
    }
}

// Function to check if a state or its epsilon closures are final
bool isFinal(Automaton* nfa, bool* states) {
    for (int i = 0; i < nfa->numStates; i++) {
        for (int j = 0; j < nfa->numFinalStates; j++) {
            if (states[i] && nfa->finalStates[j] == i) {
                return true;
            }
        }
    }
    return false;
}

// Function to convert NFA to DFA
Automaton* convertNFAtoDFA(Automaton* nfa) {
    int numDFAStates = 1;
    Automaton* dfa = new Automaton;
    initializeAutomaton(dfa, nfa->initialState);
    bool dfaStates[MAX_STATES][MAX_STATES] = { { false } };
    dfaStates[0][nfa->initialState] = true;

    for (int i = 0; i < numDFAStates; i++) {
        for (int j = 0; j < nfa->numCharacters; j++) {
            bool newState[nfa->numStates] = { false };

            for (int k = 0; k < nfa->numStates; k++) {
                if (dfaStates[i][k]) {
                    bool state[nfa->numStates] = { false };
                    findTransitions(nfa, k, nfa->characters[j], state);
                    
                    for (int l = 0; l < nfa->numStates; l++) {
                        if (state[l]) {
                            newState[l] = true;
                        }
                    }
                }
            }
            bool isStateFinal = isFinal(nfa, newState);

            bool nextStateFound = false;
            int nextStateIndex = -1;
            for (int k = 0; k < numDFAStates; k++) {
                bool statesMatch = true;
                for (int l = 0; l < nfa->numStates; l++) {
                    if (dfaStates[k][l] != newState[l]) {
                        statesMatch = false;
                        break;
                    }
                }
                if (statesMatch) {
                    nextStateFound = true;
                    nextStateIndex = k;
                    break;
                }
            }

            if (!nextStateFound) {
                nextStateIndex = numDFAStates;
                for (int k = 0; k < nfa->numStates; k++) {
                    dfaStates[nextStateIndex][k] = newState[k];
                }
                numDFAStates++;
            }

            addTransition(dfa, i, nfa->characters[j], nextStateIndex, isStateFinal);
        }
    }
    return dfa;
}

// Function to find the epsilon closure of a state
void findEpsilonClosure(Automaton* enfa, int state, bool* epsilonClosure) {
    epsilonClosure[state] = true;
    Transition* tmp = enfa->transitions[state];
    while (tmp != NULL) {
        if (tmp->character == EPSILON) {
            if (!epsilonClosure[tmp->nextState]) {
                findEpsilonClosure(enfa, tmp->nextState, epsilonClosure);
            }
        }
        tmp = tmp->next;
    }
}

// Function to check if a state or its epsilon closures are final in an ENFA
bool findENFAFinalState(Automaton* enfa, int state) {
    for (int i = 0; i < enfa->numFinalStates; i++) {
        if (state == enfa->finalStates[i]) {
            return true;
        }
    }  
    Transition* tmp = enfa->transitions[state];
    while (tmp != NULL) {
        if (tmp->character == EPSILON ) {
            for (int i = 0; i < enfa->numFinalStates; i++) {
                if (tmp->nextState == enfa->finalStates[i]) {
                    return true;
                }
            }   
            if (state != tmp->nextState) {
                findENFAFinalState(enfa, tmp->nextState);
            }
        }
        tmp = tmp->next;
    }
    return false;
}

// Function to find transitions from a state with a specific character in an ENFA
void findENFATransitions(Automaton* enfa, int state, char character, bool* nextStates) {
    Transition* tmp = enfa->transitions[state];
    while (tmp != NULL) {
        if (tmp->character == character) {
            nextStates[tmp->nextState] = true;
        }
        tmp = tmp->next;
    }
}

// Function to convert ENFA to NFA
Automaton* convertEpsilonNFAtoNFA(Automaton* enfa) {
    Automaton* nfa = new Automaton;
    initializeAutomaton(nfa, enfa->initialState);
    bool firstEpsilonClosure[MAX_STATES][MAX_STATES] = { { false } };
    bool finalStates[MAX_STATES] = { false };

    // Update final states array
    for (int i = 0; i < enfa->numStates; i++) {
        finalStates[i] = findENFAFinalState(enfa, i); 
    }
    for (int i = 0; i < enfa->numStates; i++) {
        findEpsilonClosure(enfa, i, firstEpsilonClosure[i]);

        for (int j = 0; j < enfa->numCharacters; j++) {
            char character = enfa->characters[j];
            if (character == EPSILON) {
                continue;
            }

            bool passingWithCharacter[MAX_STATES] = { false };
            bool secondEpsilonClosure[MAX_STATES] = { false };

            for (int k = 0; k < enfa->numStates; k++) {
                if (firstEpsilonClosure[i][k]) {
                    findENFATransitions(enfa, k, character, passingWithCharacter);
                }
            }
            for (int k = 0; k < enfa->numStates; k++) {
                if (passingWithCharacter[k]) {
                    findEpsilonClosure(enfa, k, secondEpsilonClosure);
                }
            }
            for (int k = 0; k < enfa->numStates; k++) {
                if (secondEpsilonClosure[k]) {
                    addTransition(nfa, i, character, k, finalStates[k]);
                }
            }
        }
    }

    return nfa;
}

// Function to print the automaton
void printAutomaton(Automaton* a) {
    cout << "Initial State: " << a->initialState << endl;
    cout << "States: [";
    for (int i = 0; i < a->numStates; i++) {
        cout << a->states[i];
        if (i < a->numStates - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "Final States: [";
    for (int i = 0; i < a->numFinalStates; i++) {
        cout << a->finalStates[i];
        if (i < a->numFinalStates - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "Characters: [";
    for (int i = 0; i < a->numCharacters; i++) {
        cout << a->characters[i];
        if (i < a->numCharacters - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << "Transitions:" << endl;
    for (int i = 0; i < a->numStates; i++) {
        cout << i << ": ";
        Transition* t = a->transitions[i];
        while (t != NULL) {
            cout << "(" << t->character << "," << t->nextState << ") ";
            t = t->next;
        }
        cout << endl;
    }
}

int main(){
    // Create an instance of the Automaton structure
    Automaton* enfa = new Automaton;
    initializeAutomaton(enfa, 0);

    // Add transitions to the ENFA
    addTransition(enfa, 0, EPSILON, 0, false);
    addTransition(enfa, 0, EPSILON, 1, true);
    addTransition(enfa, 0, 'c', 1, true);
    addTransition(enfa, 0, EPSILON, 2, false);
    addTransition(enfa, 0, 'b', 2, false);
    addTransition(enfa, 1, EPSILON, 1, true);
    addTransition(enfa, 2, 'a', 0, false);
    addTransition(enfa, 2, 'c', 0, false);
    addTransition(enfa, 2, 'c', 2, false);
    addTransition(enfa, 2, 'b', 1, true);
    addTransition(enfa, 2, EPSILON, 2, true);

    // Print the ENFA
    printAutomaton(enfa);
    cout << endl;

    // Convert ENFA to NFA
    Automaton* nfa = convertEpsilonNFAtoNFA(enfa);
    printAutomaton(nfa);
    cout << endl;

    // Convert NFA to DFA
    Automaton* dfa = convertNFAtoDFA(nfa);
    printAutomaton(dfa);

    return 0;
}
