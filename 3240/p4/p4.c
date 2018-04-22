// Aaron Itzkovitz
// Dr. Butler
// CSCI 3240
// 3/26/18
// This project uses threads to 


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define MAXTHREADS 4

void* process_val( void* arg );
long num_elems;

// array for each thread to hold partial max and other info
long pmax[MAXTHREADS][3];

int main( int argc, char* argv[]){

    alarm(60);

    // number of threads
    long tcount, maxarg, leftovers;
    long ids[MAXTHREADS];
    long result = 0;
    long i, winning_val, winning_num;
    long test;

    // vector to hold thread ID's
    pthread_t thread_ids[ MAXTHREADS ];

    // check the users args
    tcount = atoi( argv[1] );
    maxarg = atoi( argv[2] );

    // check args
    // check args
    if ( tcount > 4 ){
        printf( "First argument must be between 1 and 4\n" );
        exit(0);
    }

    // get num of elems per thread
    num_elems = maxarg / tcount;
    leftovers = maxarg % tcount;
    
    //printf( "tcount: %d\n maxarg: %d\n num_elems: %d\n leftovers: %d", tcount, maxarg, num_elems, leftovers );

    // create tcount different threads
    for ( i = 0; i < tcount; i++ ){
        ids[i] = i;
        pthread_create( &thread_ids[i], NULL, process_val, &ids[i] );
    }

    for ( i = 0; i < tcount; i++ ){
        pthread_join( thread_ids[i], NULL );
    }

    // go through the partial maxes
    
    for ( i = 0; i < tcount; i++ ){
        if ( pmax[i][0] > result ){
            result = pmax[i][0];
            winning_val = result;
            winning_num = pmax[i][2];
        }
    }
    
    if ( leftovers ){
        for ( i = maxarg - leftovers; i < maxarg; i++ ){
            test = p4test();
            if ( test > result ){
               
                result = test;
                winning_val = result;
                winning_num = i;
            }
        }
    }
    
    
    printf( "The number, %d, gave the highest return code:%d.\n", winning_num, winning_val );
    exit( 0 );
}

void* process_val( void* arg ){

    // process each one of this threads range
    // get this threads ID
    long my_id = *((long *)arg);
    long start = num_elems * my_id;
    long end = start + num_elems; 

    // init vars for the tests
    long i;
    long test_val;
    long val = 0;
    long winner;

    for ( i = start; i < end; i++ ){

        test_val = p4test(i);
        if ( test_val > val ){
            val = test_val;
            winner = i;
        }
    }

    pmax[my_id][0] = val;
    pmax[my_id][1] = my_id;
    pmax[my_id][2] = winner;
    
    return NULL;

}



