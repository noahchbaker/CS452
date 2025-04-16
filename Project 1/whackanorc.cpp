/*#include <iostream>
#include <stdio.h>
#include <string.h>
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
    srand(71911);
    int n = 1000000;
    int * monster = new int[n];
    int arraySize = n;
    long int max = 0;
    long int min = 0;
    long int sum = 0;

    // add numbers to the monster array
    if (my_rank == 0)
		for(int x = 0; x < n; x++) {
			monster[x] = 1 + (rand() % 100);
        }

    // Divide the problem up
    int localn = n/p; // how much work each processor will have to do 
	int * localMonster = new int [localn]; // local array that holds info for local a

    // Scatter
    MPI_Scatter(&monster[0], localn, MPI_INT, localMonster, localn, MPI_INT, 0, MPI_COMM_WORLD);

    //LocalWork
    //Sum 
    long int localSum = 0;
    for(int x = 0; x<localn; x++)
    {
        localSum += localMonster[x];
    }

    //Max and Min
    long int localMax = localMonster[0];
    long int localMin = localMonster[0];
    for(int x = 0; x<localn; x++)
    {
        if(localMonster[x] > localMax)
            localMax = localMonster[x];
        else if(localMonster[x] < localMin)
            localMin = localMonster[x];
    }

    // doing the stuff
    MPI_Allreduce(&localMax, &max, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD); 
    MPI_Allreduce(&localMin, &min, 1, MPI_LONG, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&localSum, &sum, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD); // Then divide by n at the end for average.

    if(my_rank == 0)
    {
        cout << max << "    " << min << "   " << (double)sum/n << endl;
    }

    delete [] monster;
    delete [] localMonster;

	// Shut down MPI
	MPI_Finalize();

	return 0;
}*/
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

// Linux commands to help you manage processes
// ps
// kill -9

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
    srand(71911);
    int n = 1000000;
    int * a = new int[n];
    int * b = new int[n];
    
    long int maximum = 0;
    long int minimum = 0;
    long int sum = 0;
    long int avg = 0;
    
    //initialize the arrays with the seed
    if(my_rank == 0)
        for(int i = 0; i < n; i++) {
            a[i] = rand() % 8;
            b[i] = rand() % 8;
        }
    //divide the problem into pieces
    int localn = n/p; 
    int * locala = new int[localn];
    int * localb = new int[localn];

    //scatter the data
    MPI_Scatter(&a[0], localn, MPI_INT, locala, localn, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&b[0], localn, MPI_INT, localb, localn, MPI_INT, 0, MPI_COMM_WORLD);
    //Do the maximum of local data
    long int localmax = 0;
    for(int i = 0; i < localn; i++) {
        if(locala[i] > localmax) {
            localmax = locala[i];
        }
        if(localb[i] > localmax) {
            localmax = localb[i];
        }
    }
    //Do the minimum of local data
    long int localmin = 0;
    for(int i = 0; i < localn; i++) {
        if(locala[i] < localmin) {
            localmin = locala[i];
        }
        if(localb[i] < localmin) {
            localmin = localb[i];
        }
    }
    //Do the sum of local data
    long int localsum = 0;
    for(int i = 0; i < localn; i++) {
        localsum += locala[i] + localb[i];
    }
    //Do the average of local data
    long int localavg = localsum / (2 * localn);
    //Collect the results
    MPI_Reduce(&localmax, &maximum, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localmin, &minimum, 1, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localavg, &avg, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    //Print the results
    cout << "Maximum: " << maximum << endl;
    cout << "Minimum: " << minimum << endl;
    cout << "Avg: " << avg << endl;
    
    delete [] a;
    delete [] b;
    delete [] locala;
    delete [] localb;



	// Shut down MPI
	MPI_Finalize();

	return 0;
}