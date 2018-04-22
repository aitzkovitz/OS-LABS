#include <stdio.h>
#include <time.h>

double timestamp(){

    time_t seconds;
    seconds =(double) time(0);
    return seconds;
}
