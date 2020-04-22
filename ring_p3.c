/*
 *  * Copyright (c) 2004-2006 The Trustees of Indiana University and Indiana
 *   *                         University Research and Technology
 *    *                         Corporation.  All rights reserved.
 *     * Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
 *      *
 *       * Simple ring test program in C.
 *        */

#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, size, next, prev, tag = 201;
    const int num_elements = 1024*1024*1024;
    //change the message to be an array of 2^30 double values
    double *message = malloc(num_elements*sizeof(double));
    
    int count;

    int times = 10;

    int i = 0;
    /* Start up MPI */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /* Calculate the rank of the next process in the ring.  Use the
    *  modulus operator so that the last process "wraps around" to
    *  rank zero. */

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;

    /* If we are the "master" process (i.e., MPI_COMM_WORLD rank 0),
    *  put the number of times to go around the ring in the
    *  message. */

    if (0 == rank) {
        //Before MPI_COMM_WORLD rank0 sends the first message, have it initialize the message thusly
        srand48(0);
       	count = times; 
        //message[0] = 10;
        for(i = 0; i < num_elements; ++i){
            message[i] = drand48();
        }
	
        printf("Process 0 sending %g to %d, tag %d (%d processes in ring)\n",
               message[0], next, tag, size);
        MPI_Send(message, num_elements, MPI_DOUBLE, next, tag, MPI_COMM_WORLD);
        MPI_Send(&count, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
        printf("Process 0 sent to %d\n", next);
    }

    /* Pass the message around the ring.  The exit mechanism works as
    *  follows: the message (a positive integer) is passed around the
    *  ring.  Each time it passes rank 0, it is decremented.  When
    *  each processes receives a message containing a 0 value, it
    *  passes the message on to the next process and then quits.  By
    *  passing the 0 message first, every process gets the 0 message
    *  and can quit normally. */

    //printf("Process 0 sent to %d\n", next);

    while (1) {
	//printf("%d at %d\n", rank, count);        

	MPI_Recv(message, num_elements, MPI_DOUBLE, prev, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
	MPI_Recv(&count, 1, MPI_INT, prev, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        //printf("Recieved on %d from %d\n", rank, prev);
        //message[0]--;
        //printf("Process 0 decremented value: %d\n", message);
        
	if (rank == 0){
	    --count;
	}

        for(i = 0; i<num_elements-1 ;i++){
            message[i] += rank * message[i + 1];
        }
        MPI_Send(message, num_elements, MPI_DOUBLE, next, tag, MPI_COMM_WORLD);
	MPI_Send(&count, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
	//printf("Sending from %d to %d with count %d\n", rank, next, count);
        if (count == 0) {
            printf("Process %d exiting\n", rank);
	    break;
        }	
    }

    /* The last process does one extra send to process 0, which needs
    *  to be received before the program can exit */

    if (0 == rank) {
        MPI_Recv(message, num_elements, MPI_DOUBLE, prev, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        
        double value = 0;
	for (i = 0; i < num_elements -1 ;i++){
		value += message[i];
	}
	printf("The value is %f\n",value);
    }


    /* All done */

    MPI_Finalize();
    return 0;
}
