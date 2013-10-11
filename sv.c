#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <fcntl.h>
#if MYSQLENABLE == 1
#include <mysql.h>
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>


#include "config.h"
#include "artefact.h"
#include "db.h"
#include "sv.h"
#include "io.h"
#include "cmd.h"


svConnectionPtr svConnectionList = 0;
fd_set svSelectRead;
fd_set svSelectWrite;
fd_set svSelectError;


#if EVMAPENABLE == 1
#define SV_INTERFACES 2
#else
#define SV_INTERFACES 1
#endif


int svListenPort[SV_INTERFACES] = { HTTP_PORT
#if EVMAPENABLE == 1
, EVMP_PORT
#endif
};
int svListenIO[SV_INTERFACES] = { 0
#if EVMAPENABLE == 1
, 1
#endif
};
int svListenSocket[SV_INTERFACES];

svConnectionPtr svDebugConnection;


#include "svban.c" //Lets try and change this in the future too... Flat file baning is just flat out bad.


int svTime() {
	struct timeval lntime;

gettimeofday( &lntime, 0 );

return ( lntime.tv_sec * 1000 ) + ( lntime.tv_usec / 1000 );
}



int svInit() {
	int a, b, c;
	struct sockaddr_in sinInterface;

c=0;
for( b = 0 ; b < SV_INTERFACES ; b++ ) {
	svListenSocket[b] = socket( AF_INET, SOCK_STREAM, 0 );
	if( svListenSocket[b] == -1 ) {
		#if FORKING == 0
		printf("Error %03d, creating listening socket\n", errno );
		#endif
		syslog(LOG_ERR, "Error %03d, creating listening socket\n", errno );
		continue;
	}
	a = 1;
	if( setsockopt( svListenSocket[b], SOL_SOCKET, SO_REUSEADDR, (char *)&a, sizeof(int) ) == -1 ) {
		#if FORKING == 0
		printf("Error %03d, setsockopt\n", errno );
		#endif
		syslog(LOG_ERR, "Error %03d, setsockopt\n", errno );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	}
	sinInterface.sin_family = AF_INET;
	//sinInterface.sin_addr.s_addr = inet_addr("212.79.239.2");
	//Should work on all server now
	sinInterface.sin_addr.s_addr = htonl(INADDR_ANY);
	sinInterface.sin_port = htons( svListenPort[b] );
	if( bind( svListenSocket[b], (struct sockaddr *)&sinInterface, sizeof(struct sockaddr_in) ) == -1 ) {
		#if FORKING == 0
		printf("Error %03d, binding listening socket to port %d\n", errno, svListenPort[b] );
		#endif
		syslog(LOG_ERR, "Error %03d, binding listening socket to port %d\n", errno, svListenPort[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	}
	if( listen( svListenSocket[b], SOMAXCONN ) == -1 ) {
		#if FORKING == 0
		printf("Error %03d, listen\n", errno, svListenPort[b] );
		#endif
		syslog(LOG_ERR, "Error %03d, listen\n", errno, svListenPort[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	} 
	if( fcntl( svListenSocket[b], F_SETFL, O_NONBLOCK ) == -1 ) {
		#if FORKING == 0
		printf("Error %03d, setting the listening socket to non-blocking\n", errno, svListenPort[b] );
		#endif
		syslog(LOG_ERR, "Error %03d, setting the listening socket to non-blocking\n", errno, svListenPort[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	} else { c++; }
	#if FORKING == 0
	printf("Server awaiting connections on port: %d\n", svListenPort[b] );
	#endif
	syslog(LOG_ERR, "Server awaiting connections on port: %d\n", svListenPort[b] );
}

if ( c == 0 ) {
	#if FORKING == 0
	printf("Server Binding failed, ports are not avalible/allowed!!" );
	#endif
	syslog(LOG_CRIT, "Server Binding failed, ports are not avalible/allowed!!" );
//Empty Return to indicate no ports avaliable, and server can not iniate.
	return 0;
}


//Return to indicate all ports avaliable, and server can iniate.
return 1;
}

void cleanUp(int pipefileid) {
close(pipefileid);
unlink(PIPEFILE);
#if FORKING == 0
printf("Server has been Completly shutdown!\n");
#endif
syslog(LOG_INFO, "Server has been Completly shutdown!\n" );
syslog(LOG_INFO, "<<<<<BREAKER-FOR-NEW-SERVER-INSTANCE>>>>>\n" );
closelog();

return;
}

void svEnd() {
	int a;
	svConnectionPtr cnt, next;
#if FORKING == 0
printf("Shutdown called, begin deamon breakdown...\n" );
#endif
syslog(LOG_INFO, "Shutdown called, begin deamon breakdown...\n" );
for( cnt = svConnectionList ; cnt ; cnt = next ) {
	next = cnt->next;
	svFree( cnt );
}
for( a = 0 ; a < SV_INTERFACES ; a++ ) {
	if( svListenSocket[a] == -1 )
		continue;

   	if( shutdown( svListenSocket[a], 2 ) == -1 )
		#if FORKING == 0
		printf("Error %03d, unable to shutdown listening socket\n", errno );
		#endif
		syslog(LOG_ERR, "Error %03d, unable to shutdown listening socket\n", errno );
 		if( close( svListenSocket[a] ) == -1 ) {
			#if FORKING == 0
			printf("Error %03d, closing socket\n", errno );
			#endif
			syslog(LOG_ERR, "Error %03d, closing socket\n", errno );
		} else {
			#if FORKING == 0
			printf("Server released port: %d\n", svListenPort[a] );
			#endif
			syslog(LOG_INFO, "Server released port: %d\n", svListenPort[a] );
		}

   	}

cleanUp(NULL);
return;
}

int svListen () {
	int a, b, socket;
	struct sockaddr_in sockaddr;
	svConnectionPtr cnt;
	ioInterfacePtr io;

for( b = 0 ; b < SV_INTERFACES ; b++ ) {
	if( svListenSocket[b] == -1 )
		continue;
	a = sizeof( struct sockaddr_in );
	socket = accept( svListenSocket[b], (struct sockaddr *)(&sockaddr), &a );
	if( socket == -1 ) {
		if( errno == EWOULDBLOCK )
			continue;
		#if FORKING == 0
		printf("Error %03d, failed to accept a connection %s\n", errno, inet_ntoa(sockaddr.sin_addr) );
		#endif
		syslog(LOG_ERR, "Error %03d, failed to accept a connection %s\n", errno, inet_ntoa(sockaddr.sin_addr) );
		continue;
		#if FORKING == 0
		printf("This is a critical problem... should we really keep going past this?? - We do anways.\n" );
		#endif
		syslog(LOG_CRIT, "This is a critical problem... should we really keep going past this?? - We do anways.\n" );
	}
	if( socket >= FD_SETSIZE ) {
		#if FORKING == 0
		printf("Error, socket >= FD_SETSIZE, %d\n", socket );
		#endif
		syslog(LOG_ERR, "Error, socket >= FD_SETSIZE, %d\n", socket );
		if( close( socket ) == -1 ) {
			#if FORKING == 0
			printf("Error %03d, unable to close socket\n", errno );
			#endif
			syslog(LOG_ERR, "Error %03d, unable to close socket\n", errno );
		}
	continue;
}
#if SERVER_REPORT_CONNECT == 1
else
#if FORKING == 0
printf("Accepting connection from %s:%d>%d\n", inet_ntoa( sockaddr.sin_addr ), ntohs( sockaddr.sin_port ), socket );
#endif
	syslog(LOG_INFO, "Accepting connection from %s:%d>%d\n", inet_ntoa( sockaddr.sin_addr ), ntohs( sockaddr.sin_port ), socket );
#endif

	if( !( cnt = malloc( sizeof(svConnectionDef) ) ) ) {
		#if FORKING == 0
		printf("ERROR, not enough memory to create a connection structure\n");	
		#endif
		syslog(LOG_ERR, "ERROR, not enough memory to create a connection structure\n" );
		if( close( socket ) == -1 ) {
			#if FORKING == 0
			printf("Error %03d, unable to close socket\n", errno );	
			#endif
			syslog(LOG_ERR, "Error %03d, unable to close socket\n", errno );
		}
		continue;
	}
	cnt->socket = 0;
	cnt->next = 0;
	cnt->previous = 0;
	cnt->time = 0;
	cnt->flags = 0;
	cnt->sendbuf = 0;
	cnt->sendbufpos = 0;
	cnt->sendpos = 0;
	cnt->sendsize = 0;
	cnt->sendflushbuf = 0;
	cnt->sendflushpos = 0;
	cnt->sendstatic = 0;
	cnt->sendstaticsize = 0;
	cnt->io = 0;
	cnt->iodata = 0;
	cnt->dbuser = 0;

	cnt->recv_buf[SERVER_RECV_BUFSIZE] = 0;
	cnt->recv_pos = 0;
	cnt->recv = cnt->recv_buf;
	cnt->recv_max = SERVER_RECV_BUFSIZE;


#if SERVER_NAGLE_BUFFERING == 0
	a = 1;
	if( setsockopt( socket, IPPROTO_TCP, TCP_NODELAY, (char *)&a, sizeof(int) ) == -1 ) {
		#if FORKING == 0
		printf("Error %03d, setsockopt\n", errno );
		#endif
		syslog(LOG_ERR, "Error %03d, setsockopt\n", errno );
	}
#endif

	if( fcntl( socket, F_SETFL, O_NONBLOCK ) == -1 ) {
		#if FORKING == 0
		printf("Error %03d, setting a socket to non-blocking\n", errno );
		#endif
		syslog(LOG_ERR, "Error %03d, setting a socket to non-blocking\n", errno );
	}

	cnt->socket = socket;
	memcpy( &(cnt->sockaddr), &sockaddr, sizeof(struct sockaddr_in) );
	cnt->time = svTime();

	cnt->previous = (void **)&(svConnectionList);
	cnt->next = svConnectionList;
	if( svConnectionList )
		svConnectionList->previous = &(cnt->next);
	svConnectionList = cnt;

	io = cnt->io = &ioInterface[svListenIO[b]];
	svSendInit( cnt, io->outputsize );
	io->inNew( cnt );
}


return 1;
}


void svSelect() {
	int a, rmax;
	svConnectionPtr cnt;
	struct timeval to;
	FD_ZERO( &svSelectRead );
	FD_ZERO( &svSelectWrite );
	FD_ZERO( &svSelectError );
rmax = 0;
for( a = 0 ; a < SV_INTERFACES ; a++ ) {
	if( svListenSocket[a] == -1 )
		continue;
	FD_SET( svListenSocket[a], &svSelectRead );
	if( svListenSocket[a] > rmax )
		rmax = svListenSocket[a];
}

for( cnt = svConnectionList ; cnt ; cnt = cnt->next ) {
	if( cnt->socket > rmax )
		rmax = cnt->socket;
	FD_SET( cnt->socket, &svSelectError );
	if( cnt->flags & ( SV_FLAGS_NEED_WRITE | SV_FLAGS_WRITE_BUFFERED | SV_FLAGS_WRITE_STATIC | SV_FLAGS_TO_CLOSE ) )
		FD_SET( cnt->socket, &svSelectWrite );
	else
		FD_SET( cnt->socket, &svSelectRead );
}

to.tv_usec = ( SERVER_SELECT_MSEC % 1000 ) * 1000;
to.tv_sec = SERVER_SELECT_MSEC / 1000;

if( select( rmax+1, &svSelectRead, &svSelectWrite, &svSelectError, &to ) < 0 ) {
	#if FORKING == 0
	printf("Error %03d, select()\n", errno );
	#endif
	syslog(LOG_ERR, "Error %03d, select()\n", errno );
	return;
}


return;
}


void svRecv() { 
	int a, b, time;
	ioInterfacePtr io;
	svConnectionPtr cnt, next;

time = svTime();

for( cnt = svConnectionList ; cnt ; cnt = next ) {
	svDebugConnection = cnt;
	next = cnt->next;
	io = cnt->io;
	if( ( time - cnt->time >= io->timeout ) && !( cnt->flags & SV_FLAGS_TIMEOUT ) ) {
		#if SERVER_REPORT_ERROR == 1
		#if FORKING == 0
		printf("%s>%d Timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		syslog(LOG_ERR, "%s>%d Timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		io->inError( cnt, 0 );
		cnt->flags |= SV_FLAGS_TIMEOUT;
	}
	if( time - cnt->time >= io->hardtimeout ) {
		#if SERVER_REPORT_ERROR == 1
		#if FORKING == 0
		printf("%s>%d Hard timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		syslog(LOG_ERR, "%s>%d Hard timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		svFree( cnt );
		continue;
	}

	if( cnt->flags & SV_FLAGS_WAIT_CLOSE ) {
		a = recv( cnt->socket, &b, 4, 0 );
		if( ( a == 0 ) || ( ( a == -1 ) && ( errno != EWOULDBLOCK ) ) ) {
			io->inClosed( cnt );
			svFree( cnt );
		}
	continue;
	}

	if( ( cnt->flags & ( SV_FLAGS_NEED_WRITE | SV_FLAGS_WRITE_BUFFERED | SV_FLAGS_WRITE_STATIC ) ) && ( FD_ISSET( cnt->socket, &svSelectWrite ) ) ) {
		if( cnt->flags & SV_FLAGS_NEED_WRITE ) {
			io->outSendReply( cnt );
			cnt->flags &= 0xFFFFFFFF - SV_FLAGS_NEED_WRITE;
			cnt->flags |= SV_FLAGS_WRITE_BUFFERED;
		}
		if( cnt->flags & SV_FLAGS_WRITE_BUFFERED ) {
			if( svSendFlush( cnt ) )
			cnt->flags &= 0xFFFFFFFF - SV_FLAGS_WRITE_BUFFERED;
			cnt->time = time;
		}
		if( ( cnt->flags & ( SV_FLAGS_WRITE_STATIC | SV_FLAGS_WRITE_BUFFERED ) ) == SV_FLAGS_WRITE_STATIC ) {
			svSendStaticFlush( cnt );
			if( !( cnt->sendstaticsize ) )
				cnt->flags &= 0xFFFFFFFF - SV_FLAGS_WRITE_STATIC;
			cnt->time = time;
		}
		if( !( cnt->flags & ( SV_FLAGS_WRITE_STATIC | SV_FLAGS_WRITE_BUFFERED ) ) )
			io->inSendComplete( cnt );
	}

	if( cnt->flags & SV_FLAGS_TO_CLOSE ) {
		svShutdown( cnt );
		cnt->flags |= SV_FLAGS_WAIT_CLOSE;
		continue;
	}

	if( !( FD_ISSET( cnt->socket, &svSelectRead ) ) && !( FD_ISSET( cnt->socket, &svSelectError ) ) )
		continue;

	if( cnt->recv_pos == cnt->recv_max )
		io->inError( cnt, 1 );

	a = recv( cnt->socket, &((cnt->recv)[cnt->recv_pos]), cnt->recv_max - cnt->recv_pos, 0 );
	if( a <= 0 ) {
		if( ( a == -1 ) && ( errno == EWOULDBLOCK ) )
			continue;
		#if SERVER_REPORT_ERROR == 1
		#if FORKING == 0
		printf("Connection to %s>%d died : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		syslog(LOG_INFO, "Connection to %s>%d died : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		io->inClosed( cnt );
		svFree( cnt );
		continue;
	}

	cnt->recv_pos += a;
	cnt->time = time;
	cnt->flags &= 0xFFFFFFFF - SV_FLAGS_TIMEOUT;

	io->inNewData( cnt );
}

return;
}




void svShutdown( svConnectionPtr cnt ) {

#if SERVER_REPORT_CLOSE == 1
#if FORKING == 0
printf("Shuting down connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket );
#endif
syslog(LOG_INFO, "Shuting down connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
#endif
shutdown( cnt->socket, 1 );

return;
}

void svClose( svConnectionPtr cnt ) {

#if SERVER_REPORT_CLOSE == 1
#if FORKING == 0
printf("Closed connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
#endif
syslog(LOG_INFO, "Closed connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
#endif
if( close( cnt->socket ) == -1 )
	syslog(LOG_ERR, "Error %03d, closing socket\n", errno);
cnt->socket = -1;

return;
}

int svFree( svConnectionPtr cnt ) {

svConnectionPtr next;

if( cnt->socket != -1 )
	svClose( cnt );
#if SERVER_REPORT_CLOSE == 1
#if FORKING == 0
printf("Freed connection to %s:%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ) );
#endif
syslog(LOG_INFO, "Freed connection to %s:%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ) );
#endif
svSendEnd( cnt );
next = cnt->next;
*(cnt->previous) = (void *)next;
if( next )
	next->previous = cnt->previous;
if( cnt->iodata ) {
	free( cnt->iodata );
	cnt->iodata = 0;
}

free( cnt );

return 1;
}





int svSendAddBuffer( svBufferPtr *bufferp, int size ) {
	svBufferPtr buffer;
	unsigned char *mem;

if( !( mem = malloc( sizeof(svBufferDef) + size ) ) ) {
	#if FORKING == 0
	printf("Error %03d, add buffer malloc\n", errno );
	#endif
	syslog(LOG_ERR, "Error %03d, add buffer malloc\n", errno );
	return 0;
}

buffer = (svBufferPtr)mem;
buffer->data = &mem[sizeof(svBufferDef)];

buffer->prev = (void **)&(bufferp);
buffer->next = 0;
*bufferp = buffer;

return 1;
}

void svSendInit( svConnectionPtr cnt, int size ) {

svSendEnd( cnt );
if( !( svSendAddBuffer( &(cnt->sendbuf), size ) ) )
	return;
cnt->sendsize = size;
cnt->sendbufpos = cnt->sendbuf;
cnt->sendpos = 0;
cnt->sendflushbuf = cnt->sendbuf;
cnt->sendflushpos = 0;

return;
}

void svSendEnd( svConnectionPtr cnt ) {
	svBufferPtr buffer, next;

for( buffer = cnt->sendbuf ; buffer ; buffer = next ) {
	next = buffer->next;
	free( buffer );
}
cnt->sendbuf = 0;

return;
}

int svSendFlush( svConnectionPtr cnt ) {
	int a, size;

for( ; cnt->sendflushbuf ; cnt->sendflushbuf = (cnt->sendflushbuf)->next ) {
	if( (cnt->sendflushbuf)->next )
		size = cnt->sendsize - cnt->sendflushpos;
	else
		size = cnt->sendpos - cnt->sendflushpos;
	a = send( cnt->socket, &(cnt->sendflushbuf)->data[cnt->sendflushpos], size, 0 );
	if( a == -1 ) {
		if( errno == EWOULDBLOCK )
			return 0;
		#if FORKING == 0
		printf("Error %d, send\n", errno);
		#endif
		syslog(LOG_ERR, "Error %d, send\n", errno);
		return 1;
	}
	if( a != size ) {
		cnt->sendflushpos += a;
		return 0;
	}
	cnt->sendflushpos = 0;
}

cnt->sendbufpos = cnt->sendbuf;
cnt->sendpos = 0;
cnt->sendflushbuf = cnt->sendbuf;
cnt->sendflushpos = 0;

return 1;
}

void svSend( svConnectionPtr cnt, void *data, int size ) {
	int mem;
	svBufferPtr buffer;

for( buffer = cnt->sendbufpos ; ; ) {
	mem = cnt->sendsize - cnt->sendpos;
	if( size <= mem ) {
		memcpy( &buffer->data[cnt->sendpos], data, size );
		cnt->sendpos += size;
		return;
	} else {
		memcpy( &buffer->data[cnt->sendpos], data, mem );
		cnt->sendpos += mem;
		size -= mem;
		data += mem;
		if( !( svSendAddBuffer( (svBufferPtr *)&(buffer->next), cnt->sendsize ) ) )
			return;
		buffer = cnt->sendbufpos = buffer->next;
		cnt->sendpos = 0;
	}
}


return;
}


void svSendString( svConnectionPtr cnt, char *string ) {

svSend( cnt, string, strlen(string) );

return;
}

void svSendPrintf( svConnectionPtr cnt, char *string, ... ) {
	int len;
	char text[4096];
	va_list ap;
	va_start( ap, string );

len = vsnprintf( text, 4096, string, ap );
va_end( ap );
svSend( cnt, text, len );

return;
}



void svSendStaticFlush( svConnectionPtr cnt ) {
int a;

a = send( cnt->socket, cnt->sendstatic, cnt->sendstaticsize, 0 );
if( a == -1 )
	return;
cnt->sendstatic += a;
cnt->sendstaticsize -= a;

return;
}

void svSendStatic( svConnectionPtr cnt, void *data, int size ) {
	cnt->sendstatic = data;
	cnt->sendstaticsize = size;
	cnt->flags |= SV_FLAGS_WRITE_STATIC;

return;
}

int svDebugTickPass;
int svDebugTickId;

void svSignal( int signal ) {
	unsigned char TICKFILE[256];
	int a, size;
	FILE *fFile;  
    
sprintf( TICKFILE, "%s/ticks", COREDIRECTORY );  
    
if(signal == SIGUSR1) {
	svRoundEnd = 1;
		if( ( fFile = fopen( TICKFILE, "r+" ) ) ) {
		fscanf( fFile, "%d", &a );
	   //	fprintf(fFile, " %d", svRoundEnd);
	   	fclose( fFile );
	}
	return;
}

if(signal == SIGUSR2) {
  	//Free memory db and reload it to have a new member in :P
	#if FORKING == 0
	printf("Ask a dbinit\n");
	#endif
	syslog(LOG_INFO, "Ask a dbinit\n");
  	dbEnd();
  	dbInit();
  	return;
}

iohttpDataPtr iohttp;
syslog(LOG_ERR, "ERROR, signal %d\n", signal);
syslog(LOG_ERR, "cnt : %d\n", (int)svDebugConnection);
syslog(LOG_ERR, "tick pass : %d\n", svDebugTickPass);
syslog(LOG_ERR, "tick id : %d\n", svDebugTickId);
#if FORKING == 0
printf( "ERROR, signal %d\n", signal );
fflush( stdout );
printf( "cnt : %d\n", (int)svDebugConnection);
fflush( stdout );
printf( "tick pass : %d\n", svDebugTickPass );
fflush( stdout );
printf( "tick id : %d\n", svDebugTickId );
fflush( stdout );
#endif
if( svDebugConnection ) {
	#if FORKING == 0
	printf( "OK\n" );
	fflush( stdout );
	#endif
	iohttp = svDebugConnection->iodata;
	#if FORKING == 0
	printf( "OK\n" );
	fflush( stdout );
	printf( "iohttp : %d\n", (int)iohttp );
	fflush( stdout );
	printf( "iohttp->path : %s\n", iohttp->path );
	fflush( stdout );
	printf("iottp content lenth: %d\n", iohttp->content_length);
	fflush(stdout);
	printf( "iohttp->content : " );
	printf( "iohttp->query_string : %s\n", iohttp->query_string );
	fflush( stdout );
	printf( "iohttp->cookie : %s\n", iohttp->cookie );
	fflush( stdout );
	printf( "iohttp->referer : %s\n", iohttp->referer );
	fflush( stdout );
	printf( "iohttp->user_agent : %s\n", iohttp->user_agent );
	fflush( stdout );
	fwrite( iohttp->content, 1, iohttp->content_length, stdout );
	fflush( stdout );
	#endif
	syslog(LOG_ERR, "iohttp : %d\n", (int)iohttp );
	syslog(LOG_ERR, "iohttp->path : %s\n", iohttp->path );
	syslog(LOG_ERR, "iottp content lenth: %d\n", iohttp->content_length );
	syslog(LOG_ERR, "iohttp->query_string : %s\n", iohttp->query_string );
	syslog(LOG_ERR, "iohttp->cookie : %s\n", iohttp->cookie );
	syslog(LOG_ERR, "iohttp->referer : %s\n", iohttp->referer );
	syslog(LOG_ERR, "iohttp->user_agent : %s\n", iohttp->user_agent );
	syslog(LOG_ERR, "iohttp->content <<START>> %s <<END>>\n", iohttp->content );

	for( ; svDebugConnection->sendflushbuf ; svDebugConnection->sendflushbuf = (svDebugConnection->sendflushbuf)->next ) {
		if( (svDebugConnection->sendflushbuf)->next )
			size = svDebugConnection->sendsize - svDebugConnection->sendflushpos;
		else
			size = svDebugConnection->sendpos - svDebugConnection->sendflushpos;
		a = size;
		fwrite( &(svDebugConnection->sendflushbuf)->data[svDebugConnection->sendflushpos], 1, size, stdout ); // hmmz ...
		if( a == -1 ) {
			if( errno == EWOULDBLOCK )
				return;
			#if FORKING == 0
			printf("Error %d, send\n", errno);
			#endif
			syslog(LOG_ERR, "Error %d, send\n", errno);
			return;
		}
		if( a != size ) {
			svDebugConnection->sendflushpos += a;
			break;
		}
		svDebugConnection->sendflushpos = 0;
	}
}


cleanUp(NULL);
exit(1);
}


int svTickNum = 0;
int svTickTime;
int svTickStatus = 0;
int svRoundEnd = 0;

int svTickAutoStart = 1;

char *trimwhitespace(char *str) {
	char *end;
// Trim leading space
while(isspace(*str)) str++;

if(*str == 0)  // All spaces?
	return str;

// Trim trailing space
end = str + strlen(str) - 1;
while(end > str && isspace(*end))
	end--;

// Write new null terminator
*(end+1) = 0;

return str;
}

//Read from pipe file... command execution latter to come...
void readfrompipe(int pipefileid){
	int num;
	char buf[256];

num = read(pipefileid, buf, sizeof(buf));
buf[num] = '\0';

if ( num > 0 ) {
	strcpy(trimwhitespace(buf),buf);
	if( strcmp(buf,"die") == 0 ) {
		#if FORKING == 0
		printf("%s\n", "Shutdown command recived from Pipe.");
		#endif
		syslog(LOG_INFO, "%s\n", "Shutdown command recived from Pipe.");
		cleanUp(pipefileid);
		exit(1);
	} else {
		#if FORKING == 0
		printf("Piping Error Unrecognized command size \"%d\" line \"%s\"\n", num, buf);
		#endif
		syslog(LOG_ERR, "Piping Error Unrecognized command size \"%d\" line \"%s\"\n", num, buf);
	}
}


return;
}

//This is the actual loop process, which listens and responds to requests on all sockets.
void daemonloop(int pipefileid) {
	unsigned char TICKFILE[256];
	int a, curtime;
	FILE *file;
	ioInterfacePtr io;

//Replacment server loop, why use "for" when we can use "while" and its so much cleaner?
	while (1) {
		readfrompipe(pipefileid);
		svSelect();
		svListen();
		svRecv();
		svDebugConnection = 0;
		curtime = time( 0 );
		if( curtime < svTickTime )
			continue;

		if(strstr(ctime(&curtime), START_TIME))
			svTickStatus = 1;
		
		if(strstr(ctime(&curtime), STOP_TIME))
			svTickStatus = 0;
			
		svTickTime += SV_TICK_TIME;
		
		for( a = 0 ; a < IO_INTERFACE_NUM ; a++ ) {
		io = &ioInterface[a];
		io->TickStart();
		}
		
		cmdTickInit();
		if( svTickStatus ) {
			cmdTick();
			svTickNum++;
		}
		cmdTickEnd();

		for( a = 0 ; a < IO_INTERFACE_NUM ; a++ ) {
			io = &ioInterface[a];
			io->TickEnd();
		}

		sprintf( TICKFILE, "%s/ticks", COREDIRECTORY );  
		file = fopen( TICKFILE, "r+" );
		if(!file)
			file = fopen( TICKFILE, "w" );
		if(file) {
			fprintf( file, "%d", svTickNum );
			fclose( file );
		}

		cmdExecuteFlush();
		
	}

return;
}

//begin upgrade to daemon, I don't like shell dependancy!
int daemon_init(void) {
	int a;
	int pipingin;
	unsigned char COREDIR[256];
	FILE *file;
	ioInterfacePtr io;
	pid_t pid, sid;

#if FORKING == 1
pid = fork();
if(pid < 0) {
	syslog(LOG_ERR, "Forking Error: %s\n", perror);
	exit(EXIT_FAILURE);
}
if(pid > 0) {
	exit(EXIT_SUCCESS); // So we forked it, suicide time! =D
 }

syslog(LOG_INFO, "%s\n", "Begining initiation of NEctroverse daemon...");

// First, start a new session
if((sid = setsid()) < 0) {
	syslog(LOG_ERR, "%s\n", "setsid has failed, unable to fork into daemon");
	exit(EXIT_FAILURE);
}

// Next, make / the current directory -- daemons like this kinda stuff.
if((chdir("/")) < 0) {
	syslog(LOG_ERR, "%s\n", "chdir has failed, unable to fork into daemon");
	exit(EXIT_FAILURE);
}

// Reset the file mode
umask(0);

//Close unneeded file descriptors
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
#endif

svTickTime = time(0) + SV_TICK_TIME;
sprintf( COREDIR, "%s/ticks", COREDIRECTORY );	
if( ( file = fopen( COREDIR, "r" ) ) ) {
	fscanf( file, "%d", &a );
	//  fscanf( file, "%d", &svRoundEnd );
	fclose( file );
}
	
//Time to set some signals
signal( SIGPIPE, SIG_IGN );
signal( SIGHUP, SIG_IGN );
signal( SIGFPE, &svSignal );
signal( SIGUSR2, &svSignal);
signal( SIGBUS, &svSignal );
signal( SIGILL, &svSignal );
signal( SIGINT, &svSignal );
signal( SIGKILL, &svSignal );
signal( SIGPOLL, &svSignal );
signal( SIGPROF, &svSignal );
signal( SIGQUIT, &svSignal );
signal( SIGSEGV, &svSignal );
signal( SIGSYS, &svSignal );
signal( SIGTERM, &svSignal );
signal( SIGUSR1, &svSignal );
signal( SIGTRAP, &svSignal );
	
srand( time(NULL) ); //Random Init
	
if( !( svInit() ) ) {
	#if FORKING == 0
	printf("Server initialisation failed, exiting\n");
	#endif
	syslog(LOG_ERR, "Server initialisation failed, exiting\n");
	return 0;
}

if( !( dbInit("Database initialisation failed, exiting\n") ) ) {
	#if FORKING == 0
	printf("Database initialisation failed, exiting\n");
	#endif
	syslog(LOG_ERR, "Database initialisation failed, exiting\n");
	return 0;
}
for( a = 0 ; a < IO_INTERFACE_NUM ; a++ ) {
	io = &ioInterface[a];
	io->Init();
}

if( !( cmdInit() ) )  {
	#if FORKING == 0
	printf("Basic Iniation failed, exiting\n");
	#endif
	syslog(LOG_ERR, "Basic Iniation failed, exiting\n");
	return 0;
}
sprintf( COREDIR, "%s/data", COREDIRECTORY );  
if( chdir( COREDIR ) == -1 ) {
	#if FORKING == 0
	printf("Change into Database Dir Failed, exiting\n");
	#endif
	syslog(LOG_ERR, "Change into Database Dir Failed, exiting\n");
	return 0;
}
sprintf( COREDIR, "%s/ticks", COREDIRECTORY );  
if( ( file = fopen( COREDIR, "r" ) ) ) {
	fscanf( file, "%d", &svTickNum );
	fclose( file );
}
	
if( ( svTickAutoStart == 1 ) && ( !svTickStatus ) && (svTickNum))
	svTickStatus = 1;

//add local pipe, for basic commands from shell
mkfifo(PIPEFILE, 0666);
pipingin = open(PIPEFILE, O_RDONLY | O_NONBLOCK);
syslog(LOG_INFO, "%s\n", "Completed initiation of NEctroverse daemon.");
//Now create the loop, this used to take place in here... but I decided to move it =P
daemonloop(pipingin);

cmdEnd();
dbEnd();

for( a = 0 ; a < IO_INTERFACE_NUM ; a++ ) {
	io = &ioInterface[a];
	io->End();
}
svEnd();

return 1;
}

int file_exist (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

char** str_split( char* str, char delim, int* numSplits ) {
	char** ret;
	int retLen;
	char* c;

if ( ( str == NULL ) || ( delim == '\0' ) ) {
	ret = NULL;
        retLen = -1;
} else {
        retLen = 0;
        c = str;

        do {
		if ( *c == delim ) {
                retLen++;
		}
	c++;
	} while ( *c != '\0' );

ret = malloc( ( retLen + 1 ) * sizeof( *ret ) );
ret[retLen] = NULL;

c = str;
retLen = 1;
ret[0] = str;

do {
	if ( *c == delim ) {
		ret[retLen++] = &c[1];
                *c = '\0';
	}

	c++;
} while ( *c != '\0' );

}

if ( numSplits != NULL ) {
	*numSplits = retLen;
}


return ret;
}

//And just to do some basic stuff... verify directory structure by attempted creation.
//Does not check for existance, but ignores error for existing dir, other errors are loged.
void dirstructurecheck(char *directory) {
	int i, num, check;
	char mkthisdir[256];
 	char* strCpy;
	char** split;

strCpy = malloc( strlen( directory ) * sizeof( strCpy ) );
strcpy( strCpy, directory );
strcpy( mkthisdir, "" );
split = str_split( strCpy, '/', &num );

if ( split == NULL ) {
	puts( "str_split returned NULL" );
} else {
	for ( i = 0; i < num; i++ ) {
		if( !( i == NULL ) ) {
			strcat(mkthisdir, "/");
			strcat(mkthisdir, split[i]);
			check = mkdir(mkthisdir,0755);
			if (!check) {
				#if FORKING == 0
				printf("Directory \"%s\" created.\n", mkthisdir );
				#endif
				syslog(LOG_INFO, "Directory \"%s\" created.\n", mkthisdir );
			} else if ( errno != 17 ) {
				#if FORKING == 0
				printf("Error creating directory: \"%s\"\n", mkthisdir );
				#endif
				syslog(LOG_ERR, "Error creating directory: \"%s\"\n", mkthisdir );
			}
		}
	}
}
free( split );
free( strCpy );


return;
}

int main() {
	unsigned char COREDIR[256];
	char buf[256];
	int num, fd;
//Proper logging facility -- can change to LOG_LOCAL* or even LOG_SYSLOG etc.
openlog(LOGTAG, LOG_CONS | LOG_PID | LOG_NDELAY, LOGFAC);

//check basic dir structure and create as needed.	
sprintf( COREDIR, "%s/data", COREDIRECTORY );
dirstructurecheck(COREDIR);
sprintf( COREDIR, "%s/users", COREDIRECTORY );
dirstructurecheck(COREDIR);
sprintf( COREDIR, "%s/logs", COREDIRECTORY );
dirstructurecheck(COREDIR);
//well its not really public yet now is it...
//sprintf( fname, "%s/forum", COREDIRECTORY );
//dirstructurecheck(fname);
sprintf( COREDIR, "%s/data/map", COREDIRECTORY );
if( !( file_exist(COREDIR) ) ) {
	#if FORKING == 0
	printf("No map detected... now generating...\n");
	#endif
	syslog(LOG_INFO, "No map detected... now generating...\n");
	mapgen();
}
if ( file_exist(PIPEFILE) ) {
printf("%s\n","Pipe file detected, auto switching to client mode");
//exit(1);
} else {
	printf("%s\n", "Server process iniating...");
#if FORKING == 1
	printf("%s\n", "You'll have to check logs to see if anything went wrong for now...");
	printf("%s\n", "Returning to shell, daemon takes over now.");
#endif
//Begin deamonization and initate server loop.
	if( !( daemon_init() ) ) {
	printf( "Can not load, oh this is bad...\n" );
	syslog(LOG_CRIT, "<<< !!!! CRITICAL ERROR !!!! >>>\n");
	return 0;
	} 
}

printf("%s\n", "Please input command to send to server...");

        if ((fd = open(PIPEFILE, O_WRONLY)) < 0)
            perror("Open Pipe for Write");

        printf("Daemon listening on Pipe... Typing \"die\" kills server.\n");
        while( file_exist(PIPEFILE) && fgets(buf, sizeof(buf), stdin), !feof(stdin) ) {

            if ((num = write(fd, buf, strlen(buf))) < 0)
                perror("Write To Pipe");
            else
                printf("Wrote %d bytes to pipe\n", num);
	sleep(1);        
	}

        close(fd);

//We should never get here... parent proccess is set to self destruct! -- Yer, we do now... haha.
//printf( "Mooooooooooooooo, just to be a cow... I made it to this line, which I never should!!!\n" );

return 1;
}



