#include "config/global.h"

pthread_mutex_t mutex;

optionsDef options = { MODE_DAEMON, { false, false }, -1, -1, -1, true, "", "", "", "status" };

configDef sysconfig;
#if MYSQL_SUPPORT
mySqlDef mysqlcfg;
#endif
adminDef admincfg;
mapcfgDef mapcfg;
tickDef ticks;
#if IRCBOT_SUPPORT
ircDef irccfg;
#endif

char logString[MAXLOGSTRING];

bool firstload = false;

svbanDef banlist = { 0 };

void cleanUp(int type) {
	char DIRCHECKER[PATH_MAX];

if( type ) {
	close(options.serverpipe);
	sprintf( DIRCHECKER, "%s/%d.pipe", TMPDIR, options.port[PORT_HTTP] );
	unlink(DIRCHECKER);
	info( "Server shutdown complete, now cleaning up!" );
	syslog(LOG_INFO, "%s", "<<<<<BREAKER-FOR-NEW-SERVER-INSTANCE>>>>>" ); //Don't really need this, but meh... why not!
	closelog();
} else {
	close(options.clientpipe);
	sprintf( DIRCHECKER, "%s/%d.client.pipe", TMPDIR, options.port[PORT_HTTP] );
	unlink(DIRCHECKER);
}

return;
}




void httpString( ReplyDataPtr rd, char *string ) {

rd->response.off += snprintf (&rd->response.buf[rd->response.off], rd->response.buf_len - rd->response.off, "%s", string);

return;
}

void httpPrintf( ReplyDataPtr rd, char *string, ... ) {
	char text[4096];
	va_list ap;
	va_start( ap, string );

vsnprintf( text, 4096, string, ap );
va_end( ap );

rd->response.off += snprintf (&rd->response.buf[rd->response.off], rd->response.buf_len - rd->response.off, "%s", text);

return;
}


void svSignal( int signal ) {
//	int a, size;
 
if( (signal == SIGNALS_SIGTERM ) || (signal == SIGNALS_SIGINT) ){
	if( options.verbose ) {
		printf("\n");
		fflush(stdout);
	}
	sprintf(logString, "%s Recived; handleing gracefully =)", cmdSignalNames[signal]);
	info( logString );
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


loghandle(LOG_CRIT, false, "ERROR, signal \'%s\'", cmdSignalNames[signal]);
//loghandle(LOG_CRIT, false, "cnt : %d", (int)(intptr_t)svDebugConnection);
loghandle(LOG_CRIT, false, "tick pass : %d", ticks.debug_pass);
loghandle(LOG_CRIT, false, "tick id : %d", ticks.debug_id);
/*
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
}*/

#if IRCBOT_SUPPORT
if( irc_is_connected(irccfg.session) ) {
	irc_send_raw( irccfg.session, "NOTICE %s :Server recived \'%s\' signal -- Shutdown Iniated!", irccfg.channel, cmdSignalNames[signal]);
	if( irc_cmd_quit( irccfg.session, "Server Saftey Triped... Shutting down!" ) ) {
		error("Quitting IRC");
	}
	irc_disconnect( irccfg.session );
}
#endif

sysconfig.shutdown = true;
server_shutdown();
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
#if PIPEFILE_SUPPORT
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
#endif
//This is the actual loop process, which listens and responds to requests on all sockets.
int daemonloop() {
	int curtime;

//Start HTTP Server(s)
if( http_prep() )
	return 1;

if( http_start() )
	return 1;

#if HTTPS_SUPPORT
if( https_start() )
	return 1;
#endif

//Replacment server loop, why use "for" when we can use "while" and its so much cleaner?
while( sysconfig.shutdown == false ) {
	/* Expire HTTP Sessions */
	expire_sessions();

	#if PIPEFILE_SUPPORT
	svPipeScan( options.serverpipe );
	#endif

	loadconfig(options.sysini,CONFIG_BANNED);

	//svDebugConnection = 0;
	curtime = time( 0 );

	if( curtime < ticks.next ) {
		nanosleep((struct timespec[]){{0, ( 500000000 / 4 ) }}, NULL);
		continue;
	}

	ticks.next = ( curtime + sysconfig.ticktime );
	
	if( ( sysconfig.autostop ) && ( timediff(sysconfig.stop) < 1 ) ) {
		ticks.status = false;
	} else if( ( sysconfig.autostart ) && ( timediff(sysconfig.start) < 1 ) ) {
		ticks.status = true;
	}

	cmdTickInit();
	if( ticks.status ) {
		cmdTick();
		ticks.last = curtime;
		ticks.number++;
	}
	cmdTickEnd();

	cmdExecuteFlush();

	if( options.verbose )
		fflush(stdout);
}

#if IRCBOT_SUPPORT
if( irc_is_connected(irccfg.session) ) {
	if( irc_cmd_quit( irccfg.session, "Server Shutdown has been iniated" ) ) {
		error("Quitting IRC");
	}
	irc_disconnect( irccfg.session );
}
#endif

server_shutdown();

return 0;
}

//begin upgrade to daemon, I don't like shell dependancy!
int daemon_init() {
	int binfo[MAP_TOTAL_INFO];
	char DIRCHECKER[PATH_MAX];
	pid_t pid, sid;

info( "Server process iniating...");



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

if( !( dbInit("Database initialisation failed, exiting\n") ) ) {
	loghandle(LOG_CRIT, false, "%s", "Server Database Initation Failed, now exiting..." );
	return 0;
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
#if PIPEFILE_SUPPORT
sprintf( DIRCHECKER, "%s/%d.pipe", TMPDIR, options.port[PORT_HTTP] );
	if( mkfifo(DIRCHECKER, 0666) < 0 ) {
	loghandle(LOG_ERR, errno, "Error creating pipe: %sn", DIRCHECKER );
	options.serverpipe = -1;
} else {
	options.serverpipe = open(DIRCHECKER, O_RDONLY | O_NONBLOCK);
}
#endif
loghandle(LOG_INFO, false, "%s", "All Checks passed, begining server loop..." ); 

//Now create the loop, this used to take place in here... but I decided to move it =P
#if IRCBOT_SUPPORT
if( irccfg.bot ) {
	pthread_create(&irccfg.thread,NULL,ircbot_connect,NULL);
}
#endif

daemonloop();


cmdEnd();
dbEnd();


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
	char mkthisdir[PATH_MAX];
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
			check = mkdir( mkthisdir, S_IRWXU );
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
	char DIRCHECKER[PATH_MAX];
	inikey ini;

if( firstload ) {
	ini = dictionary_new(0);
} else {
	ini = iniparser_load(file);
	if( ini == NULL ) {
		sprintf( logString, "Loading INI File: %s", file );
		if( type != CONFIG_TICKS )
			error( logString );
		return -1;
	}
}

if( iniparser_find_entry(ini,"NEED_TO_DELETE_ME") ) {
	sprintf(logString, "A default, non-usable version of the evsystem.ini has been detected: %s",file);
	info( logString );
	info( "You must edit this file before the game server is able to run correctly!");
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
	sysconfig.cookdomain = strdup( iniparser_getstring(ini, "system:cookiedomain", "") );
	sysconfig.directory = strdup( iniparser_getstring(ini, "system:directory", "/tmp/evcore/data") );
	sysconfig.downfrom = strdup( iniparser_getstring(ini, "system:downfrom", "http://www.sknill.com/evbasic") );
	sysconfig.httpimages = strdup( iniparser_getstring(ini, "system:httpimages", "/tmp/evcore/html/images") );
	sysconfig.httpfiles = strdup( iniparser_getstring(ini, "system:httpfiles", "/tmp/evcore/html/files") );
	sysconfig.httpread = strdup( iniparser_getstring(ini, "system:httpread", "/tmp/evcore/html/read") );
	sysconfig.pubforum = strdup( iniparser_getstring(ini, "system:publicforum", sysconfig.directory ) );

	sysconfig.httpport = iniparser_getint(ini, "system:port", 9990);

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
	#if MYSQL_SUPPORT
	mysqlcfg.enable = iniparser_getboolean(ini, "mysql:enable", false);
	mysqlcfg.host = strdup( iniparser_getstring(ini, "mysql:host", "localhost") );
	mysqlcfg.port = iniparser_getint(ini, "mysql:port", 3306);
	mysqlcfg.user = strdup( iniparser_getstring(ini, "mysql:user", "localhost") );
	mysqlcfg.password = iniparser_getstring(ini, "mysql:password", NULL) ? strdup( iniparser_getstring(ini, "mysql:password", "") ) : NULL;
	mysqlcfg.database = strdup( iniparser_getstring(ini, "mysql:database", "evgame") );
	#endif

	admincfg.numfakes = iniparser_getint(ini, "debug:create_accounts", 0);

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
	if( ( banlist.ip ) && ( banlist.number ) ) {
		free( banlist.ip );
	}
	banlist.number = iniparser_getint(ini, "banned_ips:number", 0);
	if( banlist.number > 0 ) {
		banlist.ip = malloc( banlist.number * sizeof(*banlist.ip));
	}
	for(a = 0; a < banlist.number; a++) {
		sprintf(DIRCHECKER,"banned_ips:ip%d",(a+1));
		banlist.ip[a] = strdup(iniparser_getstring(ini, DIRCHECKER, "0.0.0.0"));
	}
} else if( type == CONFIG_TICKS ) {
	ticks.status = iniparser_getboolean(ini, "ticks:status", false);
	ticks.locked = iniparser_getboolean(ini, "ticks:locked", false);
	ticks.number = iniparser_getint(ini, "ticks:number", 0);
	ticks.round = iniparser_getint(ini, "ticks:round", ( sysconfig.round ? sysconfig.round : 0 ) );
	ticks.speed = iniparser_getint(ini, "ticks:speed", ( sysconfig.ticktime ? sysconfig.ticktime : 3600 ) );
	ticks.last = iniparser_getint(ini, "ticks:last", 0);
	ticks.next = iniparser_getint(ini, "ticks:next", 0);
}
#if IRCBOT_SUPPORT
else if( type == CONFIG_IRC ) {
	irccfg.host = strdup( iniparser_getstring(ini, "irc:host", "irc.freenode.net") );
	irccfg.port = atoi( iniparser_getstring(ini, "irc:port", "6667") );
	strcpy(DIRCHECKER,"#");
	strcat(DIRCHECKER,strdup( iniparser_getstring(ini, "irc:channel", "ectroverse") ) );
	irccfg.channel = strdup(DIRCHECKER);
	irccfg.botnick = strdup( iniparser_getstring(ini, "irc:bot_nick", "EVBot") );
	irccfg.botpass = strdup( iniparser_getstring(ini, "irc:bot_pass", "botpass") );
	irccfg.bot = iniparser_getboolean(ini, "irc:bot_enable", false);
	irccfg.announcetick = iniparser_getboolean(ini, "irc:bot_announcetick", false);
}
#endif

if( firstload ) {
	FILE *dumpfile;
	if( !( iniparser_find_entry(ini,"system") ) ){
		iniparser_set(ini,"system",NULL);
		iniparser_set(ini,"system:name",sysconfig.servername);
		iniparser_set(ini,"system:cookiedomain","yourdomain.com");
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
	if( !( iniparser_find_entry(ini,"syslog") ) ){
		iniparser_set(ini,"syslog",NULL);
		iniparser_set(ini,"syslog:tag", sysconfig.syslog_tagname);
		iniparser_set(ini,"syslog:facility", sysconfig.syslog_facility);
	}
	#if IRCBOT_SUPPORT
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
	#endif
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
	if( !( dumpfile ) ) {
		loghandle(LOG_CRIT, false, "Unable to load config file, and unable to spawn in specified location: \'%s\'", file);
		file = "/tmp/evcore/evsystem.ini";
		dumpfile = fopen(file, "w");
		loghandle(LOG_CRIT, false, "A default, non-usable version of the evsystem.ini has been dumped to: \'%s\'",file);
		loghandle(LOG_CRIT, false, "%s", "You will need to edit and move this file before you can run the server, its best if you use the config directory!");
	} else {
		loghandle(LOG_CRIT, false, "A default, non-usable version of the evsystem.ini has been dumped to: \'%s\'",file);
		loghandle(LOG_CRIT, false, "%s", "You must edit this file according to your needs before you run the game server!");
	}
	if( dumpfile ) {
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

	} else {
		loghandle(LOG_CRIT, errno, "Unable to spawn defaul config file into: \'%s\'",file);
		loghandle(LOG_CRIT, false, "%s", "You may have specified an invalid path...");	
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
	char DIRCHECKER[PATH_MAX];
	inikey ini;
	FILE *file;

sprintf( DIRCHECKER, "%s/ticks.ini", sysconfig.directory );
file = fopen( DIRCHECKER, "r+" );
if(!file)
	file = fopen( DIRCHECKER, "w" );
if(file) {
	fprintf( file, "%s\n", ";Auto generated, there should be no need to edit this file!" );
	ini = dictionary_new(0);
	iniparser_set(ini,"ticks",NULL);
	iniparser_set(ini,"ticks:status",ticks.status ? "true" : "false");
	iniparser_set(ini,"ticks:locked",ticks.locked ? "true" : "false");
	iniparser_set(ini,"ticks:number",itoa(ticks.number));
	iniparser_set(ini,"ticks:round",itoa(ticks.round));
	iniparser_set(ini,"ticks:speed",itoa(ticks.speed));
	iniparser_set(ini,"ticks:last",itoa(ticks.last));
	iniparser_set(ini,"ticks:next",itoa(ticks.next));
	iniparser_set(ini,"ticks:debug_id",itoa(ticks.debug_id));
	iniparser_set(ini,"ticks:debug_pass",itoa(ticks.debug_pass));
	iniparser_set(ini,"ticks:uonline",itoa(ticks.uonline));
	iniparser_set(ini,"ticks:uactive",itoa(ticks.uactive));
	iniparser_set(ini,"ticks:uregist",itoa(ticks.uregist));
	iniparser_dump_ini(ini,file);
	iniparser_freedict(ini);
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
		sprintf(options.sysini, "%s/config/evsystem.ini" ,DIRCHECKER);
	} else {
		perror("getcwd() error");
		result = true;
	}
} else {
	char *pointer = NULL;
	pointer = strrchr( strdup(options.sysini), '/' );
	if( ( options.sysini[0] != '/' ) )
		pointer = NULL;
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
#define CLEARSCR {\
if( system( "clear" ) )\
error( "clearing screen" );\
}

int main( int argc, char *argv[] ) {
	char DIRCHECKER[PATH_MAX];
	#if PIPEFILE_SUPPORT
	int num;
	#endif
	int test;
//CLEARSCR;
if( checkops(argc,argv) ) {
	printf ("Error: Invalid usage detected...\n");
	exit(true);
}

if( file_exist(options.sysini) == 0 ) {
	printf("File does not exist: \'%s\'\n",options.sysini);
	//printf("The above file will be created with a default set, please review the file and reload.\n");
	printf("Use \'-c /path/to/evsystem.ini\' to specify ini file to load (including the file name)\n");
	fflush(stdout);
	firstload = true;
	//exit(true);
} else if( options.verbose ) {
	printf("Loading config from file: \'%s\'\n",options.sysini);
	fflush(stdout);
}

(void)pthread_mutex_init (&mutex, NULL);
dirstructurecheck(TMPDIR);

memset( &sysconfig, 0, sizeof(configDef) );

openlog(argv[0], LOG_CONS | LOG_PID | LOG_NDELAY, LOG_SYSLOG);

memset( &admincfg, 0, sizeof(adminDef) );
if( !(loadconfig(options.sysini,CONFIG_SYSTEM)) ) {
	loghandle(LOG_CRIT, false, "%s", "Unable to load system config, unable to continue.");
	exit(true);
}

if( ( firstload ) || ( sysconfig.shutdown ) )
	goto BAILOUT;
#if PIPEFILE_SUPPORT	
sprintf( DIRCHECKER, "%s/%d.pipe", TMPDIR, options.port[PORT_HTTP] );
if ( file_exist(DIRCHECKER) ) {
	loghandle(LOG_INFO, false, "%s", "Pipe file detected, auto switching to client mode");
	goto CLIENT;
}
#endif
memset( &ticks, 0, sizeof(tickDef) );
sprintf( DIRCHECKER, "%s/ticks.ini", sysconfig.directory );
loadconfig(DIRCHECKER,CONFIG_TICKS);

#if IRCBOT_SUPPORT
memset( &irccfg, 0, sizeof(ircDef) );
loadconfig(options.sysini,CONFIG_IRC);
#endif

loadconfig(options.sysini,CONFIG_BANNED);

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
#if PIPEFILE_SUPPORT
//OK, so we made it down here... that means we are a client and the pipe is active.
CLIENT:

sprintf(DIRCHECKER, "%s/%d.client.pipe", TMPDIR, options.port[PORT_HTTP] );
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
#endif
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


if ( error ) {
	if( options.verbose ) {
		printf("%s%s"RESET,font, ebuffer);
		printf(" -- "BOLDBLUE"#%d, %s"RESET"\n", error, strerror(error) );
	}
	syslog(flag, "%s -- #%d, %s", ebuffer, error, strerror(error) );
} else {
	if( options.verbose )
		printf("%s%s\n"RESET,font, ebuffer);
	syslog(flag, "%s", ebuffer );
}

if( options.verbose ) {
	printf(RESET);
	fflush(stdout);
}

memset( logString, 0, sizeof(logString) );

return;
}




