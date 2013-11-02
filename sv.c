#ifndef GLOBALINCLUDED
#include "global.h"
#endif

#include "ini.c"

svConnectionPtr svConnectionList = 0;
fd_set svSelectRead;
fd_set svSelectWrite;
fd_set svSelectError;

configDef sysconfig = { "NEctroverse", "", "", "", "", "", "", -1, false, false, false, 3306, true, 0, false, 0, "", "LOG_SYSLOG" };
optionsDef options = { MODE_DAEMON, { false, false }, 0, -1, -1, true, "", "", "", "status" };
mySqlDef mysqlcfg = { false, "localhost", 3306, "", "", "evcore_database" };
mapcfgDef mapcfg = { 0, 0, 0, 0, 0, 20, 1024.0, 60, 8.0, 2, 24, 0 };
adminDef admincfg;
tickDef ticks;



int svListenSocket[PORT_TOTAL];

svConnectionPtr svDebugConnection;

svbanDef banlist = { 0 };

int svTime() {
	struct timeval lntime;

gettimeofday( &lntime, 0 );

return ( lntime.tv_sec * 1000 ) + ( lntime.tv_usec / 1000 );
}



int svInit() {
	int a, b, c;
	struct sockaddr_in sinInterface;

c=0;
for( b = 0 ; b < options.interfaces ; b++ ) {
	svListenSocket[b] = socket( AF_INET, SOCK_STREAM, 0 );
	if( svListenSocket[b] == -1 ) {
		if( options.verbose )
		printf("Error %03d, creating listening socket\n", errno );
		syslog(LOG_ERR, "Error %03d, creating listening socket\n", errno );
		continue;
	}
	a = 1;
	if( setsockopt( svListenSocket[b], SOL_SOCKET, SO_REUSEADDR, (char *)&a, sizeof(int) ) == -1 ) {
		if( options.verbose )
		printf("Error %03d, setsockopt\n", errno );
		syslog(LOG_ERR, "Error %03d, setsockopt\n", errno );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	}
	sinInterface.sin_family = AF_INET;
	sinInterface.sin_addr.s_addr = htonl(INADDR_ANY);
	sinInterface.sin_port = htons( options.port[b] );
	if( bind( svListenSocket[b], (struct sockaddr *)&sinInterface, sizeof(struct sockaddr_in) ) == -1 ) {
		if( options.verbose )
		printf("Error %03d, binding listening socket to port: %d\n", errno, options.port[b] );
		syslog(LOG_ERR, "Error %03d, binding listening socket to port: %d\n", errno, options.port[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	}
	if( listen( svListenSocket[b], SOMAXCONN ) == -1 ) {
		if( options.verbose )
		printf("Error %03d, listen on port: %d\n", errno, options.port[b] );
		syslog(LOG_ERR, "Error %03d, listen on port: %d\n", errno, options.port[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	} 
	if( fcntl( svListenSocket[b], F_SETFL, O_NONBLOCK ) == -1 ) {
		if( options.verbose )
		printf("Error %03d, setting non-blocking on port: %d\n", errno, options.port[b] );
		syslog(LOG_ERR, "Error %03d, setting non-blocking on port: %d\n", errno, options.port[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	} else { c++; }
	if( options.verbose )
	printf("Server awaiting connections on port: %d\n", options.port[b] );
	syslog(LOG_ERR, "Server awaiting connections on port: %d\n", options.port[b] );
}

if ( c == 0 ) {
	if( options.verbose )
	printf("Server Binding failed, ports are not avalible/allowed!!\n" );
	syslog(LOG_CRIT, "Server Binding failed, ports are not avalible/allowed!!\n" );
//Empty Return to indicate no ports avaliable, and server can not iniate.
	return 0;
}


//Return to indicate all ports avaliable, and server can iniate.
return 1;
}

void cleanUp(int pipefileid, int type) {
	char DIRCHECKER[256];

close(pipefileid);


if( type ) {
sprintf( DIRCHECKER, "%s/%s.%d.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
unlink(DIRCHECKER);
syslog(LOG_INFO, "Server has been Completly shutdown!\n" );
syslog(LOG_INFO, "<<<<<BREAKER-FOR-NEW-SERVER-INSTANCE>>>>>\n" );
closelog();
if( options.verbose )
printf("Server has been Completly shutdown!\n");
} else {
sprintf( DIRCHECKER, "%s/%s.%d.client.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
unlink(DIRCHECKER);
}

return;
}

void svEnd() {
	int a;
	svConnectionPtr cnt, next;
if( options.verbose )
printf("Shutdown called, begin deamon breakdown...\n" );
syslog(LOG_INFO, "Shutdown called, begin deamon breakdown...\n" );
for( cnt = svConnectionList ; cnt ; cnt = next ) {
	next = cnt->next;
	svFree( cnt );
}
for( a = 0 ; a < options.interfaces ; a++ ) {
	if( svListenSocket[a] == -1 )
		continue;

   	if( shutdown( options.port[a], 2 ) == -1 )
		if( options.verbose )
		printf("Error %03d, unable to shutdown listening socket\n", errno );
		syslog(LOG_ERR, "Error %03d, unable to shutdown listening socket\n", errno );
 		if( close( svListenSocket[a] ) == -1 ) {
			if( options.verbose )
			printf("Error %03d, closing socket\n", errno );
			syslog(LOG_ERR, "Error %03d, closing socket\n", errno );
		} else {
			if( options.verbose )
			printf("Server released port: %d\n", options.port[a] );
			syslog(LOG_INFO, "Server released port: %d\n", options.port[a] );
		}

   	}

cleanUp(-1,1);
return;
}

int svListen () {
	int a, b, socket;
	struct sockaddr_in sockaddr;
	svConnectionPtr cnt;
	ioInterfacePtr io;

for( b = 0 ; b < options.interfaces ; b++ ) {
	if( svListenSocket[b] == -1 )
		continue;
	a = sizeof( struct sockaddr_in );
	socket = accept( svListenSocket[b], (struct sockaddr *)(&sockaddr), (socklen_t * __restrict__)&a );
	if( socket == -1 ) {
		if( errno == EWOULDBLOCK )
			continue;
		if( options.verbose )
		printf("Error %03d, failed to accept a connection %s\n", errno, inet_ntoa(sockaddr.sin_addr) );
		syslog(LOG_ERR, "Error %03d, failed to accept a connection %s\n", errno, inet_ntoa(sockaddr.sin_addr) );
		continue;
		if( options.verbose )
		printf("This is a critical problem... should we really keep going past this?? - We do anways.\n" );
		syslog(LOG_CRIT, "This is a critical problem... should we really keep going past this?? - We do anways.\n" );
	}
	if( socket >= FD_SETSIZE ) {
		if( options.verbose )
		printf("Error, socket >= FD_SETSIZE, %d\n", socket );
		syslog(LOG_ERR, "Error, socket >= FD_SETSIZE, %d\n", socket );
		if( close( socket ) == -1 ) {
			if( options.verbose )
			printf("Error %03d, unable to close socket\n", errno );
			syslog(LOG_ERR, "Error %03d, unable to close socket\n", errno );
		}
	continue;
}
#if SERVER_REPORT_CONNECT == 1
else
if( options.verbose )
printf("Accepting connection from %s:%d>%d\n", inet_ntoa( sockaddr.sin_addr ), ntohs( sockaddr.sin_port ), socket );
syslog(LOG_INFO, "Accepting connection from %s:%d>%d\n", inet_ntoa( sockaddr.sin_addr ), ntohs( sockaddr.sin_port ), socket );
#endif

	if( !( cnt = malloc( sizeof(svConnectionDef) ) ) ) {
		if( options.verbose )
		printf("ERROR, not enough memory to create a connection structure\n");	
		syslog(LOG_ERR, "ERROR, not enough memory to create a connection structure\n" );
		if( close( socket ) == -1 ) {
			if( options.verbose )
			printf("Error %03d, unable to close socket\n", errno );	
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
		if( options.verbose )
		printf("Error %03d, setsockopt\n", errno );
		syslog(LOG_ERR, "Error %03d, setsockopt\n", errno );
	}
#endif

	if( fcntl( socket, F_SETFL, O_NONBLOCK ) == -1 ) {
		if( options.verbose )
		printf("Error %03d, setting a socket to non-blocking\n", errno );
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

	io = cnt->io = &ioInterface[ ( (b == PORT_EVMP) ? true : false ) ];
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
for( a = 0 ; a < options.interfaces ; a++ ) {
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
	if( options.verbose )
	printf("Error %03d, select()\n", errno );
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
		if( options.verbose )
		printf("%s>%d Timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		syslog(LOG_ERR, "%s>%d Timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		io->inError( cnt, 0 );
		cnt->flags |= SV_FLAGS_TIMEOUT;
	}
	if( time - cnt->time >= io->hardtimeout ) {
		#if SERVER_REPORT_ERROR == 1
		if( options.verbose )
		printf("%s>%d Hard timeout : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
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
		if( options.verbose )
		printf("Connection to %s>%d died : %d\n", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
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
if( options.verbose )
	printf("Shuting down connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket );
syslog(LOG_INFO, "Shuting down connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
#endif
shutdown( cnt->socket, 1 );

return;
}

void svClose( svConnectionPtr cnt ) {

#if SERVER_REPORT_CLOSE == 1
if( options.verbose )
	printf("Closed connection to %s:%d>%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
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
if( options.verbose )
	printf("Freed connection to %s:%d\n", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ) );
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
	char *mem;

if( !( mem = malloc( sizeof(svBufferDef) + size ) ) ) {
	if( options.verbose )
	printf("Error %03d, add buffer malloc\n", errno );
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
		if( options.verbose )
		printf("Error %d, send flush\n", errno);
		syslog(LOG_ERR, "Error %d, send flush\n", errno);
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


void svSignal( int signal ) {
	int a, size;
 

iohttpDataPtr iohttp;
syslog(LOG_ERR, "ERROR, signal %d\n", signal);
syslog(LOG_ERR, "cnt : %d\n", (int)(intptr_t)svDebugConnection);
syslog(LOG_ERR, "tick pass : %d\n", ticks.debug_pass);
syslog(LOG_ERR, "tick id : %d\n", ticks.debug_id);
if( options.verbose ) {
	printf( "ERROR, signal %d\n", signal );
	fflush( stdout );
	printf( "cnt : %d\n", (int)(intptr_t)svDebugConnection);
	fflush( stdout );
	printf( "tick pass : %d\n", ticks.debug_pass );
	fflush( stdout );
	printf( "tick id : %d\n", ticks.debug_id );
	fflush( stdout );
}
if( svDebugConnection ) {
	if( options.verbose ) {
		printf( "OK\n" );
		fflush( stdout );
	}
	iohttp = svDebugConnection->iodata;
	if( options.verbose ) {
		printf( "OK\n" );
		fflush( stdout );
		printf( "iohttp : %d\n", (int)(intptr_t)iohttp );
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
	}
	syslog(LOG_ERR, "iohttp : %d\n", (int)(intptr_t)iohttp );
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
		if( options.verbose )
		fwrite( &(svDebugConnection->sendflushbuf)->data[svDebugConnection->sendflushpos], 1, size, stdout ); // hmmz ...
		if( a == -1 ) {
			if( errno == EWOULDBLOCK )
				return;
			if( options.verbose )
			printf("Error %d, send\n", errno);
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


cleanUp(-1,1);
exit(1);
}

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
void svPipeScan(int pipefileid){
	int num, stop;
	char DIRCHECKER[256];
	char buffer[128] = {0};

if(pipefileid < 0 )
return;

num = read(pipefileid, buffer, sizeof(buffer));
buffer[num] = '\0';
stop = 0;
if ( ( num > 0 ) && strlen(buffer) ) {
	if( !(strcmp(buffer,"stop") ) ) {
		stop = 1;
	} else {
		if( options.verbose )
		printf("Piping Error Unrecognized command size \"%d\" line \"%s\"\n", num, buffer);
		syslog(LOG_ERR, "Piping Error Unrecognized command size \"%d\" line \"%s\"\n", num, buffer);
	}
	sprintf(DIRCHECKER, "Recived command \"%s\", I will now process.", buffer );
	svPipeSend(0,DIRCHECKER);
}

if( stop ) {
	svPipeSend(0,"Server is shutting down as requested..");
	if( options.verbose )
	printf("%s\n", "Shutdown command recived from Pipe.");
	syslog(LOG_INFO, "%s\n", "Shutdown command recived from Pipe.");
}

if ( num > 0 ) {
	svPipeSend(0,"Yep, thats all flokes...");
	svPipeSend(0,"<<<END>>>");
}

//FIXME Lets move this latter.
if( stop ) {
	cleanUp(pipefileid,1);
	exit(1);
}

return;
}

//Respond to client, let them know we have the command.
int svPipeSend(int pipedirection, char *message){
	int num;
	FILE *pipefile;
	char DIRCHECKER[256];

sprintf( DIRCHECKER, "%s/%s.%d.%s", TMPDIR, options.pipefile, options.port[PORT_HTTP], ( pipedirection ? "pipe" : "client.pipe" ) );
if( file_exist(DIRCHECKER) && strlen(message) ) {
	if( ( pipefile = fopen(DIRCHECKER, "w") ) < 0) {
		if( options.verbose )
		printf( "Piping Error: unable to open pipe for write: %s", DIRCHECKER );
		syslog(LOG_ERR, "Piping Error: unable to open pipe for write: %s", DIRCHECKER );
		return 0;
	}
	if( ( num = fprintf(pipefile, "%s", message) ) < 0) {
		if( options.verbose )
		printf( "Piping Responce Error: unable to write to pipe: %s", DIRCHECKER );
		syslog(LOG_ERR, "Piping Responce Error: unable to write to pipe: %s", DIRCHECKER );
		return 0;
	}
	fflush(pipefile);
	fclose(pipefile);
} else {
	if( options.verbose )
	printf( "Piping Error: message to send but no pipe avaliable" );
	syslog(LOG_ERR, "Piping Error: message to send but no pipe avaliable" );
	return 0;
}

if( options.verbose )
	fflush(stdout);

return 1;
}

//This is the actual loop process, which listens and responds to requests on all sockets.
void daemonloop() {
	ioInterfacePtr io;
	int a, curtime;

//Replacment server loop, why use "for" when we can use "while" and its so much cleaner?
	while (1) {
		svPipeScan( options.serverpipe );
		loadconfig(options.sysini,CONFIG_BANNED);
		svSelect();
		svListen();
		svRecv();
		svDebugConnection = 0;
		curtime = time( 0 );
		if( curtime < ticks.next )
			continue;

		ticks.next += sysconfig.ticktime;
		
		for( a = 0 ; a < options.interfaces ; a++ ) {
		io = &ioInterface[a];
		io->TickStart();
		}
		
		cmdTickInit();
		if( ticks.status ) {
			cmdTick();
			ticks.number++;
		}
		cmdTickEnd();

		for( a = 0 ; a < options.interfaces ; a++ ) {
			io = &ioInterface[a];
			io->TickEnd();
		}


		cmdExecuteFlush();
		
	}

return;
}

//begin upgrade to daemon, I don't like shell dependancy!
int daemon_init() {
	int a;
	int binfo[MAP_TOTAL_INFO];
	char DIRCHECKER[256];
	ioInterfacePtr io;
	pid_t pid, sid;

printf("%s\n", "Server process iniating...");

if( options.mode == MODE_FORKED ) {
pid = fork();
if(pid < 0) {
	syslog(LOG_ERR, "Forking Error: %d\n", errno);
	return 0;
} else if(pid > 0) {
	return 1; // So we forked it, time to return and wait for results on a client pipe.
}

syslog(LOG_INFO, "Begining initiation of %s daemon...", sysconfig.servername);

// First, start a new session
if((sid = setsid()) < 0) {
	syslog(LOG_ERR, "%s\n", "setsid has failed, unable to fork into daemon");
	return 0;
}

// Next, make / the current directory.
if((chdir("/")) < 0) {
	syslog(LOG_ERR, "%s\n", "chdir has failed, unable to fork into daemon");
	return 0;
}

// Reset the file mode
umask(0);

//Close unneeded file descriptors
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);

}

ticks.next = time(0) + sysconfig.ticktime;
	
//Time to set some signals
signal( SIGPIPE, SIG_IGN );
signal( SIGHUP, SIG_IGN );
signal( SIGFPE, &svSignal );
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
signal( SIGTRAP, &svSignal );
signal( SIGABRT, &svSignal );
	
srand( time(NULL) ); //Random Init
	
if( !( svInit() ) ) {
	if( options.verbose )
	printf("Server initialisation failed, exiting\n");
	syslog(LOG_ERR, "Server initialisation failed, exiting\n");
	return 0;
}

if( !( dbInit("Database initialisation failed, exiting\n") ) ) {
	if( options.verbose )
	printf("Database initialisation failed, exiting\n");
	syslog(LOG_ERR, "Database initialisation failed, exiting\n");
	return 0;
}
for( a = 0 ; a < options.interfaces ; a++ ) {
	io = &ioInterface[a];
	io->Init();
}

if( !( cmdInit() ) )  {
	if( options.verbose )
	printf("Basic Iniation failed, exiting\n");
	syslog(LOG_ERR, "Basic Iniation failed, exiting\n");
	return 0;
}
sprintf( DIRCHECKER, "%s/data", sysconfig.directory );  
if( chdir( DIRCHECKER ) == -1 ) {
	if( options.verbose )
	printf("Change into Database Dir Failed, exiting\n");
	syslog(LOG_ERR, "Change into Database Dir Failed, exiting\n");
	return 0;
}

dbMapRetrieveMain( binfo );
if( ( binfo[MAP_ARTITIMER] == -1 ) || !( (binfo[MAP_ARTITIMER] - ticks.number) <= 0 ) ) {
	if( ( sysconfig.autoresume == 1 ) && ( !ticks.status ) && (ticks.number) )
		ticks.status = 1;
}

//add local pipe, for basic commands from shell

sprintf( DIRCHECKER, "%s/%s.%d.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
	if( mkfifo(DIRCHECKER, 0666) < 0 ) {
	syslog(LOG_INFO, "Error creating pipe: %s\n", DIRCHECKER );
	if( options.verbose )
	printf("Error creating pipe: %s\n", DIRCHECKER );
	options.serverpipe = -1;
} else {
	options.serverpipe = open(DIRCHECKER, O_RDONLY | O_NONBLOCK);
}
syslog(LOG_INFO, "Completed initiation of %s daemon.\n", sysconfig.servername );
if( options.verbose ) {
	printf("All checks passed, begining server loop...\n");
	fflush(stdout);
}

//Now create the loop, this used to take place in here... but I decided to move it =P
daemonloop();

cmdEnd();
dbEnd();

for( a = 0 ; a < options.interfaces ; a++ ) {
	io = &ioInterface[a];
	io->End();
}
svEnd();

return 1;
}

int file_exist (char *filename) {
	struct stat buffer;

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
		if( !( i == (int)(intptr_t)NULL ) ) {
			strcat(mkthisdir, "/");
			strcat(mkthisdir, split[i]);
			check = mkdir(mkthisdir,0755);
			if (!check) {
				if( options.verbose )
				printf("Directory \"%s\" created.\n", mkthisdir );
				syslog(LOG_INFO, "Directory \"%s\" created.\n", mkthisdir );
			} else if ( errno != 17 ) {
				if( options.verbose )
				printf("Error creating directory: \"%s\"\n", mkthisdir );
				syslog(LOG_ERR, "Error creating directory: \"%s\"\n", mkthisdir );
			}
		}
	}
}
free( split );
free( strCpy );


return;
}


static int sysconfig_handler(void* fconfig, const char* section, const char* name, const char* value) {
	configPtr pconfig = (configPtr)fconfig;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
if (MATCH("system", "port")) {
	pconfig->httpport = atoi(value);
} else if (MATCH("system", "name")) {
	pconfig->servername = strdup(value);
} else if (MATCH("system", "downfrom")) {
	pconfig->downfrom = strdup(value);
} else if (MATCH("system", "directory")) {
	pconfig->directory = strdup(value);
} else if (MATCH("system", "httpimages")) {
	pconfig->httpimages = strdup(value);
} else if (MATCH("system", "httpfiles")) {
	pconfig->httpfiles = strdup(value);
} else if (MATCH("system", "httpread")) {
	pconfig->httpread = strdup(value);
} else if (MATCH("system", "publicforum")) {
	pconfig->pubforum = strdup(value);
} else if (MATCH("system", "round")) {
	pconfig->round = atoi(value);
} else if (MATCH("system", "tick_time")) {
	pconfig->ticktime = atoi(value);
} else if (MATCH("system", "autoresume")) {
	pconfig->autoresume = strcmp(value,"false") ? true : false;
} else if (MATCH("system", "stockpile")) {
	pconfig->stockpile = atoi(value);
} else if (MATCH("system", "auto_endwar_afterticks")) {
	pconfig->warend = atoi(value);
} else if (MATCH("system", "auto_victory_afterticks")) {
	pconfig->victory = atoi(value);
} else if (MATCH("evmap", "port")) {
	pconfig->evmpport = atoi(value);
} else if (MATCH("evmap", "enabled")) {
        pconfig->evmpactv = strcmp(value,"false") ? true : false;
} else if (MATCH("syslog", "tag")) {
        pconfig->syslog_tagname = strdup(value);
} else if (MATCH("syslog", "facility")) {
        pconfig->syslog_facility = strdup(value);
} else {
        return 0;
}

return 1;
}

static int adminconfig_handler(void* fconfig, const char* section, const char* name, const char* value) {
	adminPtr pconfig = (adminPtr)fconfig;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
if (MATCH("admin", "name")) {
        pconfig->name = strdup(value);
} else if (MATCH("admin", "faction")) {
        pconfig->faction = strdup(value);
} else if (MATCH("admin", "forumtag")) {
        pconfig->forumtag = strdup(value);
} else if (MATCH("admin", "password")) {
        pconfig->password = strdup(value);
} else if (MATCH("admin", "level")) {
        pconfig->level = atoi(value);
} else if (MATCH("admin", "race")) {
        pconfig->race = atoi(value);
} else if (MATCH("admin_empire", "number")) {
        pconfig->empire = atoi(value);
} else if (MATCH("admin_empire", "name")) {
        pconfig->ename = strdup(value);
} else if (MATCH("admin_empire", "password")) {
        pconfig->epassword = strdup(value);
} else if (MATCH("admin_empire", "ommit_from_ranks")) {
	pconfig->rankommit = strcmp(value,"false") ? true : false;
} else {
        return 0;
}

return 1;
}

static int mysqlconfig_handler(void* fconfig, const char* section, const char* name, const char* value) {
	mySqlPtr pconfig = (mySqlPtr)fconfig;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
if (MATCH("mysql", "enabled")) {
        pconfig->active = strcmp(value,"false") ? true : false;
} else if (MATCH("mysql", "host")) {
        pconfig->host = strdup(value);
} else if (MATCH("mysql", "user")) {
        pconfig->user = strdup(value);
} else if (MATCH("mysql", "password")) {
        pconfig->password = strdup(value);
} else if (MATCH("mysql", "database")) {
        pconfig->database = strdup(value);
} else {
        return 0;
}

return 1;
}

static int tickconfig_handler(void* fconfig, const char* section, const char* name, const char* value) {
	tickPtr pconfig = (tickPtr)fconfig;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
if (MATCH("ticks", "status")) {
	pconfig->status = strcmp(value,"false") ? true : false;
} else if (MATCH("ticks", "number")) {
	pconfig->number = atoi(value);
} else if (MATCH("ticks", "next")) {
	pconfig->next = atoi(value);
} else if (MATCH("ticks", "debug_id")) {
	pconfig->debug_id = atoi(value);
} else if (MATCH("ticks", "debug_pass")) {
	pconfig->debug_pass = atoi(value);
} else {
        return 0;
}

return 1;
}

static int mapconfig_handler(void* fconfig, const char* section, const char* name, const char* value) {
	int a, i;
	char DIRCHECKER[256];
	mapcfgPtr pconfig = (mapcfgPtr)fconfig;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

for(a = 0; a < CMD_BONUS_NUMUSED; a++) {
	sprintf(DIRCHECKER,"%s",cmdBonusName[a]);
	for(i = 0; DIRCHECKER[i]; i++){
		DIRCHECKER[i] = tolower(DIRCHECKER[i]);
	}
	if (MATCH("mapgen", DIRCHECKER)) {
		pconfig->bonusnum += atoi(value);
		pconfig->bonusvar[a] = atoi(value);
	} else {
		continue;
	}
}

if (MATCH("mapgen", "size")) {
	pconfig->sizex = atoi(value);
//} else if (MATCH("mapgen", "height")) {
	pconfig->sizey = atoi(value); 
} else if (MATCH("mapgen", "systems")) {
	pconfig->systems = atoi(value);
} else if (MATCH("mapgen", "families")) {
	pconfig->families = atoi(value);
} else if (MATCH("mapgen", "members_perfamily")) {
	pconfig->fmembers = atoi(value);
} else if (MATCH("mapgen", "border")) {
	pconfig->border = atoi(value);
} else if (MATCH("mapgen", "anglevar")) {
	pconfig->anglevar = atoi(value);
} else if (MATCH("mapgen", "linknum")) {
	pconfig->num = atoi(value);
} else if (MATCH("mapgen", "radius")) {
	pconfig->radius = atoi(value);
} else if (MATCH("mapgen", "lenghtbase")) {
	pconfig->lenghtbase = atoi(value);
} else if (MATCH("mapgen", "lenghtvar")) {
	pconfig->lenghtvar = atoi(value);
} else {
        return 0;
}



return 1;
}

static int banconfig_handler(void* fconfig, const char* section, const char* name, const char* value) {
	int a;
	char DIRCHECKER[256];
	svbanPtr pconfig = (svbanPtr)fconfig;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
if (MATCH("banned_ips", "number")) {
	pconfig->number = atoi(value);
}
pconfig->ip = malloc(pconfig->number * sizeof(*pconfig->ip)); //Not really sure if this is needed, but I figger it's safer.
for(a = 0; a < banlist.number; a++) {
	sprintf(DIRCHECKER,"ip%d",(a+1));
	if (MATCH("banned_ips", DIRCHECKER)) {
		sprintf(pconfig->ip[a],"%s",value);
	} else {
		continue;
	}
}
free(pconfig->ip); //Not really sure if this is needed, but I figger it's safer.

return 1;
}

int loadconfig( char *file, int type ) {
	int logfac = LOG_SYSLOG;

if( type == CONFIG_SYSTEM ) {
	if (ini_parse(file, sysconfig_handler, &sysconfig) < 0) {
		syslog(LOG_ERR, "System Config Error: can not load \"%s\"\n", file );
        	printf("System Config Error: can not load \"%s\"\n", file);
		return 0;
	}
	if (ini_parse(file, mapconfig_handler, &mapcfg) < 0) {
		syslog(LOG_ERR, "Map Config Error: can not load \"%s\"\n", file );
        	printf("Map Config Error: can not load \"%s\"\n", file);
		return 0;
	}
	if (ini_parse(file, adminconfig_handler, &admincfg) < 0) {
		syslog(LOG_ERR, "Admin Config Error: can not load \"%s\"\n", file );
        	printf("Admin Config Error: can not load \"%s\"\n", file);
		return 0;
	}
	if (ini_parse(file, mysqlconfig_handler, &mysqlcfg) < 0) {
		syslog(LOG_ERR, "mySql Config Error: can not load \"%s\"\n", file );
        	printf("mySql Config Error: can not load \"%s\"\n", file);
		return 0;
	}
	if( sysconfig.evmpactv ) {
		options.interfaces = 2;
		options.port[PORT_EVMP] = sysconfig.evmpport;
	} else {
		options.interfaces = 1;
	}
	if( sysconfig.httpport )
		options.port[PORT_HTTP] = options.port[PORT_HTTP] ? options.port[PORT_HTTP] : sysconfig.httpport;

	if( strlen(sysconfig.syslog_facility) && strcmp(sysconfig.syslog_facility,"LOG_SYSLOG") ){
		closelog();
		if( strcmp(sysconfig.syslog_facility,"LOG_DAEMON") == 0 ) {
			logfac = LOG_DAEMON;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_USER") == 0 ) {
			logfac = LOG_USER;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL0") == 0 ) {
			logfac = LOG_LOCAL0;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL1") == 0 ) {
			logfac = LOG_LOCAL1;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL2") == 0 ) {
			logfac = LOG_LOCAL2;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL3") == 0 ) {
			logfac = LOG_LOCAL3;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL4") == 0 ) {
			logfac = LOG_LOCAL4;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL5") == 0 ) {
			logfac = LOG_LOCAL5;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL6") == 0 ) {
			logfac = LOG_LOCAL6;
		} else if( strcmp(sysconfig.syslog_facility,"LOG_LOCAL7") == 0 ) {
			logfac = LOG_LOCAL7;
		}
		openlog(sysconfig.syslog_tagname, LOG_CONS | LOG_PID | LOG_NDELAY, logfac);
	}
} else if( type == CONFIG_BANNED ){
	banlist.number = 0;
	if (ini_parse(file, banconfig_handler, &banlist ) < 0) {
		return 0;
	}
} else if( type == CONFIG_TICKS ) {
	if (ini_parse(file, tickconfig_handler, &ticks ) < 0) {
		return 0;
	}
}


return 1;
}

int savetickconfig() {
	char DIRCHECKER[256];
	FILE *file;

sprintf( DIRCHECKER, "%s/ticks.ini", sysconfig.directory );
file = fopen( DIRCHECKER, "r+" );
if(!file)
	file = fopen( DIRCHECKER, "w" );
if(file) {
	fprintf( file, "%s\n", ";Auto generated, there should be no need to edit this file!" );
	fprintf( file, "%s\n", "[ticks]" );
	fprintf( file, "status=%s\n", ( ticks.status ? "true" : "false" ) );
	fprintf( file, "number=%d\n", ticks.number );
	fprintf( file, "next=%d\n", ticks.next );
	fprintf( file, "debug_id=%d\n", ticks.debug_id );
	fprintf( file, "debug_pass=%d\n", ticks.debug_pass );
	fflush( file );
	fclose( file );
}


return 1;
}


int checkops(int argc, char **argv) {
	char DIRCHECKER[512] = {0};
	bool result;
	int index;
	int option;
     
opterr = 0;
result = false;
while( (option = getopt(argc, argv, "c:fm:p:qs:") ) != -1) {
	switch(option) {
		case 'c':
			sprintf(options.sysini, "%s", optarg);
			break;
		case 'f':
			options.mode = MODE_FORKED;
			options.verbose = false;
			break;
		case 'm':
			sprintf(options.mapini, "%s", optarg);
			break;
		case 'p':
			options.port[PORT_HTTP] = atoi(optarg);
			break;
		case 'q':
			options.verbose = false;
			break;
		case 's':
			sprintf(options.pipestring, "%s", optarg);
			break;


		case '?':
			if( (optopt == 'c') || (optopt == 'm') || (optopt == 'p') || (optopt == 's') ) {
				fprintf (stderr, "Option \'-%c\' requires an argument.\n", optopt);
				result = true;
			} else if( isprint(optopt) ) {
				fprintf (stderr, "Unknown option \'-%c\'.\n", optopt);
				result = true;
			} else {
				fprintf (stderr,"Unknown option character \'\\x%x\'.\n",optopt);
				result = true;
			}
	}
}

if( !( strlen(options.sysini) > 0 ) ) {
	if (getcwd(DIRCHECKER, sizeof(DIRCHECKER)) != NULL) {
		sprintf(options.sysini, "%s/evconfig.ini" ,DIRCHECKER);
	} else {
		perror("getcwd() error");
		result = true;
	}
}

for( index = optind; index < argc; index++ ) {
	printf ("Non-option argument: \'%s\'\n", argv[index]);
	result = true;
}

return result;
}

int main( int argc, char *argv[] ) {
	char DIRCHECKER[256];
	int num, test;

if( checkops(argc,argv) ) {
	printf ("Error: Invalid usage detected...\n");
	exit(true);
}

if( file_exist(options.sysini) == 0 ) {
	printf("File does not exist: \'%s\'\n",options.sysini);
	printf("Use \'-c /path/to/evconfig.ini\' to specify ini file to load (including the file name)\n");
	fflush(stdout);
	exit(true);
} else if( options.verbose ) {
	printf("Loading config from file: \'%s\'\n",options.sysini);
	fflush(stdout);
}

openlog(argv[0], LOG_CONS | LOG_PID | LOG_NDELAY, LOG_SYSLOG);

memset( &admincfg, 0, sizeof(adminDef) );
if( !(loadconfig(options.sysini,CONFIG_SYSTEM)) ) {
	printf("Error loading system config. Unable to start.\n");
	exit(true);
}

sprintf( DIRCHECKER, "%s/%s.%d.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
if ( file_exist(DIRCHECKER) ) {
	printf("%s\n","Pipe file detected, auto switching to client mode");
	goto CLIENT;
}

memset( &ticks, 0, sizeof(tickDef) );
sprintf( DIRCHECKER, "%s/ticks.ini", sysconfig.directory );
loadconfig(DIRCHECKER,CONFIG_TICKS);

loadconfig(options.sysini,CONFIG_BANNED);

dirstructurecheck(TMPDIR);

//check basic dir structure and create as needed.	
sprintf( DIRCHECKER, "%s/data", sysconfig.directory );
dirstructurecheck(DIRCHECKER);
sprintf( DIRCHECKER, "%s/users", sysconfig.directory );
dirstructurecheck(DIRCHECKER);
sprintf( DIRCHECKER, "%s/logs", sysconfig.directory );
dirstructurecheck(DIRCHECKER);
sprintf( DIRCHECKER, "%s/rankings", sysconfig.directory );
dirstructurecheck(DIRCHECKER);

//well its not really public yet now is it... <<<WORKNEEDED>>>
sprintf( DIRCHECKER, "%s/forum", sysconfig.directory );
dirstructurecheck(DIRCHECKER);

if( !( file_exist(sysconfig.httpread) ) ) {
	dirstructurecheck(sysconfig.httpread);
	if( !(file_exist(sysconfig.httpread) ) ) {
		printf("Directory creation failed... can not continue! =(\n");
		return 1;
	}
	printf("Doc base not found, fetching \"%s/read.tar.gz\" with wget ...", sysconfig.downfrom );
	fflush(stdout);
	syslog(LOG_INFO, "Doc base not found, fetching \"%s/read.tar.gz\" with wget.\n", sysconfig.downfrom );
	sprintf(DIRCHECKER,"wget -q \"%s/read.tar.gz\" -O %s/read.tar.gz", sysconfig.downfrom, TMPDIR);
	test = system(DIRCHECKER);
	printf(" %s!\n", test ? "Fail" : "Done");
	fflush(stdout);
	if(test)
	return 1;
	printf("Extracting files to: \"%s\" ...", sysconfig.httpread);
	fflush(stdout);
	syslog(LOG_INFO, "Extracting files to: \"%s\"\n", sysconfig.httpread);
	sprintf(DIRCHECKER,"tar -xzf %s/read.tar.gz -C %s", TMPDIR, sysconfig.httpread);
	test = system(DIRCHECKER);
	printf(" %s!\n", test ? "Fail" : "Done");
	fflush(stdout);
	if(test)
	return 1;
}
if( !( file_exist(sysconfig.httpfiles) ) ) {
	dirstructurecheck(sysconfig.httpfiles);
	if( !(file_exist(sysconfig.httpfiles) ) ) {
		printf("Directory creation failed... can not continue! =(\n");
		return 1;
	}
	printf("Doc base not found, fetching \"%s/files.tar.gz\" with wget ...", sysconfig.downfrom );
	fflush(stdout);
	syslog(LOG_INFO, "Doc base not found, fetching \"%s/files.tar.gz\" with wget.\n", sysconfig.downfrom );
	sprintf(DIRCHECKER,"wget -q \"%s/files.tar.gz\" -O %s/files.tar.gz", sysconfig.downfrom, TMPDIR);
	test = system(DIRCHECKER);
	printf(" %s!\n", test ? "Fail" : "Done");
	fflush(stdout);
	if(test)
	return 1;
	printf("Extracting files to: \"%s\" ...", sysconfig.httpfiles);
	fflush(stdout);
	syslog(LOG_INFO, "Extracting files to: \"%s\"\n", sysconfig.httpfiles);
	sprintf(DIRCHECKER,"tar -xzf %s/files.tar.gz -C %s", TMPDIR, sysconfig.httpfiles);
	test = system(DIRCHECKER);
	printf(" %s!\n", test ? "Fail" : "Done");
	fflush(stdout);
	if(test)
	return 1;
}
if( !( file_exist(sysconfig.httpimages) ) ) {
	dirstructurecheck(sysconfig.httpimages);
	if( !(file_exist(sysconfig.httpimages) ) ) {
		printf("Directory creation failed... can not continue! =(\n");
		return 1;
	}
	printf("Image base not found, fetching \"%s/images.tar.gz\" with wget ...", sysconfig.downfrom );
	fflush(stdout);
	syslog(LOG_INFO, "Image base not found, fetching \"%s/images.tar.gz\" with wget.\n", sysconfig.downfrom);
	sprintf(DIRCHECKER,"wget -q \"%s/images.tar.gz\" -O %s/images.tar.gz", sysconfig.downfrom, TMPDIR);
	test = system(DIRCHECKER);
	printf(" %s!\n", test ? "Fail" : "Done");
	fflush(stdout);
	if(test)
	return 1;
	printf("Extracting images to: \"%s\" ...", sysconfig.httpimages);
	fflush(stdout);
	syslog(LOG_INFO, "Extracting files to: \"%s\"\n", sysconfig.httpimages);
	sprintf(DIRCHECKER,"tar -xzf %s/images.tar.gz -C %s", TMPDIR, sysconfig.httpimages);
	test = system(DIRCHECKER);
	printf(" %s!\n", test ? "Fail" : "Done");
	fflush(stdout);
	if(test)
	return 1;
}

printf("\n");
sprintf( DIRCHECKER, "%s/data/map", sysconfig.directory );
if( !( file_exist(DIRCHECKER) ) ) {
	if( options.verbose )
	printf("No map detected... now generating...\n");
	syslog(LOG_INFO, "No map detected... now generating...\n");
	spawn_map();
}
//Begin deamonization and initate server loop.
if( !( daemon_init( ) ) ) {
	printf( "Can not load, oh this is bad...\n" );
	syslog(LOG_CRIT, "<<< !!!! CRITICAL ERROR !!!! >>>\n");
	return 1;
}
if( options.mode == MODE_FORKED ) {
	printf("%s\n", "Returning to shell, daemon has loaded in the background.");
	printf("\n");
}
return 0;

//OK, so we made it down here... that means we are a client and the pipe is active.
CLIENT:

sprintf(DIRCHECKER, "%s/%s.%d.client.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
mkfifo(DIRCHECKER, 0666);

if( strlen(options.pipestring) ) {
	svPipeSend(1, options.pipestring);
} else {
	printf("%s\n", "No command input detected... Unable to send blank command!" );
	printf("%s\n", "Use \'-s command\' if you want to send a command to the server via the pipe." );
	printf("\n");
	return true;
}

options.clientpipe = open(DIRCHECKER, O_RDONLY | O_NONBLOCK);
printf("\n");
while( file_exist(DIRCHECKER) ) {
	char buffer[1024] = {0};
	num = read(options.clientpipe, buffer, sizeof(buffer) );
	if( strcmp(buffer,"<<<END>>>") == false )
		break;
	if ( num > 0 ) {
                puts( buffer );
		fflush( stdout );
	}
}


cleanUp(options.clientpipe,0);
printf("\n");


return 0;
}



