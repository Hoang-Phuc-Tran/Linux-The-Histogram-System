/*	FILE			: DP-2.c
 *	PROJECT			: PROG1970 - Assignment #5
 *	PROGRAMMER		: Caine Phung - ID: 6983324
                	Hoang Phuc Tran - ID: 8789102
 *	FIRST VERSION	: 2023-04-11
 *	DESCRIPTION		: The DP-2 is similar to the DP-1 with a few important exceptions and is intended to be launched by the DP-1 programme. 
 Every 1/20 seconds, the DP-2 programme adds one character to the circular buffer. This program spawns the DC application..
 */

#include "../../Common/inc/Common.h"
#include "../inc/DP-2.h"

int main(int argc, char *argv[], char *envp[])
{
	
	char pathName[MAXIMUM_SIZE_ARGUMENT] = "../../DC/bin/DC";
	char parent_PID[MAXIMUM_SIZE_ARGUMENT] = {0};
	char ownPID[MAXIMUM_SIZE_ARGUMENT] = {0};
	BufCircular *_circularBuff;

	// The event handler for the signal
	signal(SIGINT, _signalHandler);
	// Set random number generator		
	srand(time(NULL));

	// Get the shared memory ID from the command line DP-1
	int sharedMemoryID = atoi(argv[1]);

	// the parent PID and current PID
	sprintf(parent_PID, "%d", getppid());
	sprintf(ownPID, "%d", getpid());

	// the argument lists
	char *arguments[MAXIMUM_SIZE_ARGUMENT] = {pathName, argv[1], parent_PID, ownPID, NULL};

	// fork and run the DP-2 process
    if (fork() == 0) 
    {
        if (execve(arguments[0], (char **)arguments, NULL) == -1) 
        {
            printf("failt to process execve child\n");
            return -1;
        }
    }   

	// attach to the shared memory 
	_circularBuff = (BufCircular*)shmat(sharedMemoryID, NULL, 0);		
	
	// Generate random letter from 'A' to 'T'
	createRandomLetter(_circularBuff);

	// Close the Shared Memory
	shmctl(sharedMemoryID, IPC_RMID, NULL);
	return 0;
}

/* 
FUNCTION	: _signalHandler
DESCRIPTION	: This function handles the SIGINT signal by closing the program
PARAMETERS	: int _signal : The signal given
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
PARAMETERS	: char randomLetter - The random letter 
			  BufCircular *_circular - The object pointer contains buffer
RETURNS		: NONE
*/
void writeBuff(char randomLetter, BufCircular *_circularBuff)
{
	// The read and write indecies
	int *readIndex = &(_circularBuff->indexOfRead);
	int *writeIndex = &(_circularBuff->indexOfWrite);

	// Check to see if the semaphore is used or not
	if (semop(_circularBuff->semID, &acquire_operation, 1) == -1)
	{
		printf("Fail to get semaphore!!!\n");
		usleep(25000);
		writeBuff(randomLetter, _circularBuff);
	}
	else
	{		
		// accessing to the semaphore is gained
		if (!(*writeIndex + 1 == *readIndex || (*writeIndex == SIZE_OF_BUFFER - 1 && *readIndex == 0)))
		{
			// Set arrayLetters as the character at the write index.
			memmove(&(_circularBuff->buff[*writeIndex]), &randomLetter, sizeof(randomLetter));	

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

		//printf("DP-2 Releasing Semaphore...\n");
		semop(_circularBuff->semID, &release_operation, 1);
	}
}


/* 
FUNCTION	: createRandomLetter
DESCRIPTION	: This function is used to generate letters from 'A' to 'T'
PARAMETERS	: BufCircular *_circular - The object pointer contains buffer
RETURNS		: NONE
*/
void createRandomLetter(BufCircular* _circularBuff)
{
	int running = 0;

	while (!running)
	{
		//Get random letter from 'A' to 'T'
		char randomLetter = (rand() % MAXIMUM_LETTERS) + 'A';

		// write the array to the circular buffer
		writeBuff(randomLetter, _circularBuff);

		//sleep for 1/20  second
		usleep(50000);
	}	
}