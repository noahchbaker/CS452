/*#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
    ifstream file("bookTest.txt");
    int count = 0;
    char characters;

    while(file >> characters)
    {
        ++count;
    }

    int n = count;
    int localn = n/p;
    char * letters = new char [count];
    int letterCount[26] = {0}; 

    if(my_rank == 0)
    {
        ifstream file("bookTest.txt");
        file >> letters;
    }

    file.close();


    char * letterLetters = new char [localn];
    int localLetterCount[26] = {0};

    MPI_Scatter(&letters[0], localn, MPI_CHAR, letterLetters, localn, MPI_CHAR, 0, MPI_COMM_WORLD);


    for(int i = 0; i< localn; i++)
    {
        localLetterCount[(int)letterLetters[i] - 97]++;
    }

    for(int i = 0; i<26; i++)
    {
        MPI_Reduce(&localLetterCount[i], &letterCount[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if(my_rank == 0)
    {
        for(int i=n-(n%p); i<n; i++)
        {
            letterCount[(int)letters[i]-97]++;
        }

        int completeCount = 0;
        for(int i = 0; i<26; i++)
        {
            cout << (char)(i+97) << ": " << letterCount[i] << endl;
            completeCount += letterCount[i];
        }
        cout << "Total Letters " << completeCount << endl;
    }

    delete [] letterLetters;
    delete [] letters;
	// Shut down MPI
	MPI_Finalize();

	return 0;
}*/
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	ifstream file("bookTest.txt");
	string text, line;
	if (file.is_open()) {
		while (getline(file, line)) {
			text += line;
		}
		file.close();
	} else {
		cerr << "Failed to open file\n";
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	int n = text.length();
    int local_n = n/p; // Base size of text each process will handle
	
	char* characters = new char[text.size() + 1];
	strcpy(characters, text.c_str());
	
	int local_size = (text.size() + 1) / p;
	

	char* local_characters = new char[local_size + 1]();
	MPI_Scatter(characters, local_size, MPI_CHAR, local_characters, local_size, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	cout << local_size << endl;
	
	unordered_map<char, int> hash_map;
	
	for (int i = 0; local_characters[i]; i++) {
        char current_char = local_characters[i];
        hash_map[current_char]++;
    }
	
	local_size = hash_map.size();
    char* chars = new char[local_size + 1];
    int* freqs = new int[local_size + 1];
	chars[local_size] = '\0';
	
	int index = 0;
	for (const auto& pair : hash_map) {
		chars[index] = pair.first;
		freqs[index] = pair.second;
		index++;
    }
	
	
	int total_size = p * local_size;
    char* all_chars = nullptr;
    int* all_freqs = nullptr;

    if (my_rank == 0) {
        all_chars = new char[total_size];
        all_freqs = new int[total_size];
    }
	
	MPI_Gather(chars, local_size, MPI_CHAR, all_chars, local_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Gather(freqs, local_size, MPI_INT, all_freqs, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (my_rank == 0) {
		for (int i=0; i < total_size; i++)
			cout << "Character: " << all_chars[i] << ". Frequency:  " << all_freqs[i] << endl;
	}

	// Clean up
    delete[] local_characters;
    delete[] chars;
    delete[] freqs;
    if (my_rank == 0) {
        delete[] all_chars;
        delete[] all_freqs;
    }
	
	// Shut down MPI
	MPI_Finalize();

	return 0;
}