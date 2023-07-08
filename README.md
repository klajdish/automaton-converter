# ENFA to DFA Conversion

This project provides C++ code to convert an epsilon-nondeterministic finite automaton (ENFA) into a nondeterministic finite automaton (NFA), and further convert the NFA into a deterministic finite automaton (DFA).

## Table of Contents

- [Overview](#overview)
- [Installation](#installation)
- [Usage](#usage)
- [Functions](#functions)

## Overview

An automaton is a mathematical model used in computer science and formal language theory to describe the behavior of systems. The ENFA, NFA, and DFA are different types of automata that vary in their level of nondeterminism and determinism.

The purpose of this project is to provide a conversion mechanism from an ENFA to an NFA and from an NFA to a DFA. These conversions help simplify the automaton and make it easier to analyze and work with.

## Installation

To use this code, follow these steps:

1. Clone the repository to your local machine.
   ```
   git clone https://github.com/klajdish/automaton-converter.git
   ```
2. Navigate to the project directory.
   ```
   cd automaton-converter
   ```
3. Compile the C++ code using a C++ compiler of your choice.
   ```
   g++ main.cpp -o automaton-converter
   ```

## Usage

To convert an ENFA to an NFA or an NFA to a DFA, you need to provide an automaton as input. The code provides two main functions for conversion:

1. `convertEpsilonNFAtoNFA`: This function takes an epsilon-nondeterministic finite automaton as input and converts it into a nondeterministic finite automaton.
2. `convertNFAtoDFA`: This function takes a nondeterministic finite automaton as input and converts it into a deterministic finite automaton.

You can also use the helper function `printAutomaton` to print the automaton to the console.

## Functions

### `convertEpsilonNFAtoNFA`

```cpp
Automaton* convertEpsilonNFAtoNFA(Automaton* enfa);
```

This function takes an epsilon-nondeterministic finite automaton (`enfa`) as input and returns a nondeterministic finite automaton.

### `convertNFAtoDFA`

```cpp
Automaton* convertNFAtoDFA(Automaton* nfa);
```

This function takes a nondeterministic finite automaton (`nfa`) as input and returns a deterministic finite automaton.

### `printAutomaton`

```cpp
void printAutomaton(Automaton* a);
```

This function prints the automaton (`a`) to the console.
