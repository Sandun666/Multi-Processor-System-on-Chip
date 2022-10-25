/*
 * 	FIFO queue for shared memory communication between niosII processors
 *
 *  	DATE		: 18-08-2016
 *      AUTHOR	: Isuru Nawinne
*
*	Structure of the FIFO:
*	___________________________________________________________________________
*	| 	full	| 	empty	| 	count		|	|	|	|	|	|	|	|	|
*	| 	?	| 	?		| 	?		|	|	|	|	|	|	|	|	|
*	___________________________________________________________________________
*/


#include "FIFO_1.h"


void WRITE_FIFO_1(int *buffer)
{

	// Wait if the FIFO is full
	while(IORD_32DIRECT(MEM_BASE,fullp)==1);
	// Write the data to FIFO
	IOWR_32DIRECT(MEM_BASE, writep, *buffer);
	// Update the write pointer
	writep += UNIT_SIZE;
	// Update "count" in shared memory
	IOWR_32DIRECT(MEM_BASE, countp, IORD_32DIRECT(MEM_BASE,countp)+1);
	// Update the "full?" and "empty?" flags accordingly
	if(IORD_32DIRECT(MEM_BASE, countp) == CAPACITY){
		IOWR_32DIRECT(MEM_BASE, fullp, 0x1);
	}
	// Reset the empty flag if FIFO now has atleast one entry
	else if(IORD_32DIRECT(MEM_BASE, countp) > 0){
		IOWR_32DIRECT(MEM_BASE, emptyp, 0x0);
	}
	printf("%d  ",writep);
	//if write pointer hit the end off the queue
	if(writep == STARTP + CAPACITY*UNIT_SIZE)
		writep = STARTP;

}




//Initialization
void FIFO_1_INIT()
{
	writep = STARTP; // Initially the FIFO is empty, so start writing at the first slot
	readpp  = STARTP;
	fullp   = 0; // SET THIS OFFSET (If there are previous FIFOs in shared memory, use Prev Fifo's STARTP + Prev Fifo's size)
	emptyp   = fullp + 0x4;
	countp  = emptyp + 0x4;

	// Assigning values for the flags.
	IOWR_32DIRECT(MEM_BASE, fullp, 0x0);
	IOWR_32DIRECT(MEM_BASE, emptyp, 0x1); // The fifo is empty at the start
	IOWR_32DIRECT(MEM_BASE, countp, 0x0); // The fifo is empty at the start
}
