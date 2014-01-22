#include "gtthread_sched.h"
#include <stdio.h>
//Period associated with Round robin
long RRPeriod = 0;					
//ID of next thread
int id = 0; 

//////////////////////////////////
//addContext()
//
//parameters: 
//      ucontext_t newContext - 
//             new threads context
//returns: 
//      int - id
//
//Adds context to linked list
//////////////////////////////////
int addContext(ucontext_t newContext){
    //Create new node
    contextNode* newNode = (contextNode*) malloc(sizeof(contextNode));
    newNode->node = newContext;
    newNode->next = NULL;
	newNode->id = id++;

    //Add node to linked list
    if (information.head==NULL){
        information.head = newNode;
        information.tail = newNode;
		current = newNode;
		newNode->parent = -1;
		initialContext();
    }else{
		newNode->parent = current->id;        
		information.tail->next = newNode;
        information.tail = newNode;
    }

	return newNode->id;
}

//////////////////////////////////
//removeContext()
//
//parameters: 
//      none
//returns: 
//      none
//
//Remove context from linked list
//////////////////////////////////
void removeContext(){
    //Pointers to nodes
    contextNode* lead = information.head;
    contextNode* trail = NULL;

    //Search while not NULL
    while(lead!=NULL){
        //If reached the correct node
        if (lead==current){

            if (trail!=NULL){
                trail->next = lead->next;
            }else{      //its the head
                information.head = lead->next;
                //If only item in linked list
                if (information.tail==current){
                    information.tail = lead->next;
                }
            }
            free(lead);
            break;
        }
        //Move values
        trail = lead;
        lead = lead->next;
    }


	//Change current thread
	if (trail->next!=NULL){
		current = trail->next;
	}else{
		current = information.head;
	}
	//Change contextdo 	
	changeContext(DONE);


}

//////////////////////////////////
//removeThread()
//
//parameters: 
//      int id - id of thread to kill
//returns: 
//      int - success
//
//Kill Thread with given id
//////////////////////////////////
int removeThread(int id){
    //Pointers to nodes
    contextNode* lead = information.head;
    contextNode* trail = NULL;

    //Search while not NULL
    while(lead!=NULL){
        //If reached the correct node
        if (lead->id==id){
            if (trail!=NULL){
                trail->next = lead->next;
            }else{      //its the head
                information.head = lead->next;
                //If only item in linked list
                if (information.tail==lead){
                    information.tail = lead->next;
                }
            }
            //Free memory
            free(lead);
			return 0;
        }
        //Move values
        trail = lead;
        lead = lead->next;
    }

	return 1;	//if none match, return 0
}

//////////////////////////////////
//threadDead()
//
//parameters: 
//      int id - id of thread to kill
//		int parent - id of parent
//returns: 
//      int - 0 if dead
//
//Find if thread with is dead
//////////////////////////////////
int threadDead(int id, int parent){
    //Pointers to nodes
    contextNode* lead = information.head;
    //Search while not NULL
    while(lead!=NULL){
        //If reached the correct node
        if (lead->id==id){
            if (lead->parent==parent){

                return 1;
            }
        }
        //Move values
        lead = lead->next;
    }
	return 0;	//if none match, return 0
}

//////////////////////////////////
//changeContext()
//
//parameters: 
//      int sig - type of signal
//returns: 
//      none
//
//Set alarm, switch context based 
//on next in linked list
//////////////////////////////////
void changeContext(int sig)
{
    struct itimerval it;       //Structure to hold time info
    struct sigaction act, oact;         //Structure for signal handler
	contextNode* prev; 

    //Swap context
	if (sig==DONE){		//a thread just finished
		current = current;
		prev = dead;	
	}else if (current->next!=NULL){		//last thread in linked list
		prev = current;
		current = current->next;
	}else{
		prev = current;
    	current = information.head;
	}
    
    //Set signal handler
    act.sa_handler = changeContext;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGVTALRM, &act, &oact); 

	// Start itimer
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = RRPeriod;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = RRPeriod;
    setitimer(ITIMER_VIRTUAL, &it, NULL);

	//Swap COntext
    swapcontext(&prev->node, &current->node);
}

//////////////////////////////////
//initialContext()
//
//parameters: 
//      none
//returns: 
//      none
//
//Set initial context
//////////////////////////////////
void initialContext(){

    struct itimerval it;       //Structure to hold time info
    struct sigaction act, oact;         //Structure for signal handler
    
    //Set signal handler
    act.sa_handler = changeContext;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPROF, &act, &oact); 

    // Start itimer
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = RRPeriod;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = RRPeriod;
    setitimer(ITIMER_PROF, &it, NULL);

    //Set current context
    current = information.head;
}

//////////////////////////////////
//getID()
//
//parameters: 
//      none
//returns: 
//      int - id of currently running
//
//Return id of self
//////////////////////////////////
int getID(){
	//Get id and return
	return current->id;
}

//////////////////////////////////
//setRR()
//
//parameters: 
//      long period - time of RR
//returns: 
//      none
//
//Set period
//////////////////////////////////
void setRR(long period){
	RRPeriod = period;	//set period
}

