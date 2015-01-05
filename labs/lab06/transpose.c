#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void transpose( int n, int blocksize, int *dst, int *src ) {
    /* TO DO: implement blocking (two more loops) */
    for (int x = 0; x < n; x += blocksize) {
        for (int y = 0; y < n; y += blocksize) {
            for (int i = x; i < blocksize + x && i < n; i++) { //handle 5 by 5 i < n ex. i = 3 < n, so good
                for (int j = y; j < blocksize + y && j < n; j++) { // j < n
                    dst[j + i*n] = src[i + j*n];
                }
            }
        }
    }
}
//naive implementation
/* void transpose( int n, int blocksize, int *dst, int *src ) {
    int i,j; */
    /* TO DO: implement blocking (two more loops) */
   /* for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            dst[j + i*n] = src[i + j*n];
} */

int main( int argc, char **argv ) {
    int n = 2000,i,j;
    int blocksize = 33; /* to do: find a better block size */

    /* allocate an n*n block of integers for the matrices */
    int *A = (int*)malloc( n*n*sizeof(int) );
    int *B = (int*)malloc( n*n*sizeof(int) );

    /* initialize A,B to random integers */
    srand48( time( NULL ) );
    for( i = 0; i < n*n; i++ ) A[i] = lrand48( );
    for( i = 0; i < n*n; i++ ) B[i] = lrand48( );

    /* measure performance */
    struct timeval start, end;

    gettimeofday( &start, NULL );
    transpose( n, blocksize, B, A );
    gettimeofday( &end, NULL );

    double seconds = (end.tv_sec - start.tv_sec) +
        1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "%g milliseconds\n", seconds*1e3 );

    /* check correctness */
    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            if( B[j+i*n] != A[i+j*n] ) {
                printf("Error!!!! Transpose does not result in correct answer!!\n");
                exit( -1 );
            }

    /* release resources */
    free( A );
    free( B );
    return 0;
}

