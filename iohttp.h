#ifndef IOHTTP_H
#define IOHTTP_H
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

int iohttpMimeFind( char *name );

char iohttpInputHex( char *src );

int io_prep();
int io_end();

#endif
