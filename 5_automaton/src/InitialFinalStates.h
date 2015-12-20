#ifndef INITIALFINALSTATES_H
#define INITIALFINALSTATES_H

#include <iostream>
#include <vector>


class InitialFinalStates {
    const int S_;
    // Matrix, which describes from which (initial) state we can get to which final states. Each row
    // corresponds to one initial state. It is represented as a vector
    std::vector<bool> states_;
#ifdef INCLUDE_NEGATIVES
    // In the case that a state should be final and not final at the same time we need to exclude the
    // whole row of the matrix (the initial state that corresponds to that row cannot be initial because
    // it would not accept and reject the given examples)
    std::vector<bool> rejected_;
#endif
    std::vector<int> num_finals_;


public:

    InitialFinalStates (int S);
    ~InitialFinalStates ();

    void setFinal (const std::vector<int> &initial_states, const std::vector<int> &final_states);

#ifdef INCLUDE_NEGATIVES
    void unsetFinal (const std::vector<int> &initial_states, const std::vector<int> &final_states);
#endif

    int getNumFinals (int s) const;
#ifdef INCLUDE_NEGATIVES
    bool isRejected (int s) const;
#endif


    // Print the results
    void printResults (int F) const;


private:

    int getNumFinalStates_ (int s) const;

};


#endif // INITIALFINALSTATES_H
