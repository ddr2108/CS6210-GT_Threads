#include <stdio.h>
#include <stdlib.h>
#include "gtthread.h"

/* Tests creation.
   Should print "Hello World!" */

int *thr1(void *in) {
int i;
int j = 0;
    for(i = 0; i<100000; i++){
	fprintf(stderr, "Hello\n");
    //fflush(stdout);
	//gtthread_cancel(1);
	}	
    return 5;
}

void *thr2(void *in) {
int i;
int j = 0;
    for(i = 0; i<100000; i++){
	fprintf(stderr, "World\n");
    //fflush(stdout);
	//gtthread_cancel(1);
	}	
	
    return NULL;
}

int main() {
    gtthread_t t1, t2;
int *a; 
    gtthread_init(50000L);
   	gtthread_create( &t2, thr2, NULL);
	fprintf(stderr, "Hello1\n");
    gtthread_create( &t1, thr1, NULL);
	fprintf(stderr, "Hello2\n");
gtthread_join(1, &a);
gtthread_join(2, &a);
		fprintf(stderr,"Duh\n");
		//gtthread_yield();

    return EXIT_SUCCESS;
}

//make clean; rm test1; make; gcc -Wall -pedantic -I{...} -o test1 test1.c gtthread.a;clear;  ./test1 

