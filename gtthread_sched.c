#include "gtthread_sched.h"
#include <stdio.h>

void blockSig(){
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGVTALRM);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

void unblockSig(){
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGVTALRM);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

unsigned int addContext(ucontext_t newContext){
    //Create new node
    contextNode* newNode = getNode();
    
    if (newNode==NULL){
        perror("Out of threads");
    }
    
	//Set parts
    newNode->node = newContext;				//Set context
	newNode->id = information.id++;			//Set id
    //Add node to linked list
    if (information.head==NULL){			//if it is the main thread
        information.head = newNode;			//Set the pointers
        information.tail = newNode;
		newNode->next = information.head;
		newNode->prev = information.tail;
		current = newNode;					//Set this thread to be currently running
		newNode->parent = -1;				//Mark as no parent
		initialContext();					//Start context
    }else{									//if it is a child thread
		information.tail->next = newNode;	//Fix pointers
		information.head->prev = newNode;
		newNode->next = information.head;
		newNode->prev = information.tail;
        information.tail = newNode;
		newNode->parent = current->id;      //Get parent id based on currently running thread  
    }

	return newNode->id;					//return the id
}

void removeContext(contextNode* toDelete){

	//Fix pointers
	toDelete->prev->next = toDelete->next;
	toDelete->next->prev = toDelete->prev;

	//if it is the tail
	if (toDelete == information.tail){
		information.tail = toDelete->prev;
	}

	//Fix head if needed
	if (toDelete==information.head){
		//if last node exit
		if (toDelete->next!=NULL && toDelete->next!=toDelete){
			information.head = toDelete->next;
		}else{
			exit(3);
		}

		//if not joinable, mark as invalid
		if (toDelete->parent == -1){
			int index;
			//Index of item
			index = ((struct _allocContext*)toDelete - nodeArray);
		
			nodeArray[index].valid = 0;     //Set not valid, not yet joined

			//Change context if removing current context
			if (toDelete==current){
				current = current->next;
				changeContext(DONE);
				return;
			}
		}
	}

	//Change context if removing current context
	if (toDelete==current){
		current = current->next;
		removeNode(toDelete);
		changeContext(DONE);
		return;
	}

	removeNode(toDelete);	//Free allocated memory
}

int removeID(unsigned int id){
    //Pointers to thread
    contextNode* thread = findThread(id);

	//if thread found, remove
	if (thread!=NULL){
		removeContext(thread);
		return 0;
    }
	return 1;	//if none match, return 1
}

void initialContext(){
	//Set timer
	setTimer();

    //Set current context
    current = information.head;
}

void changeContext(int sig)
{
	contextNode* prev; 		//The node to switch into

    //Swap context
	if (sig==DONE){		//a thread just finished
		current = current;
		prev = &dead;	
	}else{
		prev = current;
    	current = current->next;
	}
    
	//Set timer
	setTimer();

	//Swap COntext
    swapcontext(&prev->node, &current->node);
}

int threadDead(unsigned int id, int parent){
    //Pointers to thread
    contextNode* thread = findThread(id);

	//if thread found, return 1
	if (thread!=NULL){
		return 1;
    }

	return 0;	//if none match, return 0
}

contextNode* findThread(unsigned int id){
    //Pointers to nodes
    contextNode* thread = information.head;
    
    do{ //Search through all threads

		//Check for a match
        if (thread->id==id){
			return thread;
        }
		//Go to next thread
        thread = thread->next;

    }while(thread!=information.head && thread!=NULL);

	return NULL;	//if can't find any
}

void setTimer(){
    struct itimerval it;       		//Structure to hold time info
    struct sigaction act, oact;     //Structure for signal handler
    
	blockSig();
    //Set signal handler
    act.sa_handler = changeContext;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGVTALRM, &act, &oact); 

    // Start itimer
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = information.RRPeriod;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = information.RRPeriod;
    setitimer(ITIMER_VIRTUAL, &it, NULL);
	unblockSig();

}

int getID(){
	//Get id and return
	return current->id;
}

void setRet(void* retval){
    int index;
    //Index of item
    index = ((struct _allocContext*)current - nodeArray);
    
    nodeArray[index].ret = retval;
}

contextNode* getNode(){
    int i;
    //look through array
    for (i=0;i<MAX_THREADS;i++){
        if (nodeArray[i].valid == 0){
            nodeArray[i].valid = 1;     //Set valid
			return &(nodeArray[i].newNode);     //return the node
        }
    }
    
    return NULL;
    
    /*//old version/
    contextNode* newNode = (contextNode*) malloc(sizeof(contextNode))
    return newNode;
    */

}

void removeNode(contextNode* toDelete){
    int index;
    //Index of item
    index = ((struct _allocContext*)toDelete - nodeArray);
    
    nodeArray[index].valid = 2;     //Set not valid, not yet joined
    //	free(toDelete)              //old verion
}

void cleanMemory(){
    contextNode* thread = information.head->next;
    
	//Remove data associated with each thread	
    while(thread!=information.head && thread!=NULL){
		removeID(thread->id);
	}
}
