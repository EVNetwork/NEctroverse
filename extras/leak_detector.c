#include	"../config/global.h"

#include	"leak_detector.h"

#undef		malloc
#undef		calloc
#undef 		free


static MEM_LEAK * ptr_start = NULL;
static MEM_LEAK * ptr_next =  NULL;

/*
 * adds allocated memory info. into the list
 *
 */
void add(MEM_INFO alloc_info)
{

	MEM_LEAK * mem_leak_info = NULL;
	mem_leak_info = (MEM_LEAK *) malloc (sizeof(MEM_LEAK));
	mem_leak_info->mem_info.address = alloc_info.address;
	mem_leak_info->mem_info.size = alloc_info.size;
	strcpy(mem_leak_info->mem_info.file_name, alloc_info.file_name);
	strcpy(mem_leak_info->mem_info.function, alloc_info.function);
	mem_leak_info->mem_info.line = alloc_info.line;
	mem_leak_info->next = NULL;

	if (ptr_start == NULL)
	{
		ptr_start = mem_leak_info;
		ptr_next = ptr_start;
	}
	else {
		ptr_next->next = mem_leak_info;
		ptr_next = ptr_next->next;
	}

}

/*
 * erases memory info. from the list
 *
 */
void erase(unsigned pos)
{

	unsigned index = 0;
	MEM_LEAK * alloc_info, * temp;

	if(pos == 0)
	{
		MEM_LEAK * temp = ptr_start;
		ptr_start = ptr_start->next;
		free(temp);
	}
	else
	{
		for(index = 0, alloc_info = ptr_start; index < pos;
			alloc_info = alloc_info->next, ++index)
		{
			if(pos == index + 1)
			{
				temp = alloc_info->next;
				alloc_info->next =  temp->next;
				free(temp);
				break;
			}
		}
	}
}

/*
 * deletes all the elements from the list
 */
void clear()
{
	MEM_LEAK * temp = ptr_start;
	MEM_LEAK * alloc_info = ptr_start;

	while(alloc_info != NULL)
	{
		alloc_info = alloc_info->next;
		free(temp);
		temp = alloc_info;
	}
}

/*
 * replacement of malloc
 */
void * xmalloc (unsigned int size, const char * file, unsigned int line, const char * function)
{
	void * ptr = malloc (size);
	if (ptr != NULL)
	{
		add_mem_info(ptr, size, file, line, function);
	}
	return ptr;
}

/*
 * replacement of calloc
 */
void * xcalloc (unsigned int elements, unsigned int size, const char * file, unsigned int line, const char * function)
{
	unsigned total_size;
	void * ptr = calloc(elements , size);
	if(ptr != NULL)
	{
		total_size = elements * size;
		add_mem_info (ptr, total_size, file, line, function);
	}
	return ptr;
}


/*
 * replacement of free
 */
void xfree(void * mem_ref)
{
	remove_mem_info(mem_ref);
	free(mem_ref);
}

/*
 * gets the allocated memory info and adds it to a list
 *
 */
void add_mem_info (void * mem_ref, unsigned int size,  const char * file, unsigned int line, const char * function)
{
	MEM_INFO mem_alloc_info;

	/* fill up the structure with all info */
	memset( &mem_alloc_info, 0, sizeof ( mem_alloc_info ) );
	mem_alloc_info.address 	= mem_ref;
	mem_alloc_info.size = size;
	strncpy(mem_alloc_info.file_name, file, FILE_NAME_LENGTH);
	strncpy(mem_alloc_info.function, function, FILE_NAME_LENGTH);
	mem_alloc_info.line = line;
	/* add the above info to a list */
	add(mem_alloc_info);
}

/*
 * if the allocated memory info is part of the list, removes it
 *
 */
void remove_mem_info (void * mem_ref)
{
	unsigned short index;
	MEM_LEAK  * leak_info = ptr_start;

	/* check if allocate memory is in our list */
	for(index = 0; leak_info != NULL; ++index, leak_info = leak_info->next)
	{
		if ( leak_info->mem_info.address == mem_ref )
		{
			erase ( index );
			break;
		}
	}
}

/*
 * writes all info of the unallocated memory into a file
 */
void report_mem_leak(void) {
	int a = 0;
	MEM_LEAK * leak_info;

info( "Memory Leak Summary" );

for(leak_info = ptr_start; leak_info != NULL; leak_info = leak_info->next, a++) {
	info( "-----------------------------------" );
	sprintf(logString, "address : %p", leak_info->mem_info.address);
	info( logString );
	sprintf(logString, "size    : %d bytes", leak_info->mem_info.size);
	info( logString );
	sprintf(logString, "file    : %s", leak_info->mem_info.file_name);
	info( logString );
	sprintf(logString, "line    : %d", leak_info->mem_info.line);
	info( logString );
	sprintf(logString, "function: %s", leak_info->mem_info.function);
	info( logString );
}

if( a == 0 ) {
	info( "--- None detected =)" );
} else {
	info( "-----------------------------------" );
	sprintf(logString, "%d Memory leaks detected...", a);
	info( logString );
}

clear();

return;
}
