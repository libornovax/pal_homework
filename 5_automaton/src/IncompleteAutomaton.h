#ifndef INCOMPLETEAUTOMATON_H
#define INCOMPLETEAUTOMATON_H

#include "InitialFinalStates.h"

#include <vector>
#include <string>
#include <memory>
#include <ctime>


class IncompleteAutomaton
{
    const int S_;
    const int A_;
    const int L_;
    const int F_;

    // Table of transitions between states given input characters
    // Indexed as state s + character id (x - 'a') * this->S_
    std::vector<int> transition_table_;

    // Matrix of initial and reachable final states for each initial state
    std::shared_ptr<InitialFinalStates> ifs_;

    // IMPORTANT! This allows us to process words only for the states that are still to be considered as
    // possible initial states. If an initial state has more final states than F_ then it is automatically
    // rejected and it is not processed for any other word anymore since it cannot be the state we are
    // looking for
    std::vector<int> available_states_;

    // Because from the assignment info we know that a large part in the beginnig of each word consists
    // of only 'a' characters, we can precompute the first transitions with dynamic programming and then
    // just load them on request
    std::vector<int> precomputed_states_;
    int max_precomputed_;


public:

    IncompleteAutomaton (int S, int A, int L, int F);
    ~IncompleteAutomaton ();

    void load();

    void findFinalStatesP (const char word[]);
#ifdef INCLUDE_NEGATIVES
    void findFinalStatesN (const char word[]);
#endif

    void printResults ();


    void printTransitionTable () const;


private:

    // Return the transition form state s, when reading the character c = x - 'a'
    int getTransition_ (int s, int c) const;

    // Initializes the vector of final states to the state after this automaton processes n characters 'a'
    // This speeds up the processing because the words are known to begin with a huge amount of 'a' characters
    void initializeStatesToNthA_ (int n, std::vector<int> &final_states);

    void processWord_ (const char word[], std::vector<int> &final_states);
};

#endif // INCOMPLETEAUTOMATON_H
