// Aaron Itzkovitz
// Dr. Butler CSCI 3240
// P3
// 3/15/17

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define segment struct.
struct vm_area_struct{
	struct vm_area_struct *next;
	char flag;
	void *start_addr;
	int size;
	int extra_space;
};
struct vm_area_struct* top;


// Function to initialize heap before allocation.
void heap_init( int num_pages_for_heap )
{
	int init_sz;
	void *heap_ptr;
	init_sz = num_pages_for_heap * getpagesize();
	// Let system pick address, read and write, anonymous so we don't need FD, 0 offset.
	heap_ptr = mmap( NULL, init_sz, PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE, -1, 0 );
	// Fill byte string.
	memset( heap_ptr, 'a', init_sz );

	// Actually allocate the heap space and set area props
	top = (struct vm_area_struct* )malloc(sizeof(struct vm_area_struct) );
	
	// We still have all the space left.
	top->extra_space = init_sz;

	// We are still at the address returned by mmap
	top->start_addr = heap_ptr;

	// This whole block is now used
	top->flag = 'u';

	// So far nothing is used up
	top->size = 0;

	// Nothing else has been allocated yet.
	top->next = NULL;
	
    return;
}

// Function to actually allocate after initialization.
void *heap_alloc(int num_bytes_to_allocate)
{
	// Hold the current block
	struct vm_area_struct *tmp;

	// Hold the new block
	struct vm_area_struct *new;

	// Hold the offset for new block
	long int offset;
	tmp = top;

	// Look for the next free block
	while( tmp->next != NULL ){

		// If this block is free and the size is greater than what we need, set flag and size, return address
		if( ( tmp->flag == 'f' ) && ( tmp->size >= num_bytes_to_allocate ) ) {
			tmp->size = num_bytes_to_allocate;
			tmp->flag = 'u';
			return tmp->start_addr;
		
		}

		// If the heap_init memory is out of space return null. 
		if( tmp->extra_space < num_bytes_to_allocate ){
			return NULL;
		}
		
		// Else move on to next block.
		tmp = tmp->next;
	}	
	
	// Get the size of the last block to make sure we're on a 16 byte boundary.
	offset = tmp->size;
	while( ( ( long int ) tmp->start_addr + offset ) % 16 != 0){
		
		offset += 1;

		// if we run out of space, return null
		if( num_bytes_to_allocate + offset > tmp->extra_space ){
			return NULL;
		}
	}
	
	// we're at a 16 byte boundary so allocate
	new = (struct vm_area_struct * ) malloc( sizeof( struct vm_area_struct ) );
	
	new->extra_space = tmp->extra_space - offset;
	
	new->size = num_bytes_to_allocate;
	
	new->start_addr = tmp->start_addr + offset;

	tmp->next = new;
	
	new->flag = 'u';
	
	new->next = NULL;		
	
	return new->start_addr; 
}

// Function to free memory space.
void heap_free( void *pointer_to_area_to_free )
{
	// Hold the previous area block.
	struct vm_area_struct *prev;

	// Hold the current memory block.
	struct vm_area_struct *curr;
	
	// Start at the top;
	curr = top;

	// Cycle through to find the supplied address.
	while ( curr->start_addr != pointer_to_area_to_free ){

		prev = curr;
		curr = curr->next;

	}

	// We found it so unset the flag.
	curr->flag = 'f';

	// Now check surrounding to coalesce memory.
	if ( curr->next->flag == 'f' ){

		// Update current area.
		curr->size += curr->next->size; 
		curr->extra_space += curr->next->extra_space;
		curr->next = curr->next->next;

	}
	if ( prev->flag == 'f' ){

		prev->size += curr->size;
		prev->extra_space += prev->size;
		prev->next = curr->next;

	}

}