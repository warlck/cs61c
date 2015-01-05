#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


/* To save you time, we are including all 6 variants of the loop ordering
   as separate functions and then calling them using function pointers.
   The reason for having separate functions that are nearly identical is
   to avoid counting any extraneous processing towards the computation
   time.  This includes I/O accesses (printf) and conditionals (if/switch).
   I/O accesses are slow and conditional/branching statements could
   unfairly bias results (lower cases in switches must run through more
   case statements on each iteration).
*/
void multMat1( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* This is ijk loop order. */
    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            for( k = 0; k < n; k++ )
                C[i+j*n] += A[i+k*n]*B[k+j*n];  //here k in c is irrelevant, a increases by k stride. b increases by 1 stride
}

void multMat2( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* This is ikj loop order. */
    for( i = 0; i < n; i++ )
        for( k = 0; k < n; k++ )
            for( j = 0; j < n; j++ )
                C[i+j*n] += A[i+k*n]*B[k+j*n]; //best c increases by stride j, j in a is irrelevant, b increases by j
}

void multMat3( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* This is jik loop order. */
    for( j = 0; j < n; j++ )
        for( i = 0; i < n; i++ )
            for( k = 0; k < n; k++ )
                C[i+j*n] += A[i+k*n]*B[k+j*n];  //
}

void multMat4( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* This is jki loop order. */
    for( j = 0; j < n; j++ )
        for( k = 0; k < n; k++ )
            for( i = 0; i < n; i++ )
                C[i+j*n] += A[i+k*n]*B[k+j*n];
}

void multMat5( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* This is kij loop order. */
    for( k = 0; k < n; k++ )
        for( i = 0; i < n; i++ )
            for( j = 0; j < n; j++ )
                C[i+j*n] += A[i+k*n]*B[k+j*n]; //best c stride of j, a stride irrelevant for j, b stride of j
}

void multMat6( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* This is kji loop order. */
    for( k = 0; k < n; k++ )
        for( j = 0; j < n; j++ )
            for( i = 0; i < n; i++ )
                C[i+j*n] += A[i+k*n]*B[k+j*n];  //here c stride is 1, a stride is 1, b stride is irrelevant
}


/* defaults to Part 1.  pass it any argument for Part 2. */
/* uses timing features from sys/time.h that you haven't seen before */
int main( int argc, char **argv ) {
    int nmax = 1000,i,n;

    void (*orderings[])(int,float *,float *,float *) =
        {&multMat1,&multMat2,&multMat3,&multMat4,&multMat5,&multMat6};
    char *names[] = {"ijk","ikj","jik","jki","kij","kji"};

    float *A = (float *)malloc( nmax*nmax * sizeof(float));
    float *B = (float *)malloc( nmax*nmax * sizeof(float));
    float *C = (float *)malloc( nmax*nmax * sizeof(float));

    struct timeval start, end;

    if( argv[1] ) {
        printf("Running Part B...\n\n");

        /* fill matrices with random numbers */
        for( i = 0; i < nmax*nmax; i++ ) A[i] = drand48()*2-1;
        for( i = 0; i < nmax*nmax; i++ ) B[i] = drand48()*2-1;
        for( i = 0; i < nmax*nmax; i++ ) C[i] = drand48()*2-1;

        for( i = 0; i < 6; i++) {
            /* multiply matrices and measure the time */
            gettimeofday( &start, NULL );
            (*orderings[i])( nmax, A, B, C );
            gettimeofday( &end, NULL );

            /* convert time to Gflop/s */
            double seconds = (end.tv_sec - start.tv_sec) +
                1.0e-6 * (end.tv_usec - start.tv_usec);
            double Gflops = 2e-9*nmax*nmax*nmax/seconds;
            printf( "%s:\tn = %d, %.3f Gflop/s\n", names[i], nmax, Gflops );
        }
    } else {
        printf("Running Part A...\n\n");

        for( n = 10; n <= nmax; n = n<nmax && n+1+n/3>nmax ? nmax : n+1+n/3 ) {
            /* fill matrices with random numbers */
            for( i = 0; i < n*n; i++ ) A[i] = drand48()*2-1;
            for( i = 0; i < n*n; i++ ) B[i] = drand48()*2-1;
            for( i = 0; i < n*n; i++ ) C[i] = drand48()*2-1;

            /* multiply matrices and measure the time */
            gettimeofday( &start, NULL );
            multMat1( n, A, B, C );
            gettimeofday( &end, NULL );

            /* convert time to Gflop/s */
            double seconds = (end.tv_sec - start.tv_sec) +
                1.0e-6 * (end.tv_usec - start.tv_usec);
            double Gflops = 2e-9*n*n*n/seconds;
            printf( "n = %d, %.3f Gflop/s\n", n, Gflops );
        }
    }

    free( A );
    free( B );
    free( C );

    printf("\n\n");

    return 0;
}
