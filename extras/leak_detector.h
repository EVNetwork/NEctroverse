#ifndef  LEAK_DETECTOR_C_H
#define  LEAK_DETECTOR_C_H

#define  FILE_NAME_LENGTH   	   256
#define  OUTPUT_FILE			   "/tmp/leak_info.txt"
#define  malloc(size) 	    	   	xmalloc (size, __FILE__, __LINE__, __FUNCTION__ )
#define  calloc(elements, size)  	xcalloc (elements, size, __FILE__, __LINE__, __FUNCTION__)
#define  free(mem_ref) 		  	 	xfree(mem_ref)

struct _MEM_INFO
{
	void			*address;
	unsigned int	size;
	char			file_name[FILE_NAME_LENGTH];
	unsigned int	line;
	char			function[FILE_NAME_LENGTH];
};
typedef struct _MEM_INFO MEM_INFO;

struct _MEM_LEAK {
	MEM_INFO mem_info;
	struct _MEM_LEAK * next;
};
typedef struct _MEM_LEAK MEM_LEAK;

void leakdetector_add(MEM_INFO alloc_info);
void leakdetector_erase(unsigned pos);
void leakdetector_clear(void);

void * xmalloc(unsigned int size, const char * file, unsigned int line, const char * function);
void * xcalloc(unsigned int elements, unsigned int size, const char * file, unsigned int line, const char * function);
void xfree(void * mem_ref);

void add_mem_info (void * mem_ref, unsigned int size,  const char * file, unsigned int line, const char * function);
void remove_mem_info (void * mem_ref);
void report_mem_leak(void);

#endif
