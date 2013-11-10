#ifndef GLOBALINCLUDED
#include "global.h"
#endif

#include "iniparser.c"
#include "ircbot.c"

svConnectionPtr svConnectionList = 0;
fd_set svSelectRead;
fd_set svSelectWrite;
fd_set svSelectError;

optionsDef options = { MODE_DAEMON, { false, false }, 0, -1, -1, -1, true, "", "", "", "status" };

configDef sysconfig;
mySqlDef mysqlcfg;
adminDef admincfg;
mapcfgDef mapcfg;
tickDef ticks;
ircDef irccfg;

bool firstload = false;

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
		loghandle(LOG_ERR, errno, "Error %03d, creating listening socket", errno );
		continue;
	}
	a = 1;
	if( setsockopt( svListenSocket[b], SOL_SOCKET, SO_REUSEADDR, (char *)&a, sizeof(int) ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, setsockopt", errno );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	}
	sinInterface.sin_family = AF_INET;
	sinInterface.sin_addr.s_addr = htonl(INADDR_ANY);
	sinInterface.sin_port = htons( options.port[b] );
	if( bind( svListenSocket[b], (struct sockaddr *)&sinInterface, sizeof(struct sockaddr_in) ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, binding listening socket to port: %d", errno, options.port[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	}
	if( listen( svListenSocket[b], SOMAXCONN ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, listen on port: %d", errno, options.port[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	} 
	if( fcntl( svListenSocket[b], F_SETFL, O_NONBLOCK ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, setting non-blocking on port: %d", errno, options.port[b] );
		close( svListenSocket[b] );
		svListenSocket[b] = -1;
		continue;
	} else { c++; }
	loghandle(LOG_INFO, false, "Server awaiting connections on port: %d", options.port[b] );
}

if ( c == 0 ) {
	loghandle(LOG_CRIT, false, "Server Binding failed, ports are not avalible/allowed!!" );
//Empty Return to indicate no ports avaliable, and server can not iniate.
	return 0;
}


//Return to indicate all ports avaliable, and server can iniate.
return 1;
}

void cleanUp(int type) {
	char DIRCHECKER[256];


if( type ) {
	close(options.serverpipe);
	sprintf( DIRCHECKER, "%s/%s.%d.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
	unlink(DIRCHECKER);
	loghandle(LOG_INFO, false, "%s", "Server has been Completly shutdown!" );
	syslog(LOG_INFO, "%s", "<<<<<BREAKER-FOR-NEW-SERVER-INSTANCE>>>>>" ); //Don't really need this, but meh... why not!
	closelog();
} else {
	close(options.clientpipe);
	sprintf( DIRCHECKER, "%s/%s.%d.client.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
	unlink(DIRCHECKER);
}

return;
}

void svEnd() {
	int a;
	svConnectionPtr cnt, next;

loghandle(LOG_INFO, false, "%s", "Shutdown called, begin deamon breakdown..." );

for( cnt = svConnectionList ; cnt ; cnt = next ) {
	next = cnt->next;
	svFree( cnt );
}
for( a = 0 ; a < options.interfaces ; a++ ) {
	if( svListenSocket[a] == -1 )
		continue;
	//Error 009 means not connected, so we ignore that "error" now... afterall we are closing connections here.
   	if( ( shutdown( options.port[a], 2 ) == -1 ) && ( errno != 9 ) ) {
		loghandle(LOG_ERR, errno, "Error %03d, unable to shutdown listening socket: %d", errno, options.port[a] );
	}
 	if( close( svListenSocket[a] ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, closing socket: %d", errno, options.port[a] );
	} else {
		loghandle(LOG_INFO, false, "Server released port: %d", options.port[a] );
		}

   	}

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
		loghandle(LOG_ERR, errno, "Error %03d, failed to accept a connection %s", errno, inet_ntoa(sockaddr.sin_addr) );
		continue;
		loghandle(LOG_CRIT, false, "This is a critical problem... should we really keep going past this?? - We do anways." );
	}
	if( socket >= FD_SETSIZE ) {
		loghandle(LOG_ERR, false, "Error, socket >= FD_SETSIZE, %d", socket );
		if( close( socket ) == -1 ) {
			loghandle(LOG_ERR, errno, "Error %03d, unable to close socket", errno );
		}
	continue;
}
#if SERVER_REPORT_CONNECT == 1
else
loghandle(LOG_INFO, false, "Accepting connection from %s:%d>%d", inet_ntoa( sockaddr.sin_addr ), ntohs( sockaddr.sin_port ), socket );
#endif

	if( !( cnt = malloc( sizeof(svConnectionDef) ) ) ) {
		loghandle(LOG_ERR, false, "%s", "ERROR, not enough memory to create a connection structure" );
		if( close( socket ) == -1 ) {
			loghandle(LOG_ERR, errno, "Error %03d, unable to close socket", errno );
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
		loghandle(LOG_ERR, errno, "Error %03d, setsockopt", errno );
	}
#endif

	if( fcntl( socket, F_SETFL, O_NONBLOCK ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, setting a socket to non-blocking", errno );
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

if( ( select( rmax+1, &svSelectRead, &svSelectWrite, &svSelectError, &to ) < 0 ) && (sysconfig.shutdown == false) ) {
	loghandle(LOG_ERR, errno, "Error %03d, select()", errno );
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
		loghandle(LOG_ERR, errno, "%s>%d Timeout : %d", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
		#endif
		io->inError( cnt, 0 );
		cnt->flags |= SV_FLAGS_TIMEOUT;
	}
	if( time - cnt->time >= io->hardtimeout ) {
		#if SERVER_REPORT_ERROR == 1
		loghandle(LOG_ERR, errno, "%s>%d Hard timeout : %d", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
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
		loghandle(LOG_ERR, errno, "Connection to %s>%d died : %d", inet_ntoa( (cnt->sockaddr).sin_addr ), cnt->socket, errno );
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
loghandle(LOG_INFO, false, "Shuting down connection to %s:%d>%d", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
#endif
shutdown( cnt->socket, 1 );

return;
}

void svClose( svConnectionPtr cnt ) {

#if SERVER_REPORT_CLOSE == 1
loghandle(LOG_INFO, false, "Closed connection to %s:%d>%d", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ), cnt->socket);
#endif
if( close( cnt->socket ) == -1 ) {
	loghandle(LOG_ERR, errno, "Error %03d, closing socket", errno);
}
cnt->socket = -1;

return;
}

int svFree( svConnectionPtr cnt ) {

svConnectionPtr next;

if( cnt->socket != -1 )
	svClose( cnt );
#if SERVER_REPORT_CLOSE == 1
loghandle(LOG_INFO, false, "Freed connection to %s:%d", inet_ntoa( cnt->sockaddr.sin_addr ), ntohs( cnt->sockaddr.sin_port ) );
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
	loghandle(LOG_ERR, errno, "Error %03d, add buffer malloc", errno );
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
		loghandle(LOG_ERR, errno, "Error %d, send flush", errno);
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
 
if( (signal == SIGNALS_SIGTERM ) || (signal == SIGNALS_SIGINT) ){
	if( options.verbose ) {
		printf("\n");
		fflush(stdout);
	}
	loghandle(LOG_INFO, false, "%s Recived; handleing gracefully =)", cmdSignalNames[signal]);  
  	sysconfig.shutdown = true;
	return;
}
 /* if(signal == SIGUSR2)
  {
  	//Free memory db and reload it to have a new member in :P
  	loghandle(LOG_INFO, false, "%s", "Ask a dbinit");
  	dbEnd();
  	dbInit();
  	return;
  }*/

iohttpDataPtr iohttp;
loghandle(LOG_CRIT, false, "ERROR, signal \'%s\'", cmdSignalNames[signal]);
loghandle(LOG_CRIT, false, "cnt : %d", (int)(intptr_t)svDebugConnection);
loghandle(LOG_CRIT, false, "tick pass : %d", ticks.debug_pass);
loghandle(LOG_CRIT, false, "tick id : %d", ticks.debug_id);

if( svDebugConnection ) {

	iohttp = svDebugConnection->iodata;

	loghandle(LOG_CRIT, false, "iohttp : %d", (int)(intptr_t)iohttp );
	loghandle(LOG_CRIT, false, "iohttp->path : %s", iohttp->path );
	loghandle(LOG_CRIT, false, "iottp content lenth: %d", iohttp->content_length );
	loghandle(LOG_CRIT, false, "iohttp->query_string : %s", iohttp->query_string );
	loghandle(LOG_CRIT, false, "iohttp->cookie : %s", iohttp->cookie );
	loghandle(LOG_CRIT, false, "iohttp->referer : %s", iohttp->referer );
	loghandle(LOG_CRIT, false, "iohttp->user_agent : %s", iohttp->user_agent );
	loghandle(LOG_CRIT, false, "iohttp->content : <<START>> %s <<END>>", iohttp->content );

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
			loghandle(LOG_CRIT, errno, "Error %d, send", errno);
			return;
		}
		if( a != size ) {
			svDebugConnection->sendflushpos += a;
			break;
		}
		svDebugConnection->sendflushpos = 0;
	}
}


if( irccfg.bot ) {
	ircbot_send("NOTICE %s :Server recived \'%s\' signal -- Shutdown Iniated!", irccfg.channel, cmdSignalNames[signal]);
	ircbot_send("QUIT");
	if( close( options.botconn ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, closing ircbot socket", errno);
	}

}

sysconfig.shutdown = true;
cleanUp(1);
cleanUp(0);

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
	char buffer[128] = {0}, bufferstrip[128] = {0};

if(pipefileid < 0 )
return;

num = read(pipefileid, buffer, sizeof(buffer));
buffer[num] = '\0';
stop = 0;
sprintf(bufferstrip,"%s",trimwhitespace(buffer));
sprintf(buffer,"%s",bufferstrip);

if ( ( num > 0 ) && strlen(buffer) ) {
	if( !(strcmp(buffer,"stop") ) ) {
		sysconfig.shutdown = true;
		stop = 1;
	} else if( !( strncmp(buffer, "bot", 3) ) ) {
		if( !ircbot_command(buffer) ) {
			loghandle(LOG_INFO, false, "Bot subfunction reported error with command: \"%s\"", buffer);
			svPipeSend(0, "Bot subfunction reported error with command: \"%s\"\n",buffer );
		}
	} else {
		loghandle(LOG_INFO, false, "Piping Error Unrecognized command size \"%d\" line \"%s\"", num, buffer);
	}
}

if( stop ) {
	svPipeSend(0,"Server is shutting down as requested..");
	if( options.verbose )
	loghandle(LOG_INFO, false, "%s", "Shutdown command recived from Pipe."); 
}

if ( num > 0 ) {
	svPipeSend(0,"<<<END>>>");
}

return;
}

//Respond to client, let them know we have the command.
int svPipeSend(int pipedirection, char *message, ...){
	int num;
	va_list ap;
	FILE *pipefile;
	char DIRCHECKER[256];
	char formatuffer[SERVER_RECV_BUFSIZE] = {0};

va_start(ap, message);
vsnprintf(formatuffer, SERVER_RECV_BUFSIZE, message, ap);
va_end(ap);

sprintf( DIRCHECKER, "%s/%s.%d.%s", TMPDIR, options.pipefile, options.port[PORT_HTTP], ( pipedirection ? "pipe" : "client.pipe" ) );
if( file_exist(DIRCHECKER) && strlen(formatuffer) ) {
	if( ( pipefile = fopen(DIRCHECKER, "w") ) < 0) {
		loghandle(LOG_ERR, errno, "Piping Error: unable to open pipe for write: %s", DIRCHECKER );
		return 0;
	}
	if( ( num = fprintf(pipefile, "%s\r\n", formatuffer) ) < 0) {
		loghandle(LOG_ERR, errno, "Piping Responce Error: unable to write to pipe: %s", DIRCHECKER );
		return 0;
	}
	fflush(pipefile);
	fclose(pipefile);
} else {
	loghandle(LOG_ERR, false, "%s", "Piping Error: message to send but no pipe avaliable" );
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
while( sysconfig.shutdown == false ) {
	if( irccfg.bot ) {
		ircbot_scan();
	}
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
	
	if( ( sysconfig.autostop ) && ( timediff(sysconfig.stop) < 1 ) ) {
		ticks.status = false;
	} else if( ( sysconfig.autostart ) && ( timediff(sysconfig.start) < 1 ) ) {
		ticks.status = true;
	}


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

	if( options.verbose )
		fflush(stdout);
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

loghandle(LOG_INFO, false, "%s", "Server process iniating...");

if( options.mode == MODE_FORKED ) {
pid = fork();
if(pid < 0) {
	loghandle(LOG_ERR, errno, "Forking Error: %d", errno);
	return 0;
} else if(pid > 0) {
	return 1; // So we forked it, time to return and wait for results on a client pipe.
}

loghandle(LOG_INFO, false, "Begining initiation of %s daemon...", sysconfig.servername);

// First, start a new session
if((sid = setsid()) < 0) {
	loghandle(LOG_ERR, errno, "%s", "setsid has failed, unable to fork into daemon");
	return 0;
}

// Next, make /tmp/evcore the current directory. -- I do this, just because I can. (It doesn't matter it changes a lot latter, I need to fix that still).
if((chdir("/tmp/evcore")) < 0) {
	loghandle(LOG_ERR, errno, "%s", "chdir has failed, unable to fork into daemon");
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
signal( SIGUSR1, &svSignal);
signal( SIGUSR2, &svSignal);
	
srand( time(NULL) ); //Random Init
	
if( !( svInit() ) ) {
	loghandle(LOG_CRIT, false, "%s", "Server Core Initation Failed, now exiting..." );
	return 0;
}

if( !( dbInit("Database initialisation failed, exiting\n") ) ) {
	loghandle(LOG_CRIT, false, "%s", "Server Database Initation Failed, now exiting..." );
	return 0;
}
for( a = 0 ; a < options.interfaces ; a++ ) {
	io = &ioInterface[a];
	io->Init();
}

if( !( cmdInit() ) )  {
	loghandle(LOG_CRIT, false, "%s", "Server Command Initation Failed, now exiting..." );
	return 0;
}
sprintf( DIRCHECKER, "%s/data", sysconfig.directory );  
if( chdir( DIRCHECKER ) == -1 ) {
	if( options.verbose )
	loghandle(LOG_ERR, errno, "Change into Database Dir \"%s\" Failed, exiting", DIRCHECKER );
	return 0;
}

dbMapRetrieveMain( binfo );
if( ( binfo[MAP_ARTITIMER] == -1 ) || !( (binfo[MAP_ARTITIMER] - ticks.number) <= 0 ) ) {
	if( ( sysconfig.autostart == 1 ) && ( !ticks.status ) && (ticks.number) )
		ticks.status = 1;
}

//add local pipe, for basic commands from shell

sprintf( DIRCHECKER, "%s/%s.%d.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
	if( mkfifo(DIRCHECKER, 0666) < 0 ) {
	loghandle(LOG_ERR, errno, "Error creating pipe: %sn", DIRCHECKER );
	options.serverpipe = -1;
} else {
	options.serverpipe = open(DIRCHECKER, O_RDONLY | O_NONBLOCK);
}

loghandle(LOG_INFO, false, "%s", "All Checks passed, begining server loop..." ); 

//Now create the loop, this used to take place in here... but I decided to move it =P
if( irccfg.bot ) {
	ircbot_connect();
}

daemonloop();

if( irccfg.bot ) {
	ircbot_send("NOTICE %s :Server Shutdown has been iniated!", irccfg.channel);
	ircbot_send("QUIT");
	if( close( options.botconn ) == -1 ) {
		loghandle(LOG_ERR, errno, "Error %03d, closing ircbot socket", errno);
	}

}

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
				loghandle(LOG_INFO, false, "Directory \"%s\" created.", mkthisdir );
			} else if ( errno != 17 ) {
				loghandle(LOG_ERR, errno, "Error creating directory: \"%s\"", mkthisdir );
			}
		}
	}
}
free( split );
free( strCpy );


return;
}

int loadconfig( char *file, int type ) {
	int a, i;
	int logfac = LOG_SYSLOG;
	char DIRCHECKER[256];
	inikey ini;

if( firstload ) {
	ini = dictionary_new(0);
} else {
	ini = iniparser_load(file);
	if( ini == NULL ) {
		fprintf(stderr, "cannot parse file: %s\n", file);
		return -1;
	}
}

if( iniparser_find_entry(ini,"NEED_TO_DELETE_ME") ) {
	loghandle(LOG_CRIT, false, "A default, non-usable version of the evconfig.ini has been detected: \'%s\'",file);
	loghandle(LOG_CRIT, false, "%s", "You must edit this file before the game server is able to run correctly!");
	sysconfig.shutdown = true;
	return -1;
}

if( sysconfig.shutdown )
	return -1;

if( type == CONFIG_SYSTEM ) {
//Enter new scaner.
	sysconfig.syslog_tagname = strdup( iniparser_getstring(ini, "syslog:tag", "EVServer" ) );
	sysconfig.syslog_facility = strdup( iniparser_getstring(ini, "syslog:facility", "LOG_LOCAL6" ) );

	sysconfig.servername = strdup( iniparser_getstring(ini, "system:name", "NEctroverse") );
	sysconfig.directory = strdup( iniparser_getstring(ini, "system:directory", "/tmp/evcore/data") );
	sysconfig.downfrom = strdup( iniparser_getstring(ini, "system:downfrom", "http://www.sknill.com/evbasic") );
	sysconfig.httpimages = strdup( iniparser_getstring(ini, "system:httpimages", "/tmp/evcore/html/images") );
	sysconfig.httpfiles = strdup( iniparser_getstring(ini, "system:httpfiles", "/tmp/evcore/html/files") );
	sysconfig.httpread = strdup( iniparser_getstring(ini, "system:httpread", "/tmp/evcore/html/read") );
	sysconfig.pubforum = strdup( iniparser_getstring(ini, "system:pubforum", sysconfig.directory ) );

	sysconfig.httpport = iniparser_getint(ini, "system:port", 9990);
	sysconfig.evmpactv = iniparser_getboolean(ini, "evmap:enable", false);
	sysconfig.evmpport = iniparser_getint(ini, "evmap:port", 9991);

	sysconfig.stockpile = iniparser_getint(ini, "system:stockpile", 0);
	sysconfig.warend = iniparser_getint(ini, "system:auto_victory_afterticks", 0);
	sysconfig.victory = iniparser_getint(ini, "system:auto_endwar_afterticks", 0);
	sysconfig.ticktime = iniparser_getint(ini, "system:tick_time", 0);

	sysconfig.round = iniparser_getint(ini, "system:round", 0); //FIXME -- Needs moving to a different function set.

	sysconfig.autostart = iniparser_getboolean(ini, "auto_start:enable", false);
	sysconfig.start.tm_sec = iniparser_getint(ini, "auto_start:second", -1);
	sysconfig.start.tm_min = iniparser_getint(ini, "auto_start:minute", -1);
	sysconfig.start.tm_mday = iniparser_getint(ini, "auto_start:day", -1);
	sysconfig.start.tm_mon = (( iniparser_getint(ini, "auto_start:month", -1) ) - 1);
	sysconfig.start.tm_year = (( iniparser_getint(ini, "auto_start:year", -1) ) + 100);

	sysconfig.autostop = iniparser_getboolean(ini, "auto_stop:enable", false);
	sysconfig.stop.tm_sec = iniparser_getint(ini, "auto_stop:second", -1);
	sysconfig.stop.tm_min = iniparser_getint(ini, "auto_stop:minute", -1);
	sysconfig.stop.tm_mday = iniparser_getint(ini, "auto_stop:day", -1);
	sysconfig.stop.tm_mon = (( iniparser_getint(ini, "auto_stop:month", -1) ) - 1);
	sysconfig.stop.tm_year = (( iniparser_getint(ini, "auto_stop:year", -1) ) + 100);

	mysqlcfg.enable = iniparser_getboolean(ini, "mysql:enable", false);
	mysqlcfg.host = strdup( iniparser_getstring(ini, "mysql:host", "localhost") );
	mysqlcfg.port = iniparser_getint(ini, "mysql:port", 3306);
	mysqlcfg.user = strdup( iniparser_getstring(ini, "mysql:user", "localhost") );
	mysqlcfg.password = iniparser_getstring(ini, "mysql:password", NULL) ? strdup( iniparser_getstring(ini, "mysql:password", "") ) : NULL;
	mysqlcfg.database = strdup( iniparser_getstring(ini, "mysql:database", "evgame") );


	admincfg.numadmins = iniparser_getint(ini, "admin:number", 0);
	if( admincfg.numadmins > 0 ) {
		admincfg.race = malloc( admincfg.numadmins * sizeof(*admincfg.race) );
		admincfg.level = malloc( admincfg.numadmins * sizeof(*admincfg.level) );
		admincfg.name = malloc( admincfg.numadmins * sizeof(*admincfg.name) );
		admincfg.password = malloc( admincfg.numadmins * sizeof(*admincfg.password) );
		admincfg.faction = malloc( admincfg.numadmins * sizeof(*admincfg.faction) );
		admincfg.forumtag = malloc( admincfg.numadmins * sizeof(*admincfg.forumtag) );
	}
	for( a = 0; a < admincfg.numadmins ; a++ ){
		sprintf(DIRCHECKER,"admin:race%d",(a+1));
		admincfg.race[a] = iniparser_getint(ini, DIRCHECKER, 0);
		sprintf(DIRCHECKER,"admin:level%d",(a+1));
		admincfg.level[a] = iniparser_getint(ini, DIRCHECKER, 0);
		sprintf(DIRCHECKER,"admin:name%d",(a+1));
		admincfg.name[a] = iniparser_getstring(ini, DIRCHECKER, NULL ) ? strdup( iniparser_getstring(ini, DIRCHECKER, "") ) : NULL;
		sprintf(DIRCHECKER,"admin:password%d",(a+1));
		admincfg.password[a] = iniparser_getstring(ini, DIRCHECKER, NULL ) ? strdup( iniparser_getstring(ini, DIRCHECKER, "") ) : NULL;
		sprintf(DIRCHECKER,"admin:faction%d",(a+1));
		admincfg.faction[a] = iniparser_getstring(ini, DIRCHECKER, NULL ) ? strdup( iniparser_getstring(ini, DIRCHECKER, "") ) : NULL;
		sprintf(DIRCHECKER,"admin:forumtag%d",(a+1));
		admincfg.forumtag[a] = strdup( iniparser_getstring(ini, DIRCHECKER, "Helper") );
	}
	admincfg.empire = iniparser_getint(ini, "admin_empire:number", 0);
	admincfg.ename = strdup( iniparser_getstring(ini, "admin_empire:name", "Administration") );
	admincfg.epassword = strdup( iniparser_getstring(ini, "admin_empire:password", "password") );
	admincfg.rankommit = iniparser_getboolean(ini, "admin_empire:ommit_from_ranks", false);

	mapcfg.sizex = iniparser_getint(ini, "map:sizex", 100);
	mapcfg.sizey = iniparser_getint(ini, "map:sizey", mapcfg.sizex);

	mapcfg.systems = iniparser_getint(ini, "map:systems", 250);
	mapcfg.families = iniparser_getint(ini, "map:families", 10);
	mapcfg.fmembers = iniparser_getint(ini, "map:members_perfamily", 10);

	mapcfg.border = iniparser_getint(ini, "map:border", 20);
	mapcfg.anglevar = iniparser_getdouble(ini, "map:anglevar", 1024.0);

	mapcfg.linknum = iniparser_getdouble(ini, "map:linknum", 60);
	mapcfg.linkradius = iniparser_getdouble(ini, "map:linkradius", 8.0);
	mapcfg.lenghtbase = iniparser_getint(ini, "map:lenghtbase", 2);
	mapcfg.lenghtvar = iniparser_getint(ini, "map:lenghtvar", 24);

	mapcfg.bonusnum = 0;
	for(a = 0; a < CMD_BONUS_NUMUSED; a++) {
		sprintf(DIRCHECKER,"map:%s",cmdBonusName[a]);
		for(i = 0; DIRCHECKER[i]; i++){
			DIRCHECKER[i] = tolower(DIRCHECKER[i]);
		}
		mapcfg.bonusvar[a] = iniparser_getint(ini, DIRCHECKER, 0);
		mapcfg.bonusnum += mapcfg.bonusvar[a];
	}


//End config scanning... handle variables.
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
	/*if( banlist.ini )
		iniparser_freedict(banlist.ini);
	banlist.ini = iniparser_load(file);*/
	if( ( banlist.ip ) && ( banlist.number ) ) {
		free( banlist.ip );
	}
	banlist.number = iniparser_getint(/*banlist.*/ini, "banned_ips:number", 0);
	if( banlist.number > 0 ) {
		banlist.ip = malloc( banlist.number * sizeof(*banlist.ip));
	}
	for(a = 0; a < banlist.number; a++) {
		sprintf(DIRCHECKER,"banned_ips:ip%d",(a+1));
		banlist.ip[a] = strdup(iniparser_getstring(/*banlist.*/ini, DIRCHECKER, "0.0.0.0"));
	}
} else if( type == CONFIG_TICKS ) {
	if( ticks.ini )
		iniparser_freedict(ticks.ini);
	ticks.ini = iniparser_load(file);
	ticks.status = iniparser_getboolean(ticks.ini, "ticks:status", false);
	ticks.number = iniparser_getint(ticks.ini, "ticks:number", 0);
	ticks.round = iniparser_getint(ticks.ini, "ticks:round", ( sysconfig.round ? sysconfig.round : 0 ) );
	ticks.speed = iniparser_getint(ticks.ini, "ticks:speed", ( sysconfig.ticktime ? sysconfig.ticktime : 3600 ) );
	ticks.next = iniparser_getint(ticks.ini, "ticks:next", 0);
} else if( type == CONFIG_IRC ) {
	/*if( irccfg.ini )
		iniparser_freedict(irccfg.ini);
	irccfg.ini = iniparser_load(file);*/
	irccfg.host = strdup( iniparser_getstring(/*irccfg.*/ini, "irc:host", "irc.freenode.net") );
	irccfg.port = strdup( iniparser_getstring(/*irccfg.*/ini, "irc:port", "6667") );
	strcpy(DIRCHECKER,"#");
	strcat(DIRCHECKER,strdup( iniparser_getstring(/*irccfg.*/ini, "irc:channel", "ectroverse") ) );
	irccfg.channel = strdup(DIRCHECKER);
	irccfg.botnick = strdup( iniparser_getstring(/*irccfg.*/ini, "irc:bot_nick", "EVBot") );
	irccfg.botpass = strdup( iniparser_getstring(/*irccfg.*/ini, "irc:bot_pass", "botpass") );
	irccfg.bot = iniparser_getboolean(/*irccfg.*/ini, "irc:bot_enable", false);
	irccfg.announcetick = iniparser_getboolean(/*irccfg.*/ini, "irc:bot_announcetick", false);
}

if( firstload ) {
	FILE *dumpfile;
	if( !( iniparser_find_entry(ini,"system") ) ){
		iniparser_set(ini,"system",NULL);
		iniparser_set(ini,"system:name",sysconfig.servername);
		iniparser_set(ini,"system:directory",sysconfig.directory);
		iniparser_set(ini,"system:downfrom",sysconfig.downfrom);
		iniparser_set(ini,"system:httpimages",sysconfig.httpimages);
		iniparser_set(ini,"system:httpfiles",sysconfig.httpfiles);
		iniparser_set(ini,"system:httpread",sysconfig.httpread);
		iniparser_set(ini,"system:pubforum",sysconfig.pubforum);
		iniparser_set(ini,"system:port",itoa(sysconfig.httpport));
		iniparser_set(ini,"system:stockpile","14");
		iniparser_set(ini,"system:auto_victory_afterticks","52");
		iniparser_set(ini,"system:auto_endwar_afterticks","26");
		iniparser_set(ini,"system:tick_time","3600");
		iniparser_set(ini,"system:round","0");
	}
	if( !( iniparser_find_entry(ini,"evmap") ) ){
		iniparser_set(ini,"evmap",NULL);
		iniparser_set(ini,"evmap:enable", sysconfig.evmpactv ? "true" : "false" );
		iniparser_set(ini,"evmap:port", itoa(sysconfig.evmpport) );
	}
	if( !( iniparser_find_entry(ini,"syslog") ) ){
		iniparser_set(ini,"syslog",NULL);
		iniparser_set(ini,"syslog:tag", sysconfig.syslog_tagname);
		iniparser_set(ini,"syslog:facility", sysconfig.syslog_facility);
	}
	if( !( iniparser_find_entry(ini,"irc") ) ){
		iniparser_set(ini,"irc",NULL);
		iniparser_set(ini,"irc:host", "irc.freenode.net" );
		iniparser_set(ini,"irc:port", "6667" );
		iniparser_set(ini,"irc:channel","ectroverse");
		iniparser_set(ini,"irc:bot_nick", "EVBot" );
		iniparser_set(ini,"irc:bot_pass", "botpass" );
		iniparser_set(ini,"irc:bot_enable", "false" );
		iniparser_set(ini,"irc:bot_announcetick", "false" );
	}
	if( !( iniparser_find_entry(ini,"admin") ) ){
		iniparser_set(ini,"admin",NULL);
		iniparser_set(ini,"admin:number", "2" );
		iniparser_set(ini,"admin:name1", "admin" );
		iniparser_set(ini,"admin:password1","password");
		iniparser_set(ini,"admin:faction1", "Admins Faction" );
		iniparser_set(ini,"admin:forumtag1", "<img src=\"images/admin.gif\">" );
		iniparser_set(ini,"admin:level1", "3" );
		iniparser_set(ini,"admin:race1", "2" );
		iniparser_set(ini,"admin:name2", "help" );
		iniparser_set(ini,"admin:password2","password");
		iniparser_set(ini,"admin:faction2", "Admins Helper" );
		iniparser_set(ini,"admin:forumtag2", "Helper" );
	}
	if( !( iniparser_find_entry(ini,"admin_empire") ) ){
		iniparser_set(ini,"admin_empire",NULL);
		iniparser_set(ini,"admin_empire:number", itoa(admincfg.empire) );
		iniparser_set(ini,"admin_empire:name", admincfg.ename );
		iniparser_set(ini,"admin_empire:password", admincfg.epassword );
		iniparser_set(ini,"admin_empire:ommit_from_rank", admincfg.rankommit ? "true" : "false" );
	}
	if( !( iniparser_find_entry(ini,"map") ) ){
		iniparser_set(ini,"map",NULL);
		iniparser_set(ini,"map:sizex", itoa(mapcfg.sizex) );
		iniparser_set(ini,"map:systems", itoa(mapcfg.systems) );
		iniparser_set(ini,"map:families", itoa(mapcfg.families) );
		iniparser_set(ini,"map:members_perfamily", itoa(mapcfg.fmembers) );
		iniparser_set(ini,"map:border", itoa(mapcfg.border) );
		iniparser_set(ini,"map:anglevar", itoa(mapcfg.anglevar) );
		iniparser_set(ini,"map:linknum", itoa(mapcfg.linknum) );
		iniparser_set(ini,"map:linkradius", itoa(mapcfg.linkradius) );
		iniparser_set(ini,"map:lenghtbase", itoa(mapcfg.lenghtbase) );
		iniparser_set(ini,"map:lenghtvar", itoa(mapcfg.lenghtvar) );
		for(a = 0; a < CMD_BONUS_NUMUSED; a++) {
			sprintf(DIRCHECKER,"map:%s",cmdBonusName[a]);
			for(i = 0; DIRCHECKER[i]; i++){
				DIRCHECKER[i] = tolower(DIRCHECKER[i]);
			}
			iniparser_set(ini,DIRCHECKER, itoa( (rand() % 35) ) );
		}
	}
	if( !( iniparser_find_entry(ini,"banned_ips") ) ){
		iniparser_set(ini,"banned_ips",NULL);
		iniparser_set(ini,"banned_ips:number", "3" );
		iniparser_set(ini,"banned_ips:ip1", "10.0.0.*" );
		iniparser_set(ini,"banned_ips:ip2","192.168.0.*");
		iniparser_set(ini,"banned_ips:ip3", "127.0.0.1" );
	}
	iniparser_set(ini,"NEED_TO_DELETE_ME",NULL);
	dumpfile = fopen(file, "w");
	if(file) {
		fprintf( dumpfile, "%s\n", "; NEctroverse Alpha Config file" );
		fprintf( dumpfile, "%s\n", "; Why did I change from config.h to config.ini ??" );
		fprintf( dumpfile, "%s\n", "; Simple, changes here can be implemented without a rebuild!" );
		fprintf( dumpfile, "%s\n", "; Just change and restart. Simple =D" );
		fprintf( dumpfile, "%s\n", "; -- Necro" );
		fprintf( dumpfile, "\n" );
		iniparser_dump_ini(ini,dumpfile);
		fprintf( dumpfile, "%s\n", ";Auto generated, You will need to delete the DELETE settings before this file will load.!" );
		fprintf( dumpfile, "%s\n", ";This file was automaticly generated as no ini file was present!" );
		fflush( dumpfile );
		fclose( dumpfile );
		loghandle(LOG_CRIT, false, "A default, non-usable version of the evconfig.ini has been dumped to: \'%s\'",file);
		loghandle(LOG_CRIT, false, "%s", "You must edit this file according to your needs before you run the game server!");
	}
	sysconfig.shutdown = true;
}

iniparser_freedict(ini);

return 1;
}

char *itoa(int i){
    char const digit[] = "0123456789";
    char buffer[1024];
    char *p = buffer;

if(i<0){
	*p++ = '-';
	i = -1;
}

int shifter = i;
do{
	++p;
	shifter = shifter/10;
}while(shifter);
*p = '\0';
do{
	*--p = digit[i%10];
	i = i/10;
}while(i);
    
return strdup(p);
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
	iniparser_set(ticks.ini,"ticks:status",ticks.status ? "true" : "false");
	iniparser_set(ticks.ini,"ticks:number",itoa(ticks.number));
	iniparser_set(ticks.ini,"ticks:round",itoa(ticks.round));
	iniparser_set(ticks.ini,"ticks:speed",itoa(ticks.speed));
	iniparser_set(ticks.ini,"ticks:next",itoa(ticks.next));
	iniparser_set(ticks.ini,"ticks:debug_id",itoa(ticks.debug_id));
	iniparser_set(ticks.ini,"ticks:debug_pass",itoa(ticks.debug_pass));
	iniparser_dump_ini(ticks.ini,file);
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
} else {
	char *pointer = NULL;
	pointer = strrchr( strdup(options.sysini), '/' );
	if( !( pointer ) ) {
		char *file = strdup(options.sysini);
		if (getcwd(DIRCHECKER, sizeof(DIRCHECKER)) != NULL) {
			sprintf(options.sysini, "%s/%s" ,DIRCHECKER, file);
		} else {
			perror("getcwd() error");
			result = true;
		}
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
	//printf("The above file will be created with a default set, please review the file and reload.\n");
	printf("Use \'-c /path/to/evconfig.ini\' to specify ini file to load (including the file name)\n");
	fflush(stdout);
	firstload = true;
	//exit(true);
} else if( options.verbose ) {
	printf("Loading config from file: \'%s\'\n",options.sysini);
	fflush(stdout);
}

memset( &sysconfig, 0, sizeof(configDef) );

openlog(argv[0], LOG_CONS | LOG_PID | LOG_NDELAY, LOG_SYSLOG);

memset( &admincfg, 0, sizeof(adminDef) );
if( !(loadconfig(options.sysini,CONFIG_SYSTEM)) ) {
	loghandle(LOG_CRIT, false, "%s", "Unable to load system config, unable to continue.");
	exit(true);
}

if( ( firstload ) || ( sysconfig.shutdown ) )
	goto BAILOUT;
	
sprintf( DIRCHECKER, "%s/%s.%d.pipe", TMPDIR, options.pipefile, options.port[PORT_HTTP] );
if ( file_exist(DIRCHECKER) ) {
	loghandle(LOG_INFO, false, "%s", "Pipe file detected, auto switching to client mode");
	goto CLIENT;
}

memset( &ticks, 0, sizeof(tickDef) );
sprintf( DIRCHECKER, "%s/ticks.ini", sysconfig.directory );
loadconfig(DIRCHECKER,CONFIG_TICKS);

memset( &irccfg, 0, sizeof(ircDef) );
loadconfig(options.sysini,CONFIG_IRC);

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
		loghandle(LOG_INFO, false, "%s", "Directory creation failed, unable to continue.");
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
		loghandle(LOG_INFO, false, "%s", "Directory creation failed, unable to continue.");
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
		loghandle(LOG_INFO, false, "%s", "Directory creation failed, unable to continue.");
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
	loghandle(LOG_INFO, false, "%s", "No map detected now generating...");
	spawn_map();
}
//Begin deamonization and initate server loop.
if( !( daemon_init( ) ) ) {
	loghandle(LOG_CRIT, false, "%s", "<<CRITICAL>> Daemon initiation failed <<CRITICAL>>");
	return 1;
}
if( options.mode == MODE_FORKED ) {
	printf("%s\n", "Returning to shell, daemon has loaded in the background.");
	printf("\n");
}
cleanUp(1);
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
	char buffer[1024] = {0}, bufferstrip[1024] = {0};
	num = read(options.clientpipe, buffer, sizeof(buffer) );
	sprintf(bufferstrip,"%s",trimwhitespace(buffer));
	sprintf(buffer,"%s",bufferstrip);

	if( strncmp(buffer,"<<<END>>>",9) == false )
		break;
	if ( num > 0 ) {
                puts( buffer );
		fflush( stdout );
	}
}

BAILOUT:
cleanUp(0);
printf("\n");


return 0;
}


char *cmdSignalNames[SIGNALS_NUMUSED] =
{
"MISSING",
"Hangup",
"Interrupt",
"Quit",
"Illegal Instruction",
"Trace trap",
"Abort",
"BUS error",
"Floating-Point arithmetic Exception",
"Kill",
"User-defined signal 1",
"Segmentation Violation",
"User-defined signal 2",
"Broken pipe",
"Alarm clock",
"Termination",
"Stack fault",
"Child status has changed",
"Continue",
"Stop",
"Keyboard Stop",
"Background read from tty",
"Background write to tty",
"Urgent condition on socket",
"CPU limit exceeded",
"File size limit exceeded",
"Virtual Time Alarm",
"Profiling alarm clock",
"Window size change",
"I/O now possible",
"Power failure restart",
"Bad system call"
};


void loghandle( int flag, int error, char *fmt, ... ) {
	va_list ap;
	char font[32];
	char ebuffer[MAXLOGSTRING];

va_start(ap, fmt);
vsnprintf(ebuffer, MAXLOGSTRING, fmt, ap);
va_end(ap);

switch(flag) {
	case LOG_INFO:
			strcpy(font,WHITE);
			break;
	case LOG_ERR:
			strcpy(font,YELLOW);
			break;
	case LOG_CRIT:
			strcpy(font,RED);
			break;
	case '?':
			strcpy(font,RESET);
			break;
}


if( error ) {
	if( options.verbose ) {
		printf("%s%s"RESET,font, ebuffer);
		printf(" -- "BOLDBLUE"%s"RESET"\n", strerror(error) );
	}
	syslog(flag, "%s -- %s", ebuffer, strerror(error) );
} else {
	if( options.verbose )
		printf("%s%s\n"RESET,font, ebuffer);
	syslog(flag, "%s", ebuffer );
}

if( options.verbose ) {
	printf(RESET);
	fflush(stdout);
}

return;
}



