/*	FILE			: DC.c
 *	PROJECT			: PROG1970 - Assignment #5
 *	PROGRAMMER		: Caine Phung - ID: 6983324
                	  Hoang Phuc Tran - ID: 8789102
 *	FIRST VERSION	: 2023-04-11
 *	DESCRIPTION		: The DC's function is to read data from the DP-1 and DP-2 programs, 
 display it and decode the SIGINT signal when the buffer is read, signalling the program to end. 

 */
#include "../../Common/inc/Common.h"
#include "../inc/DC.h"

// Global variables
int count[MAXIMUM_LETTERS] = {0};
BufCircular *cirBuffer;	
int dp1;
int dp2;
int memory_ID;
int _second;


int main(int argc, char *argv[])
{
	// The event handler for the signal
	signal(SIGINT, _signalHandler);

	// Initialize global variables for signal use
	memory_ID = atoi(argv[1]);
	dp1 = atoi(argv[2]);
	dp2 = atoi(argv[3]);
	_second = 0;
	memset(count, 0, sizeof(count));

	// Verify the shared memory is valid; if not, try again after 10 seconds.
	while (*((int*)(cirBuffer = (BufCircular*)shmat(memory_ID, NULL, 0))) == -1) 
	{ 
		sleep(10); 
	}	
	
	// signal for reading every 2 seconds
	signal(SIGALRM, readFromCircularBuffer);
	alarm(DELAY_READ);

	printAfterTenSecond(_second);

	return 0;
}

/* 
FUNCTION	: _signalHandler
DESCRIPTION	: When the SIGINT signal is received reading from the buffer, printing one final histogram, and stopping the DP-1 and DP-2 processes.
PARAMETERS	: int _signal - The signal given
RETURNS		: NONE
*/
void _signalHandler(int _signal)
{
    signal(_signal, SIG_IGN);

    // Terminate DP-1 and DP-2 
	kill(dp1, SIGINT);
	kill(dp2, SIGINT);

	// read in the buffer 
	readFromCircularBuffer();

	// Display the results
	printSymbol();

	printf("Shazam !!\n");

	// Exit the program
	exit(1);
}

/* 
FUNCTION	: printSymbol
DESCRIPTION	: This function is used to print the symbol to the screen
PARAMETERS	: NONE
RETURNS		: NONE
*/
void printSymbol()
{

	for (int counter = 0; counter < MAXIMUM_LETTERS; counter++) 
	{
		int numberVar = count[counter];

		// Print letters
		printf("%c: ", counter + 'A');

		// Print '*'
		for (int counter = 0; counter < (numberVar % 1000) / 100; counter++)
		{
			printf("*");
		}
		// Print '+'
		for (int counter = 0; counter < (numberVar % 100) / 10; counter++)
		{
			printf("+");
		}
		// Print '-'
		for (int counter = 0; counter < (numberVar % 10); counter++)
		{
			printf("-");
		}

		printf("\n");
	}
		printf("\n");
}

/* 
FUNCTION	: readFromCircularBuffer
DESCRIPTION	: This function begins reading from the circular buffer at the read index and keeps 
reading until it reaches the writing index. SIGALRM triggers an alarm and it resets the alarm so it can be read again later.
PARAMETERS	: NONE
RETURNS		: NONE
*/
void readFromCircularBuffer()
{
	// The read and write indecies
	int* readIndex = &cirBuffer->indexOfRead;
	int* writeIndex = &cirBuffer->indexOfWrite;

	// increment _second by add DELAY_READ
	_second += DELAY_READ;

	// Check to see if the semaphore is used or not
	if (semop(cirBuffer->semID, &acquire_operation, 1) == -1)
	{
		printf("Fail to get semaphore!!!\n");
		usleep(25000);
		readFromCircularBuffer();
	}
	else
	{
		while (*readIndex != *writeIndex)
		{
			//Each time a letter appears in the buffer, count will increase.
			count[cirBuffer->buff[*readIndex] - 'A']++;
			
			// Determine whether or not the write index necessitates wrapping back to the list's beginning.
			if(*readIndex == SIZE_OF_BUFFER - 1)
			{
				*readIndex = 0;
			}
			else
			{
				// If not, move the buffer down one more position.
				(*readIndex)++;
			}
		}

		// free the semaphore
		semop(cirBuffer->semID, &release_operation, 1);	
		
		// Reset the signal
		signal(SIGALRM, readFromCircularBuffer);
		alarm(DELAY_READ);
	}	
}

/* 
FUNCTION	: printAfterTenSecond
DESCRIPTION	: This function is used to print the histogram every 10 seconds
PARAMETERS	: NONE
RETURNS		: NONE
*/
void printAfterTenSecond(int seconds)
{
	int running = 0;

	while (!running)
	{
		if (_second >= 10)
		{	
			// Print the histogram every 10 seconds
			printSymbol();

			_second -= 10;
		}
	}	
}