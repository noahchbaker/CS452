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
	char messageOdd[100];	// message itself
    char messageEven[100];
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE

    char * oddRing = messageOdd;
    sprintf(oddRing, "Odd ring: ");
    sprintf(oddRing + strlen(oddRing) , "%d ", my_rank);

    char * evenRing = messageEven;
    sprintf(evenRing, "Even list: ");
    sprintf(evenRing + strlen(evenRing) , "%d ", my_rank);


    if(my_rank == 0)
    {
        //cout << "Run, process  " << my_rank << " RUN!" << endl;
        MPI_Send(evenRing, strlen(evenRing) + 1, MPI_CHAR, 2, tag, MPI_COMM_WORLD);
        if(p % 2 == 0)
        {
            MPI_Recv(evenRing, 100, MPI_CHAR, p - 2, tag, MPI_COMM_WORLD, &status);
        }
        else
        {
            MPI_Recv(evenRing, 100, MPI_CHAR, p - (p%2), tag, MPI_COMM_WORLD, &status);
        }
        sprintf(evenRing + strlen(evenRing) , "--> %d ", my_rank);
        cout << evenRing << endl;
    }   
    else if (my_rank == 1)
    {
        //cout << "Run, process  " << my_rank << " RUN!" << endl;
        MPI_Send(oddRing, strlen(oddRing) + 1, MPI_CHAR, p - (p%2) - 1 , tag, MPI_COMM_WORLD);
        MPI_Recv(oddRing, 100, MPI_CHAR, my_rank + 2 , tag, MPI_COMM_WORLD, &status);
        sprintf(oddRing + strlen(oddRing) , "--> %d ", my_rank);
        cout << oddRing << endl;
    }
    else if(p%2 == 0) //even number of processors
    {
        if(my_rank % 2 == 0) // even case
        {
            MPI_Recv(evenRing, 100, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD, &status);
            sprintf(evenRing + strlen(evenRing) , "--> %d ", my_rank);
            //cout << "Run, process  " << my_rank << " RUN!" << endl;
            MPI_Send(evenRing, strlen(evenRing) + 1, MPI_CHAR, (my_rank + 2) % p, tag, MPI_COMM_WORLD);
        }
        else // odd case
        {
            MPI_Recv(oddRing, 100, MPI_CHAR, (my_rank + 2) % p, tag, MPI_COMM_WORLD, &status);
            sprintf(oddRing + strlen(oddRing) , "--> %d ", my_rank);
            //cout << "Run, process  " << my_rank << " RUN!" << endl;
            MPI_Send(oddRing, strlen(oddRing) + 1, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD);
        }
    }
    else //odd processors
    {
        if(my_rank % 2 == 0) // even case
        {
            MPI_Recv(evenRing, 100, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD, &status);
            sprintf(evenRing + strlen(evenRing) , "--> %d ", my_rank);
            MPI_Send(evenRing, strlen(evenRing) + 1, MPI_CHAR, (my_rank + 2) % (p+1), tag, MPI_COMM_WORLD);
        }
        else // odd case
        {
            if(p-2 == my_rank)
            {
                MPI_Recv(oddRing, 100, MPI_CHAR, (p+1) % (my_rank + 2), tag, MPI_COMM_WORLD, &status);
            }
            else
            {
                MPI_Recv(oddRing, 100, MPI_CHAR, (my_rank + 2) % (p+1), tag, MPI_COMM_WORLD, &status);
            }
            sprintf(oddRing + strlen(oddRing) , "--> %d ", my_rank);
            MPI_Send(oddRing, strlen(oddRing) + 1, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD);
        }
    }

	// Shut down MPI
	MPI_Finalize();

	return 0;
}*/

#include <iostream>
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
    char * ring1 = message;
    char * ring2 = message;
    sprintf(ring1, "Hi from Ring 1, Sent list: ");
    sprintf(ring1 + strlen(ring1), "%d", my_rank);
    
    sprintf(ring2, "Hi from Ring 2, Sent list: ");
    sprintf(ring2 + strlen(ring2), "%d", my_rank);
    
    //First Ring
    if (my_rank==0)
    {
        int last_even = 0;
        int next_even = (my_rank + 2)%p;

        if(p%2 == 0)
        {
            last_even = p-2;
        }
        else
        {
            last_even = p-1;
        }
        MPI_Send(ring1, strlen(ring1)+1, MPI_CHAR, next_even, tag, MPI_COMM_WORLD);
        MPI_Recv(ring1, 100, MPI_CHAR, last_even, tag, MPI_COMM_WORLD, &status);
        cout << "Ring 1: " << ring1 << endl;
    }
    else if (my_rank%2 == 0)
    {
        int next_even = (my_rank+2)%p;
        if(next_even == 1)
        {
            next_even = 0;
        }
        MPI_Recv(ring1, 100, MPI_CHAR, (my_rank-2)%p, tag, MPI_COMM_WORLD, &status);
        sprintf(ring1 + strlen(ring1), "%d", my_rank);
        MPI_Send(ring1, strlen(ring1)+1, MPI_CHAR, next_even, tag, MPI_COMM_WORLD);
    }

    //Second Ring
    if(my_rank == 1)
    {
        int last_odd = 0;
        if(p%2==0)
        {
            last_odd = p-1;
        }
        else
        {
            last_odd = p-2;
        }
        int prev_odd = (my_rank+2);
        MPI_Send(ring2, strlen(ring2)+1, MPI_CHAR, last_odd, tag, MPI_COMM_WORLD);
        MPI_Recv(ring2, 100, MPI_CHAR, prev_odd, tag, MPI_COMM_WORLD, &status);
        cout << "Ring 2: " << ring2 << endl;
    }
    else if(my_rank%2 != 0)
    {
        int prev_odd = 0;
        if(my_rank == p || my_rank==(p-1))
        {
            prev_odd = 1;
        }
        else
        {
            if((my_rank+2)>=p)
            {
                prev_odd = 1;
            }
            else
            {
                prev_odd = (my_rank+2);
            }

        }
        MPI_Recv(ring2, 100, MPI_CHAR, prev_odd, tag, MPI_COMM_WORLD, &status); 
        sprintf(ring2 + strlen(ring2), "%d ", my_rank); 
        MPI_Send(ring2, strlen(ring2) +1, MPI_CHAR,(my_rank-2), tag,MPI_COMM_WORLD); 
    }

	// Shut down MPI
	MPI_Finalize();

	return 0;
}