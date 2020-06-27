//
// Common data structures.
//

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#ifndef DPLL_COMMON_H
#define DPLL_COMMON_H

// A literal is a atomic formula (that contains a variable). Like in dimacs,
// + positive numbers denote the corresponding variables;
// - negative numbers denote the negations of the corresponding variables.
// Variables are numbered from 1.
typedef int literal;
#define POSITIVE(x) ((x) > 0)
#define NEGATIVE(x) ((x) < 0)
#define VAR(x) (((x) > 0) ? (x) : (-(x)))

// A clause is a list of literals (meaning their disjunction).
typedef std::vector<literal> clause;

// A formula is a list of clauses (meaning their conjunction).
// We also specify the total number of variables, as some of them may not occur in any clause!
struct formula {
    int num_variable;
    std::vector<clause> clauses;

    formula(int n, const std::vector<clause>& clauses): num_variable(n), clauses(clauses) {}
};

// A satisfying model (interpretation).
// e.g. `model[i] = false` means variable `i` is assigned to false.
typedef std::unordered_map<int, bool> model;

#endif //DPLL_COMMON_H
