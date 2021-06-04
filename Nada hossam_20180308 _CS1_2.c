#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
int main(int argc, char * argv[])
{
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/
    int n;
    int arr[100];
    double localSum=0;
    double globalSum=0;
    double globalSumOfMean=0;
    double localSumOfMean=0;
    double result=0;

    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

    /* Start up MPI */
    MPI_Init( &argc, &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int a=0;

    if( my_rank == 0)
    {
     if( a==0)
    {
        printf("Enter number");
        scanf("%d", &n);
        a=1;
    }
    }

    MPI_Bcast(&n, 1, MPI_DOUBLE, 0,MPI_COMM_WORLD);

    if( my_rank != 0)
    {
	srand(my_rank);
        int i;

        for(i=0;i<n;i++)
        {
            arr[i]= rand()%20;
            localSum=localSum+arr[i];
            printf("my randum number at process %d is %d\n",my_rank,arr[i] );
        }


    }


    MPI_Allreduce(&localSum,&globalSum,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
   //printf("SUM at rank %d is %f\n",my_rank,globalSum );
      if( my_rank != 0)
    {
        double Globalmean=globalSum/(n*(p-1));
        //printf("Globalmean %f\n",Globalmean );
        int i;
        for(i=0; i<n;i++)
        {
            localSumOfMean=localSumOfMean+pow((arr[i]-Globalmean),2);
            //printf("localSumOfMean at rank %d is %f\n",my_rank,localSumOfMean);
        }


    }
    MPI_Reduce(&localSumOfMean,&globalSumOfMean,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
//    printf("globalSumOfMean at rank %d is %f\n",my_rank,globalSumOfMean );

    if (my_rank == 0 )
    {

        globalSumOfMean=globalSumOfMean/(n*(p-1));
        result=sqrt(globalSumOfMean);
      //  printf("globalSumOfMean at rank %d is %f\n",my_rank,globalSumOfMean );
        printf("Standard Deviation at rank %d is %f\n",my_rank,result );
    }
    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
