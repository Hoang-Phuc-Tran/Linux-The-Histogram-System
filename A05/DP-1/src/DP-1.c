/*	FILE			: DP-1.c
 *	PROJECT			: PROG1970 - Assignment #5
 *	PROGRAMMER		: Caine Phung - ID: 6983324
                	Hoang Phuc Tran - ID: 8789102
 *	FIRST VERSION	: 2023-04-11
 *	DESCRIPTION		:  The DP-1 sets up a semaphore for writing blocking and spins up a system to 
 count a block of characters to the circular buffer every 2 seconds. As the command line argument, sharedMemoryID launches the DP-2 process.
 */

#include "../../Common/inc/Common.h"
#include "../inc/DP-1.h"

int main(int argc, char *argv[])
{
	// This object contains shared memory
	BufCircular *_circularBuff;	
	char memoryString[MAXIMUM_SIZE_ARGUMENT] = {0};
	char pathName[MAXIMUM_SIZE_ARGUMENT] = "../../DP-2/bin/DP-2";

	// The event handler for the signal
	signal(SIGINT, _signalHandler);
	// Set random number generator	
	srand(time(NULL) + 1);
	
	int memory_ID;

	// Get the Shared Memory key
   	key_t memory_key = ftok(".", 'Z');
	// Fail to get memory key
	if (memory_key == -1) 
	{ 
	  return 1;
	}

   	// Get the shared memory ID
   	memory_ID = shmget(memory_key, sizeof(BufCircular), 0);

	// Check if Shared Memory alreadIndexy exists 
	if(memory_ID == -1)
   	{
   		// Check if Shared Memory doesn't exist, we create a new one
     	memory_ID = shmget(memory_key, sizeof(BufCircular), (IPC_CREAT | 0660));
		if (memory_ID == -1) 
		{
		  return 2;
		}
   	}

	int sem_ID;
	
	// Get the Semaphore key
   	key_t sem_key = ftok("../../common/", 'A'); 
	// Fail to get semaphore key
	if (sem_ID == -1) 
	{ 
	  return 1;
	}

	// Get Semaphore ID
   	sem_ID = semget(sem_key, 1, 0);

	// Check to see if Semaphore alreadIndexy exists 
	if(sem_ID == -1)
   	{
   		// Check if Semaphore Memory doesn't exist, we create a new one
     	sem_ID = semget(sem_key, 1, (IPC_CREAT | 0660));
		if (memory_ID == -1) 
		{
		  return 2;
		}
		
   	}

	// Get the shared memory space 
	_circularBuff = (BufCircular*)shmat(memory_ID, NULL, 0);

	// Set the used semaphore
	semctl(sem_ID, 0, SETVAL, 1);

	// Give the default values
	_circularBuff->indexOfRead = 0;
	_circularBuff->indexOfWrite = 0;
	_circularBuff->semID = sem_ID;		

	// Clear all the data
	memset(_circularBuff->buff, 0, sizeof(_circularBuff->buff));

	// convert memory_ID and pass it into the array
	sprintf(memoryString, "%d", memory_ID);

	// the argument lists
	char *arguments[MAXIMUM_SIZE_ARGUMENT] = {pathName, memoryString , NULL};

	// fork and run the DP-2 process
    if (fork() == 0) 
    {
        if (execve(arguments[0], (char **)arguments, NULL) == -1) 
        {
            printf("failt to process execve child\n");
            return -1;
        }
    } 

	// Generate random letter from 'A' to 'T'
	createRandomLetter(_circularBuff);

	// Close the shared memory
	shmctl(memory_ID, IPC_RMID, NULL);
	shmdt(_circularBuff);
	return 0;
}


/* 
FUNCTION	: _signalHandler
DESCRIPTION	: This function handles the SIGINT signal by closing the program
PARAMETERS	: int _signal 	: The signal given
RETURNS		: NONE
*/
void _signalHandler(int _signal)
{
	signal(_signal, SIG_IGN);
	exit(1);
}

/* 
FUNCTION	: writeBuff
DESCRIPTION	: This function writes a specified char array to the BufCircular pointer..
PARAMETERS	: char* charArray - The array of characters 
			  BufCircular *_circular - The object pointer contains buffer
RETURNS		: NONE
*/
void writeBuff(char* arrayLetters, BufCircular *_circularBuff)
{
	// The read and write indecies
	int *readIndex = &(_circularBuff->indexOfRead);
	int *writeIndex = &(_circularBuff->indexOfWrite);

	// Check to see if the semaphore is used or not
	if (semop(_circularBuff->semID, &acquire_operation, 1) == -1)
	{
		printf("Fail to get semaphore!!!\n");
		usleep(25000);
		writeBuff(arrayLetters, _circularBuff);
	}
	else
	{
		// accessing to the semaphore is gained
		for (int counter = 0; counter< strlen(arrayLetters); counter++)
		{
			if (!(*writeIndex + 1 == *readIndex || (*writeIndex == SIZE_OF_BUFFER - 1 && *readIndex == 0)))
			{
				// Set arrayLetters as the character at the write index.
				memmove(&(_circularBuff->buff[*writeIndex]), &arrayLetters, sizeof(arrayLetters));	

				// Determine whether or not the write index necessitates wrapping back to the list's beginning.
				if(*writeIndex == SIZE_OF_BUFFER - 1)
				{
					*writeIndex = 0;
				}
				else
				{
					// If not, move the buffer down one more position.
					(*writeIndex)++;
				}			
			}
		}

		// free the semaphore
		semop(_circularBuff->semID, &release_operation, 1);
	}
}

/* 
FUNCTION	: createRandomLetter
DESCRIPTION	: This function is used to generate letters from 'A' to 'T'
PARAMETERS	: BufCircular *_circular - The object pointer contains buffer
RETURNS		: NONE
*/
void createRandomLetter(BufCircular* _circular)
{
	int running = 0;

	while (!running)
	{
		char charArray[MAXIMUM_LETTERS + 1] = {0};

		//Get random letter from 'A' to 'T'
		for (int i = 0; i < MAXIMUM_LETTERS; i++)
		{
			// put it in the array
			charArray[i] = (rand() % MAXIMUM_LETTERS) + 'A';
		}

		// write the array to the circular buffer
		writeBuff(charArray, _circular);

		//sleep 2 sec
		sleep(2);
	}	
}