// Aaron Itzkovitz
// CSCI 3240
// Project 5
// 4/8/18

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
int main( int argc, char* argv[] ){
    
    alarm(60);

    int i, rc, s_rc, num_args, n;
    int stdin_pipe_fds[2], stdout_pipe_fds[2], stderr_pipe_fds[2];
    char buf[20];
    char* filename;
    struct timeval tv;
    fd_set readfds;
    int done = 0;
    
    filename = argv[1];
    printf( "filename %s\n", filename );
    if ( filename == NULL ){
        printf("You must enter a filename as the first CL arg");
        exit(1);
    }
    
    FILE *file = fopen( filename, "r" );
    if ( file == NULL ){
    
        printf( "Could not open file" );
        exit(0);

    }
    // If we opened the file, continue
    // Create pipe ends
    pipe(stdin_pipe_fds);
    pipe(stdout_pipe_fds);
    pipe(stderr_pipe_fds);


    // Fork
    rc = fork();
    
    // If we are a child, run bc and wait for input
    if ( rc == 0 ){
        // we don't need the writing end of stdin pipe, or reading end of stdout and stderr
        close(stdin_pipe_fds[1]);
        close(stdout_pipe_fds[0]);
        close(stderr_pipe_fds[0]);
        // close stdin stdout and stderr and duplicate
        close(0);
        dup(stdin_pipe_fds[0]);
        close(stdin_pipe_fds[0]);
        close(1);
        dup(stdout_pipe_fds[1]);
        close(stdout_pipe_fds[1]);
        close(2);
        dup(stderr_pipe_fds[1]);
        close(stderr_pipe_fds[1]);
        
        // Get the num of args
        num_args = argc;
        if ( num_args == 2){
            int code = execlp("/usr/bin/bc", "/usr/bin/bc", NULL);
        } else if( num_args == 3 ) {
            execlp("/usr/bin/bc", "/usr/bin/bc", argv[2], NULL);
        } else if ( num_args == 4 ){
            execlp("/usr/bin/bc", "/usr/bin/bc", argv[2], argv[3], NULL);
        } else if ( num_args == 5 ){
            execlp("/usr/bin/bc", "/usr/bin/bc", argv[2], argv[3], argv[4], NULL);
        } else {
            exit(0);
        }
        exit(1);
    }
    // Parent. we want to send the lines as stdin to bc
    // Close extra pipe parts

    close( stdin_pipe_fds[0] );
    close( stdout_pipe_fds[1] );
    close( stderr_pipe_fds[1] );
    char line[100];
    // while there is line in the file
    while ( fgets( line, sizeof line, file ) != NULL ){
        // write into pipe
        i = write(stdin_pipe_fds[1], line, strlen(line ));
        
        // set up select
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        FD_ZERO( &readfds );
        FD_SET( stdout_pipe_fds[0], &readfds );  
        
        
        s_rc = select( FD_SETSIZE, &readfds, NULL, NULL, &tv );
        if ( s_rc == 0 ){ 
            printf( "in: %s", line );
            printf( "bc: \n\n" );

        }
        if ( s_rc < 0 ){
        
            printf( "select failed" );

        }
        if ( FD_ISSET( stdout_pipe_fds[0], &readfds ) ){
            // there's something on Stdin, read it
            n = read( stdout_pipe_fds[0], buf, 20 );
            if (n == -1){
                printf( "read failed\n" );
            } else if ( n == 0 ){
                printf( "read failed 2" );
            } else {
                printf( "in: %s", line );
                write(0, "bc: ", 5 );
                write(0, buf, n );
                write(0, "\n", 2 );
                //printf( "in: %s", line );
                //printf( "bc: %s\n", buf );
            }
        }
        
        
        /*        
        n = read( stdout_pipe_fds[0], buf, 10 );
        if (n == -1){
            printf( "read failed\n" );
        } else if ( n == 0 ){
            printf( "read failed 2" );
        } else {
            printf( "in: %s", line );
            printf( "bc: %s\n", buf );
            //printf( "\n" );
        }
        */
    }
    return 1;
}
