#include "pipefile.h"

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
	}
	#if IRCBOT_SUPPORT
	 else if( !( strncmp(buffer, "bot", 3) ) ) {
		if( !ircbot_command(buffer) ) {
			loghandle(LOG_INFO, false, "Bot subfunction reported error with command: \"%s\"", buffer);
			svPipeSend(0, "Bot subfunction reported error with command: \"%s\"\n",buffer );
		}
	}
	#endif
	 else {
		loghandle(LOG_INFO, false, "Piping Error Unrecognized command size \"%d\" line \"%s\"", num, buffer);
	}
}

if( stop ) {
	svPipeSend(0,"Server is shutting down as requested..");
	info( "Shutdown command recived from Pipe." );
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
	char DIRCHECKER[PATH_MAX];
	char formatuffer[128] = {0};

va_start(ap, message);
vsnprintf(formatuffer, 128, message, ap);
va_end(ap);

sprintf( DIRCHECKER, "%s/%d.%s", TMPDIR, options.port[PORT_HTTP], ( pipedirection ? "pipe" : "client.pipe" ) );
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
