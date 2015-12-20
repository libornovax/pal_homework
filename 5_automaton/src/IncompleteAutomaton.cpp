#include "IncompleteAutomaton.h"

#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>


IncompleteAutomaton::IncompleteAutomaton(int S, int A, int L, int F)
    : S_(S),
      A_(A),
      L_(L),
      F_(F),
      max_precomputed_(0)
{
    // Create the transition table - it is a 2D table, but represented as a vector
    this->transition_table_.resize(this->A_ * this->S_);

    this->ifs_ = std::make_shared<InitialFinalStates>(this->S_);

    // Set available states to all in the beginning
    this->available_states_.resize(this->S_);
    for (int i = 0; i < this->S_; ++i)
        this->available_states_[i] = i;

#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif
    // Initialize the array of precomputed states for the first L-sqrt(L) 'a' characters
    this->precomputed_states_.resize(this->S_ * (this->L_+1));
#ifdef MEASURE_TIME
    std::cout << "IncompleteAutomaton: Precomputed states initialization: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

    // 0th precomputed state is the initial
    for (int i = 0; i < this->S_; ++i)
        this->precomputed_states_[i] = i;
}


IncompleteAutomaton::~IncompleteAutomaton()
{

}


void IncompleteAutomaton::load()
{
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif

    // Load all state transitions
    int current_state;

    for (int i = 0; i < this->S_; ++i)
    {
        std::scanf("%d ", &current_state);

        // Load each transition from this state
        for (int j = 0; j < this->A_; ++j)
        {
            std::scanf("%d ", &this->transition_table_[current_state + j * this->S_]);
        }
    }
#ifdef MEASURE_TIME
    std::cout << "IncompleteAutomaton: Automaton load: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    // 0th precomputed state is the initial
    for (int i = 0; i < this->S_; ++i)
        this->precomputed_states_[i] = i;

    // Precompute all states with 'a' characters
    std::vector<int> temp;
    initializeStatesToNthA_(this->L_, temp);
#ifdef MEASURE_TIME
    std::cout << "IncompleteAutomaton: Precomputing 'a' states: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif
}


void IncompleteAutomaton::findFinalStatesP (const char word[])
{
    // Initialize final states
    std::vector<int> final_states;

    // Compute the final states for each initial states, while reading the given word
    this->processWord_(word, final_states);


    // Note the final states in the state table
    this->ifs_->setFinal(this->available_states_, final_states);

    // Check if we can throw away some of the states
    for (int i = 0; i < this->available_states_.size(); ++i)
    {
        if (this->ifs_->getNumFinals(this->available_states_[i]) > this->F_)
        {
            // This initial state already has too much final states - discard it
            this->available_states_.erase(this->available_states_.begin() + i);
            i--;
        }
    }
}


#ifdef INCLUDE_NEGATIVES
void IncompleteAutomaton::findFinalStatesN (const char word[])
{
    // Initialize final states
    std::vector<int> final_states;

    // Compute the final states for each initial states, while reading the given word
    this->processWord_(word, final_states);


    // Check for collisions in the initial and final state relations
    this->ifs_->unsetFinal(this->available_states_, final_states);

    for (int i = 0; i < this->available_states_.size(); ++i)
    {
        // Any state that does not have F_ final states does not have to be considered - it cannot be
        // the one that we are looking for anyway
        if (this->ifs_->getNumFinals(this->available_states_[i]) != this->F_ || this->ifs_->isRejected(this->available_states_[i]))
        {
            this->available_states_.erase(this->available_states_.begin() + i);
            i--;
        }
    }
}
#endif


void IncompleteAutomaton::printResults ()
{
    this->ifs_->printResults(this->F_);
}


void IncompleteAutomaton::printTransitionTable () const
{
    // For debugging

    for (int i = 0; i < this->S_; ++i)
    {
        std::cout << "[" << i << "] ";

        for (int j = 0; j < this->A_; ++j)
        {
            std::cout << this->getTransition_(i, j) << " ";
        }

        std::cout << std::endl;
    }
}



// ---------------------------------------- PRIVATE METHODS ---------------------------------------- //

inline int IncompleteAutomaton::getTransition_ (int s, int c) const
{
    // For current state s and the character c = x - 'a' (form 0 to 25) determine the next state
    return this->transition_table_[s + c * this->S_];
}


void IncompleteAutomaton::initializeStatesToNthA_ (int n, std::vector<int> &final_states)
{
    // Check if we have a state this far in the precomputed ones
    if (n > this->max_precomputed_)
    {
        // We haven't been this far yet - we need to precompute the values

#ifndef DISABLE_RAW_POINTERS
        // Raw pointers are a little faster
        int *tr = &this->transition_table_[0];
#endif

        // Keep evaluating the automaton as if we are reading n 'a' characters
        for (int i = this->max_precomputed_; i < n; ++i)
        {
            for (int j = 0; j < this->S_; ++j)
            {
                // Determine the next state when reading the character c
#ifdef DISABLE_RAW_POINTERS
                this->precomputed_states_[(i+1)*this->S_ + j] = this->getTransition_(this->precomputed_states_[i*this->S_ + j], 0);
#else
                this->precomputed_states_[(i+1)*this->S_ + j] = *(tr+this->precomputed_states_[i*this->S_ + j]);
#endif
            }
        }

        this->max_precomputed_ = n;
    }

    // Now load the state
    final_states.resize(this->available_states_.size());
    for (int i = 0; i < this->available_states_.size(); ++i)
    {
        final_states[i] = this->precomputed_states_[n*this->S_ + this->available_states_[i]];
    }
}


inline void IncompleteAutomaton::processWord_ (const char word[], std::vector<int> &final_states)
{
    // Compute the number of 'a' characters in the beginning of the word
    int num_a = 0;
    for (int i = 0; i < this->L_; ++i)
        if (word[i] == 'a')
            num_a++;
        else
            break;

    // Initialize the final states directly to an advanced state
    this->initializeStatesToNthA_(num_a, final_states);

    // For each letter find the next set of states of the automaton
    for (int i = num_a; i < this->L_; ++i)
    {
        // The characters are represented as integers from 0 to 25
        int c = word[i] - 'a';

#ifndef DISABLE_RAW_POINTERS
        // Raw pointers are a little faster
        int *tr = &this->transition_table_[c * this->S_];
#endif

        for (int j = 0; j < this->available_states_.size(); ++j)
        {
            // Determine the next state when reading the character c
#ifdef DISABLE_RAW_POINTERS
            final_states[j] = this->getTransition_(final_states[j], c);
#else
            final_states[j] = *(tr+final_states[j]);
#endif
        }
    }
}



