//
// Libor Novak
// 24.11.2015
//
// PAL Homework 5 - Incomplete Automaton optimized to 1 function
//

#include <iostream>
#include <cstdio>
#include <ctime>
#include <bitset>
#include <vector>


typedef unsigned char uchar;
typedef unsigned int uint;


int main (int argc, char* argv[])
{
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif

    // -- LOAD PARAMETERS -- //
    uint S, A, F, P, N, L;
    std::scanf("%d %d %d %d %d %d\n", &S, &A, &F, &P, &N, &L);
    const static uint S_ = S; // Number of states
    const static uint A_ = A; // Size of the alphabet
    const static uint F_ = F; // Number of final states
    const static uint P_ = P; // Number of positive samples
//    const static uint N_ = N; // Number of negative samples
    const static uint L_ = L; // Length of a sample


    // -- CREATE THE AUTOMATON -- //
    // NOTE: Apparently it is faster if the row of the matrix has 2^n elements because multiplication by powers
    // of 2 can be done by bit shift and that is what is then needed when accessing the matrix elements
    static uchar transition_table_[26][256];
    uchar precomputed_states_[L_+1][256];

    // LOAD
#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    // Load the transition table
    for (uint i = 0; i < S_; ++i)
    {
        // We need to scan integers!!
        uint current_state;
        std::scanf("%d ", &current_state);

        // Load each transition from this state
        for (uint j = 0; j < A_; ++j)
        {
            uint temp;
            std::scanf("%d ", &temp);
            transition_table_[j][current_state] = (uchar) temp;
        }
    }
    std::scanf("\n");
#ifdef MEASURE_TIME
    std::cout << "Automaton load: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif


    // PRECOMPUTE THE 'a' STATES
#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    // When there is 0 'a's in the beginning of the string return the state indeces
    for (uint i = 0; i < S_; ++i)
        precomputed_states_[0][i] = i;

    // Now precompute the states all the way to the end of the word as if we read only 'a's all the time
    // This takes basically no time
    for (uint i = 0; i < L; ++i)
    {
        for (uint j = 0; j < S_; ++j)
        {
            // Determine the next state when reading the character c
            precomputed_states_[i+1][j] = transition_table_[0][precomputed_states_[i][j]];
        }
    }
#ifdef MEASURE_TIME
    std::cout << "Precomputation of 'a': " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif



    // -- PROCESS THE SAMPLES -- //
    // These variables will hold information about initial - final state relations
    std::bitset<256*256> init_final;
    uchar num_finals[256] = {0};

#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    char word[L_+1];

// Toggle one or the other method
//*

    // Process ONLY POSITIVE samples - apparently the negatives are not even needed
    for (uint w = 0; w < P_; ++w)
    {
        // Scan the whole line (word) because we want to iterate over the word for each initial state!!
//        std::scanf("%s ", word);
        std::fread(word, sizeof(char), L_, stdin); // Fuck this is fast!! For some reason it gives segfaults in the upload system (not on my mac though!)
        std::scanf(" ");

        // Find all first 'a's
        uint a = 0;
        while (word[a] == 'a' && a < L_)
            a++;

        // Subtract 'a' from the remaining characters to get the character indeces
        for (uint i = a; i < L_; ++i)
            word[i] -= 'a';


        // For each initial state iterate over the word
        // NOTE: THIS IS DIFFERENT than in my other implementation, because tha outer loop iterates over states
        // and the inner loop over the characters of the word
        for (uint s = 0; s < S_; ++s)
        {
            // NOTE: The CRUCIAL idea here is that we do not even have to process the word for the current
            // initial state s if we already found more than F_ final states for this initial one. If we
            // found more final states for this s it means that it is NOT the configuration of the automaton
            // that we are looking for, therefore we do not even have to bother with it!
            // This makes it much faster than running the loops in the opposite manner because of this check

            // We can skip this state if we already found more than F_ final states for this initial state
            // s - it is going to be excluded from the results
            if (num_finals[s] > F_)
                continue;

            // Load the precomputed state
            uchar final = precomputed_states_[a][s];

            // Bubble all the way to the end of the word
            for (uint i = a; i < L_; ++i)
            {
                // NOTE: Cyclic states - if we find a cyclic state we can jump over all same characters
                // without even looking into the transition table again!
                // I implement this only for character 'a'
                if (word[i] == 0 && final == transition_table_[(uint)word[i]][(uint)final])
                {
                    // Cycle through all same characters in the word
                    // Once we find a different character we get back to the initial loop
                    for (;word[i] == 0 && i < L_; ++i);
                }

                final = transition_table_[(uint)word[i]][(uint)final];
            }

            // Raise counter of final states if it is not a final state already
            if (!init_final[256*s + final])
                // New final state for this initial state
                num_finals[s]++;

            // Set this final as a final state for this s
            init_final[256*s + final] = 1;
        }
    }

/*/

    uint c;
    std::vector<uchar> available(S_);
    for (int i = 0; i < S_; ++i)
        available[i] = i;

    for (int w = 0; w < P_; ++w)
    {
        uchar finals[available.size()];
        // Scan one character after another

        // Scan all 'a's
        uint a = 0;
        while ((c = getchar_unlocked()) == 'a')
            a++;

        // Set the state
        for (int s = 0; s < available.size(); ++s)
            finals[s] = precomputed_states_[a][available[s]];

        // Convert the current character to a number
        c = c - 'a';
        for (int s = 0; s < available.size(); ++s)
        {
            finals[s] = transition_table_[c][finals[s]];
        }

        for (int i = a+1; i < L_; i++)
        {
            // Process the current character
            c = getchar_unlocked() - 'a';

            // Find all next states
            for (int s = 0; s < available.size(); ++s)
            {
                finals[s] = transition_table_[c][finals[s]];
            }
        }

        // We must scan the endline in order for this to work!!
        std::scanf("\n");


        // Write down the final states
        for (int i = 0, fi = 0; i < available.size(); ++i, ++fi)
        {
            if (!init_final[256*available[i] + finals[fi]])
            {
                // New final state for this initial state
                num_finals[available[i]]++;

                // If there are already more than F_ final states for this initial one, remove the initial
                // one form the consider ones
                if (num_finals[available[i]] > F_)
                {
                    available.erase(available.begin() + i);
                    i--;
                }
            }

            init_final[256*available[i] + finals[fi]] = 1;
        }
    }

//*/

#ifdef MEASURE_TIME
    std::cout << "Load and process words: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

    // -- WRITE RESULT -- //
    for (uint i = 0; i < S_; ++i)
    {
        // Check how many final states we have in this row
        if (num_finals[i] == F_)
        {
            std::cout << i << " ";

            for (uint j = 0; j < S_; ++j)
                if (init_final[256*i + j])
                    std::cout << j << " ";

            std::cout << std::endl;
        }
    }


#ifdef MEASURE_TIME
    std::cout << "TOTAL: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

	return 0;
}


