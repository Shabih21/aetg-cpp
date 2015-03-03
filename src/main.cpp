#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/* Name:  Tyler Page
 * Program: AETG Algorithim
 * Purpose: To implement a rudimentary
 * 			algorithm to understand
 * 			the concept of product testing.
*/

/* Implementation: 3D Array [Factors][Options][0] = number
 *                                            [1] = weight
 *
 * In order to attempt to make my algorithim more efficient
 * I added a weight-system.  Each number will only be used
 * (option * (factor - 1)) times each.  Everytime a number
 * is part of a generated tuple, I add 1 to it's weight,
 * which allows me to select lesser used values in my tuple
 * selection
*/

/* Compiling: g++ main.cpp -std=c++0x
*/


int main()
{
    char str[100]; //input string
    char *tmp; //token pointer

    int factors, options; //base factors and options variables from input
    int counter = 0; //counter for assigning numbers to array

    srand(time(NULL)); //seed for randomization


    //get input
    cout << "Please enter a valid factor^option format (e.g 1^5): ";
    cin >> str;

    //parse string so we can use entered factor/options
    tmp = strtok(str, "^");
    factors = atoi(tmp);
    options = atoi(strtok(NULL, "^"));

    //initialize array AFTER we get input from user
    int digit_array[factors][options];
    int pair_count = 0;

    //counters for number assignment
    int factors_count;
    int options_count;

    //generate and assign numbers to array
    for(factors_count = 0; factors_count < factors; factors_count++)
    {
        for(options_count = 0; options_count < options; options_count++)
        {
            //store the sequential number in the array at (column, row) position
            digit_array[factors_count][options_count] = counter;
            counter++;
        }
        pair_count += (options * (factors - (factors_count + 1))) * options;
    }

    //cout << counter << " cells generated with a possible " << pair_count << " number of pairs.\n";

    int pair_array[pair_count][3]; //pair_array[number of pairs possible][3]
    int entry_counter = 0; //counter for the amount of pair entries
    int pair_factor = 0; //factor index for pair we are matching
    int pair_option = 0; //option index for pair we are matching

    //for each factor (A factor)
    for(factors_count = 0; factors_count < factors; factors_count++)
       {
           //for each option we have in that factor (A option)
           for(options_count = 0; options_count < options; options_count++)
           {
               //for each factor in B (+1 so we don't match with stuff from own factor)
               for(pair_factor = factors_count + 1; pair_factor < factors; pair_factor++)
               {
                   //for each option in B
                   for(pair_option = 0; pair_option < options; pair_option++)
                   {
                       //set [0] to A, [1] to B, and [2] to 0
                       pair_array[entry_counter][0] = digit_array[factors_count][options_count];
                       pair_array[entry_counter][1] = digit_array[pair_factor][pair_option];
                       pair_array[entry_counter][2] = 0;

                       //cout << "Pair generated: " << pair_array[entry_counter][0] << " " << pair_array[entry_counter][1] << ".\n";
                       entry_counter++;
                   }
               }
           }
       }

    //generate random selection from each factor
    //for test case

    int rand_option; //per test case
    int test_case_array[50][factors + 1]; //per test case


    vector<string> best_test_suite; //per program run
    int best_test_case_counter = 0; //per program run
    int test_suite_counter = 0; // per program run

    while(test_suite_counter < 100)
    {
        vector<string> test_suite; //per test suite
        int test_case_counter = 0; //per test suite
        int covered_pair_counter = 0; //per test suite
        while(covered_pair_counter < pair_count)
        {

            //modify so it picks a random uncovered pair, and generates test case
            //from that


            for(counter = 0; counter < 50; counter++)
            {
                //cout << "Test Case #" << counter << ": ";
                for(factors_count = 0; factors_count < factors; factors_count++)
                {
                    //generate the random number from 0 to (options)
                    rand_option = rand() % options;

                    //assign it to the array
                    test_case_array[counter][factors_count] = digit_array[factors_count][rand_option];
                    //cout << test_case_array[counter][factors_count] << " ";
                }

                //after assigning all the random choices, set the last
                //index to 0
                test_case_array[counter][factors] = 0;
                //cout << endl;
            }

            //cout << "Generated 50 test cases with " << factors <<" options in each.\n";

            //test each case

            //to keep track of which case is the best;
            int best_index = -1;
            int best_pairs = 0;
            int factors_countB;
            int pair_counter;

            for(counter = 0; counter < 50; counter++)
            {
                //factors_count = A from (A->B)
                for(factors_count = 0; factors_count < factors; factors_count++)
                {
                    //factors_countB = B from (A->B)
                    for(factors_countB = factors_count + 1; factors_countB < factors; factors_countB++)
                    {
                        //check pair_array for pair
                        for(pair_counter = 0; pair_counter < pair_count; pair_counter++)
                        {
                            //the pair is covered if the test case tuple (A,B) matches the pair (A,B)
                            //if test case [counter][factors_count] == pair_array[pair counter][0]
                            //		and    [counter][factor_countB] == pair_array[pair counter][1]
                            //      and     we haven't already used the pair yet
                            if(test_case_array[counter][factors_count] == pair_array[pair_counter][0]
                                && test_case_array[counter][factors_countB] == pair_array[pair_counter][1]
                                && pair_array[pair_counter][2] == 0)
                            {

                                //increase the counter of max pairs for the test case
                                test_case_array[counter][factors]++;
                            }
                        }
                    }
                }

                //if we beat the best amount of pairs from any previous test case
                if(test_case_array[counter][factors] > best_pairs)
                {

                    //save the best index
                    best_index = counter;
                    best_pairs = test_case_array[counter][factors];
                }
            }

            //cout << "The best random test case was #" << best_index <<
                    //" with " << test_case_array[best_index][factors] << " pairs.\n";

            //mark the pairs generated from the best test case as used
            //factors_count = A from (A->B)
            for(factors_count = 0; factors_count < factors; factors_count++)
            {
                //factors_countB = B from (A->B)
                for(factors_countB = factors_count + 1; factors_countB < factors; factors_countB++)
                {
                    //check pair_array for pair
                    for(pair_counter = 0; pair_counter < pair_count; pair_counter++)
                    {
                            //if the pair from the test case matches a pair we haven't used yet
                            if(test_case_array[best_index][factors_count] == pair_array[pair_counter][0]
                            && test_case_array[best_index][factors_countB] == pair_array[pair_counter][1]
                            && pair_array[pair_counter][2] == 0)
                            {
                                //mark it as used
                                pair_array[pair_counter][2] = 1;

                                //increment the covered pair counter for every new pair covered
                                covered_pair_counter++;
                            }
                    }
                }
            }

            //save best test case
            stringstream sstr;
            int i;
            for(i = 0; i < factors; i++)
            {
                sstr << test_case_array[best_index][i] << " ";
            }

            test_suite.push_back(sstr.str());
            test_case_counter++;
            //repeat until no more pairs
            //repeat whole thing 100x (test suite)
            //record best test suite to user
        }

        //if we have fewer than the previous best test suite OR its the first test suite
        if(test_case_counter < best_test_case_counter || best_test_case_counter == 0)
        {
            //set it equal to the current one
            best_test_case_counter = test_case_counter;
            best_test_suite = test_suite;
        }
        //cout << "Generated test suite #" << test_suite_counter << " with " << test_case_counter << " test cases.\n";

        int x;
        for(x = 0; x < pair_count; x++)
        {
            pair_array[x][2] = 0;
        }
        test_suite_counter++;
    }

    //cout << "The best test suite had "<< best_test_case_counter << " case(s) in it.\n";

    //begin output file

    //open the file "output.txt" or erase if exists
    ofstream file;
    file.open("output.txt", ios::trunc);

    //suite_master is the fewest possible test cases
    //followed by two end lines
    file << best_test_suite.size() << "\n\n";

    int j = 0;

    //print out the test cases from the shortest suite generated
    for(j = 0; j < best_test_suite.size(); j++)
    {
        file << best_test_suite[j] << "\n";
    }

    //close the file
    file.close();

    //we're done
    return 0;
}
