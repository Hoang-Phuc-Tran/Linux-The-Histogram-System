/*	FILE			: Common.h
 *	PROJECT			: PROG1970 - Assignment #5
 *	PROGRAMMER		: Caine Phung - ID: 6983324
                	Hoang Phuc Tran - ID: 8789102
 *	FIRST VERSION	: 2023-04-11
 *	DESCRIPTION		: This file contains header files and constants.
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAXIMUM_LETTERS			20
#define SIZE_OF_BUFFER			256
#define MAXIMUM_SIZE_ARGUMENT	65
#define DELAY_READ 				2

// Semaphore operations 
struct sembuf acquire_operation = { 0, -1, SEM_UNDO }; 
struct sembuf release_operation = { 0, 1, SEM_UNDO }; 

// CircularBuffer struct definition
typedef struct structCirBuffer
{
	int semID;
	int indexOfRead;
	int indexOfWrite;
	char buff[SIZE_OF_BUFFER];
} BufCircular;