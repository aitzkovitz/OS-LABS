// p6 will take a function name
// Aaron Itzkovitz
// CSCI 3240
// p6
// 4/21/18

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> 

int main( int argc, char* argv[] ){

    double result, dblArgs[4];
    char *funcName = argv[2];
    char *libName = argv[1];
    int argCount = argc - 3;
    void *handle;
    char *error;
    int i;

    for ( i = 3; i < argc; i++ ){
        dblArgs[i-3] = atof( argv[i] );
    }

    if ( argCount == 4 ){

        double( *funcPtr )( double , double , double , double ); 
        handle = dlopen( libName, RTLD_LAZY );
        if ( !handle ){

            fprintf( stderr, "%s\n", dlerror() );
            exit(1);
        } 

        funcPtr = dlsym( handle, funcName );
        if ( ( error = dlerror() ) != NULL ){
            fprintf( stderr, "%s\n", error );
            exit(1);
        }
        
        result = funcPtr( dblArgs[0], dblArgs[1], dblArgs[2], dblArgs[3] );
        fprintf( stdout, "%f\n", result );       


    } else if ( argCount == 3 ){

        double( *funcPtr )( double, double, double ); 
        handle = dlopen( libName, RTLD_LAZY );
        if ( !handle ){

            fprintf( stderr, "%s\n", dlerror() );
            exit(1);
        } 

        funcPtr = dlsym( handle, funcName );
        if ( ( error = dlerror() ) != NULL ){
            fprintf( stderr, "%s\n", error );
            exit(1);
        }

        result = funcPtr( dblArgs[0], dblArgs[1], dblArgs[2] );
        fprintf( stdout, "%f\n", result );       
    } else if ( argCount == 2 ){

        double( *funcPtr )( double, double ); 
        handle = dlopen( libName, RTLD_LAZY );
        if ( !handle ){

            fprintf( stderr, "%s\n", dlerror() );
            exit(1);
        } 

        funcPtr = dlsym( handle, funcName );
        if ( ( error = dlerror() ) != NULL ){
            fprintf( stderr, "%s\n", error );
            exit(1);
        }

        result = funcPtr( dblArgs[0], dblArgs[1] );
        fprintf( stdout, "%f\n", result );       

    } else if ( argCount == 1 ){
        double( *funcPtr )( double ); 
        handle = dlopen( libName, RTLD_LAZY );
        if ( !handle ){

            fprintf( stderr, "%s\n", dlerror() );
            exit(1);
        } 

        funcPtr = dlsym( handle, funcName );
        if ( ( error = dlerror() ) != NULL ){
            fprintf( stderr, "%s\n", error );
            exit(1);
        }

        result = funcPtr( dblArgs[0] );
        fprintf( stdout, "%f\n", result );       
    } else if ( argCount == 0 ){

        double( *funcPtr )(); 
        handle = dlopen( libName, RTLD_LAZY );
        if ( !handle ){

            fprintf( stderr, "%s\n", dlerror() );
            exit(1);
        } 

        funcPtr = dlsym( handle, funcName );
        if ( ( error = dlerror() ) != NULL ){
            fprintf( stderr, "%s\n", error );
            exit(1);
        }

        result = funcPtr( );
        fprintf( stdout, "%f\n", result );       

    } else {
        // print error

        fprintf( stderr, "wrong amount of args" );       
    }

    return 1;
}
