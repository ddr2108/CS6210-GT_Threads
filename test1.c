#include <stdio.h>
#include <stdlib.h>
#include "gtthread.h"
    gtthread_t t1, t2;
gtthread_mutex_t g_mutex;
/* Tests creation.
   Should print "Hello World!" */
void thr2(void *in) {
int i;
int j = 0;
char * a;
int k = 1000000;
//int *k = (int*) in;
	for (i = 0 ; i <10; i++){
		//gtthread_mutex_lock(&g_mutex);
		printf("Exiting %d\n", gtthread_self()); 
		//gtthread_mutex_unlock(&g_mutex);
		while(k--);
k = 2000000;
	//gtthread_yield();
	}
    return;
}

char* thr1(void *in) {
int i; 
int j = 0;

int *a = (int*)in; 
int k = 2000000;


//for(i = 0; i<3; i++){
	//printf("Exiting asd: %d\n", gtthread_equal(t2, gtthread_self()));
	//while(k--);
	//printf("Exiting Thread 1\n");
	//fflush(stdout);
    //fflush(stdout);
	//gtthread_cancel(1);
	//}	
//gtthread_exit("Asd");
//for (i = 0; i< 20; i++){
	//gtthread_mutex_lock(&g_mutex);
	//gtthread_mutex_unlock(&g_mutex);
	//while(1){
	//if (gtthread_self()==1)
   	//gtthread_create( &t2, thr2, NULL);
	for (i = 0 ; i <20; i++){
		//gtthread_mutex_lock(&g_mutex);
		printf("Exiting %d\n", gtthread_self()); 
		//gtthread_mutex_unlock(&g_mutex);
		while(k--);
k = 2000000;
	//gtthread_yield();
	}
	//	printf("Exiting %d\n", gtthread_self()); 
//gtthread_join(t2, NULL);
//gtthread_yield();
//k = 1000000;
//}
	/*for (i = 0; i< 5; i++){

		printf("Exiting %d\n", 1); 
	while(k--);
k = 1000000;
}*/

const char quote[50] = "asdaa asd as dasd sdsdasdasd\0";
//gtthread_exit(quote);

    return (char*)quote;
}



int main() {
int i;
char *a; 
void *b;
int c = 1000000;
    gtthread_init(100000L);
	gtthread_mutex_init(&g_mutex);
	//gtthread_mutex_lock(&g_mutex);
   	gtthread_create( &t1, thr1, &c);


				//printf("id:%d\n", t1);
	//while(c--);
   	gtthread_create( &t2, thr1, &c);  
			//	printf("id:%d\n", t2); 
	//printf("Exiting Main\n"); 


	//	while(1);
	/*for (i = 0; i< 5; i++){

	printf("Exiting Main\n");
	while(c--);
c = 1000000;}*/
	//gtthread_mutex_unlock(&g_mutex);
	//gtthread_yield();
	//gtthread_exit(NULL);	
	//gtthread_mutex_lock(&g_mutex);
   	//gtthread_create( &t1, thr1, &c);
	//gtthread_mutex_unlock(&g_mutex);

//	gtthread_join(t1, &b);
	//printf("b:%s\n", (char*) b);
	//gtthread_join(t2, &b);

				//a = (int*) killed[0].ret;



		//gtthread_yield();

while(1);
//gtthread_cancel(t1);
//gtthread_exit(NULL);

    return EXIT_SUCCESS;
}

//make clean; rm test1; make; gcc -Wall -pedantic -I{...} -o test1 test1.c gtthread.a;clear;  ./test1 

