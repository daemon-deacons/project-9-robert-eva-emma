#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>


int main(int argv, char *argc[]) 
{
    double *message = (double *)( malloc( (2<<29) * sizeof(double) ) );
    
    int num_elements = 1024 * 1024 * 1024; // 2^30
    int i,j,k;

    // MPI init
    int my_rank;
    int comm_sz;
    MPI_Init(&argv, &argc);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    printf("Process %d of %d IN\n",my_rank,comm_sz);

    if(my_rank==0) // process 0
    {
        // message initialization
        srand48(0);
        for(i=0;i<num_elements;i++)
        {
            message[i] = drand48();
        }
    }
    
    

    for(k=0;k<10;k++)
    {
        if(my_rank!=0) // avoid locking
        {
            MPI_Recv(message, num_elements, MPI_DOUBLE, my_rank-1,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Computation after recv for each process with idx: `my_rank`
            // update for the whole message array
            for(i=0;i<num_elements-1;i++)
                message[i] += my_rank * message[i+1];
            printf("Process %d received message from process %d\n", my_rank,my_rank-1);
        }
        // everyone send message to the next
        MPI_Send(message, num_elements, MPI_DOUBLE, (my_rank+1)%comm_sz ,0, MPI_COMM_WORLD);

        if(my_rank==0)
        {
            MPI_Recv(message, num_elements, MPI_DOUBLE, comm_sz-1,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Computation left out since my_rank == 0 here
            printf("Process %d received message from process %d\n", my_rank,comm_sz-1);
        }
    }
    
    printf("Process %d of %d OUT\n",my_rank,comm_sz);
	if(my_rank==0)
		free(message);
    MPI_Finalize();
    return 0;
}
