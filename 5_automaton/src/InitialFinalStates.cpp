#include "InitialFinalStates.h"


InitialFinalStates::InitialFinalStates (int S)
    :S_(S)
{
    this->states_.assign(S*S, false);
#ifdef INCLUDE_NEGATIVES
    this->rejected_.assign(S, false);
#endif
    this->num_finals_.assign(S, 0);
}


InitialFinalStates::~InitialFinalStates()
{

}


void InitialFinalStates::setFinal (const std::vector<int> &initial_states, const std::vector<int> &final_states)
{
    for (int i = 0; i < initial_states.size(); ++i)
    {
        if (!this->states_[this->S_*initial_states[i] + final_states[i]])
            // New final state
            this->num_finals_[initial_states[i]]++;

        this->states_[this->S_*initial_states[i] + final_states[i]] = true;
    }
}


#ifdef INCLUDE_NEGATIVES
void InitialFinalStates::unsetFinal (const std::vector<int> &initial_states, const std::vector<int> &final_states)
{
    // In the case that a state should be final and not final at the same time we need to exclude the
    // whole row of the matrix (the initial state that corresponds to that row cannot be initial because
    // it would not accept and reject the given examples)

    for (int i = 0; i < initial_states.size(); ++i)
    {
        if (this->states_[this->S_*initial_states[i] + final_states[i]])
        {
            // We found a final state that has to be rejected at the same time - reject the whole row
            this->rejected_[initial_states[i]] = true;
        }
    }
}
#endif


int InitialFinalStates::getNumFinals (int s) const
{
    return this->num_finals_[s];
}


#ifdef INCLUDE_NEGATIVES
bool InitialFinalStates::isRejected (int s) const
{
    return this->rejected_[s];
}
#endif


void InitialFinalStates::printResults (int F) const
{
    // Print the results of the search
    for (int i = 0; i < this->S_; ++i)
    {
#ifdef INCLUDE_NEGATIVES
        // If this row was rejected continue to next one
        if (this->rejected_[i])
            continue;
#endif

        // Check how many final states we have in this row
        if (this->getNumFinals(i) == F)
        {
            std::cout << i << " ";

            for (int j = 0; j < this->S_; ++j)
                if (this->states_[this->S_*i + j])
                    std::cout << j << " ";

            std::cout << std::endl;
        }
    }
}



// ---------------------------------------- PRIVATE METHODS ---------------------------------------- //

inline int InitialFinalStates::getNumFinalStates_ (int s) const
{
    // Compute the number of final states in the row
    int num_finals = 0;
    int row = this->S_*s;

    for (int i = row; i < row + this->S_; ++i)
    {
        if (this->states_[i])
            num_finals++;
    }

    return num_finals;
}
