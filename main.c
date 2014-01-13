#include "config/global.h"

#if PIPEFILE_SUPPORT
#include "pipefile.c"
#endif

#include "loadini.c"

pthread_mutex_t mutex;

ServerOptionsDef options = { MODE_DAEMON, { false
#if HTTPS_SUPPORT
, false
#endif
 }, -1, -1, -1, true, "", "", "", "status" };

SystemCoreDef sysconfig;

TickInfoDef ticks;

#define BREAKER_TAG "<<<<<BREAKER-FOR-NEW-SERVER-INSTANCE>>>>>"

bool firstload = false;

IPBanDef banlist = { 0 };

void cleanUp(int type) {
	char DIRCHECKER[PATH_MAX];

if( type ) {
	close(options.serverpipe);
	sprintf( DIRCHECKER, "%s/%d.pipe", TMPDIR, options.port[PORT_HTTP] );
	unlink(DIRCHECKER);
	info( BREAKER_TAG ); //Don't really need this, but meh... why not!
	closelog();
} else {
	close(options.clientpipe);
	sprintf( DIRCHECKER, "%s/%d.client.pipe", TMPDIR, options.port[PORT_HTTP] );
	unlink(DIRCHECKER);
}

return;
}

void svSignal( int signal ) {

if( (signal == SIGNALS_SIGTERM ) || (signal == SIGNALS_SIGINT) ){
	if( options.verbose ) {
		printf("%s\n", cmdSignalNames[signal] );
		fflush(stdout);
	}
	info( "%s Recived; handleing gracefully =)", cmdSignalNames[signal]);
  	sysconfig.shutdown = true;
	return;
}

loghandle(LOG_CRIT, false, "ERROR, signal \'%s\'", cmdSignalNames[signal]);
loghandle(LOG_CRIT, false, "tick pass : %d", ticks.debug_pass);
loghandle(LOG_CRIT, false, "tick id : %d", ticks.debug_id);


#if IRCBOT_SUPPORT
	ConfigArrayPtr setting;
if( irc_is_connected(sysconfig.irc_session) ) {
	setting = GetSetting( "IRC Channel" );
	irc_send_raw( sysconfig.irc_session, "NOTICE %s :Server recived \'%s\' signal -- Shutdown Iniated!", setting->string_value, cmdSignalNames[signal]);
	if( irc_cmd_quit( sysconfig.irc_session, "Server Saftey Triped... Shutting down!" ) ) {
		error("Quitting IRC");
	}
	irc_disconnect( sysconfig.irc_session );
}
#endif
sysconfig.shutdown = true;
Shutdown();

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

//This is the actual loop process, which listens and responds to requests on all sockets.
int daemonloop() {
	time_t curtime;


//Replacment server loop, why use "for" when we can use "while" and its so much cleaner?
while( sysconfig.shutdown == false ) {
	/* Expire HTTP Stuff */
	WWWExpire();

	#if MULTI_THREAD_SUPPORT == 0
	WWWSelect(false);
	#if IRC_BOT_SUPPORT
	IRCSelect( );
	#endif
	#endif
	
	#if PIPEFILE_SUPPORT
	svPipeScan( options.serverpipe );
	#endif

	loadconfig(options.sysini,CONFIG_BANNED); //FIXME: Need to create an option to edit internal listing instead... this is too dirty.

	//svDebugConnection = 0;
	time( &curtime );

	if( ( ticks.locked ) || ( ( sysconfig.autostop ) && ( timediff( sysconfig.stop ) < 1 ) ) ) {
		ticks.status = false;
	} else if( ( sysconfig.autostart ) && ( timediff(sysconfig.start) < 1 ) ) {
		ticks.status = true;
	}

	if( curtime < ticks.next ) {
		nanosleep((struct timespec[]){{0, ( 500000000 / 4 ) }}, NULL);
		continue;
	}
	
	ticks.next = ( curtime + ticks.speed );
	
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
if( irc_is_connected(sysconfig.irc_session) ) {
	if( irc_cmd_quit( sysconfig.irc_session, "Server Shutdown has been iniated" ) ) {
		error("Quitting IRC");
	}
	irc_disconnect( sysconfig.irc_session );
}
#endif

Shutdown();

return YES;
}

//begin upgrade to daemon, I don't like shell dependancy!
int daemon_init() {
	int binfo[MAP_TOTAL_INFO];
	char DIRCHECKER[PATH_MAX];
	ConfigArrayPtr settings[2];
	pid_t pid, sid;

info( "Server process iniating...");



if( options.mode == MODE_FORKED ) {
pid = fork();
if(pid < 0) {
	error( "Forking Error: %d" )
	return NO;
} else if(pid > 0) {
	return YES; // So we forked it, time to return and wait for results on a client pipe.
}

settings[0] = GetSetting( "Server Name" );
info( "Begining initiation of %s daemon...", settings[0]->string_value );

// First, start a new session
if((sid = setsid()) < 0) {
	error( "setsid has failed, unable to fork into daemon" );
	return NO;
}

// Next, make /tmp/evcore the current directory. -- I do this, just because I can. (It doesn't matter it changes a lot latter, I need to fix that still).
if((chdir(TMPDIR)) < 0) {
	error( "chdir has failed, unable to fork into daemon" );
	return NO;
}

// Reset the file mode
umask(0);

//Close unneeded file descriptors
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);

}

settings[0] = GetSetting( "Tick Speed" );
ticks.speed = (int)settings[0]->num_value;
ticks.next = time(0) + ticks.speed;
	
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
	
RANDOMIZE_SEED;

if( dbInit() == NO ) {
	loghandle(LOG_CRIT, false, "%s", "Server Database Initation Failed, now exiting..." );
	return NO;
}


if( cmdInit() == NO )  {
	loghandle(LOG_CRIT, false, "%s", "Server Command Initation Failed, now exiting..." );
	return NO;
}

settings[0] = GetSetting( "Directory" );
sprintf( DIRCHECKER, "%s/data", settings[0]->string_value );  
if( chdir( DIRCHECKER ) == -1 ) {
	if( options.verbose )
	loghandle(LOG_ERR, errno, "Change into Database Dir \"%s\" Failed, exiting", DIRCHECKER );
	return NO;
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
	error( "Error creating pipe: %s", DIRCHECKER );
	options.serverpipe = -1;
} else {
	options.serverpipe = open(DIRCHECKER, O_RDONLY | O_NONBLOCK);
}
#endif

if( http_prep() == NO ) {
	critical( "HTTP Preperation Failed" );
	return NO;
} else {
	info( "HTTP Settings initalized..." );
}

info( "All Checks passed, begining server loop..." ); 

#if FACEBOOK_SUPPORT
settings[0] = GetSetting( "Facebook Application" );
settings[1] = GetSetting( "Facebook Secret" );
if( strlen( settings[0]->string_value ) && strlen( settings[1]->string_value ) ) {
	facebook_apptoken( &sysconfig.facebook_token );
	info( "Loading the Facebook Session Token... %s", ( sysconfig.facebook_token ) ? "Sucessfull" : "Failed" );
} else {
	info( "Unable to load Facebook Token due to bad ini settings" );
}
#endif

//Now create the loop, this used to take place in here... but I decided to move it =P
#if IRCBOT_SUPPORT
if( sysconfig.irc_enabled ) {
	#if MULTI_THREAD_SUPPORT
	pthread_create(&sysconfig.irc_thread,NULL,ircbot_connect,NULL);
	#else
	ircbot_connect();
	#endif
}
#endif

//Start HTTP Server(s)


if( http_start() == NO ) {
	critical( "HTTP Server failed to start" );
	return NO;
}

#if HTTPS_SUPPORT
if( https_start() == NO )
	error( "HTTPS Server failed to start" );
#endif

if( daemonloop() == NO )
	return NO;


cmdEnd();
dbEnd();


return YES;
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
				info( "Directory \"%s\" created.", mkthisdir );
			} else if ( errno != 17 ) {
				error( "Error creating directory: \"%s\"", mkthisdir );
			} else if( errno == 17 ) {
				errno = 0;
			}
		}
	}
}
free( split );
free( strCpy );


return;
}

int loadconfig( char *file, int type ) {
	int a;
	int logfac = LOG_SYSLOG;
	char DIRCHECKER[PATH_MAX];
	ConfigArrayPtr settings;
	inikey ini;

if( firstload ) {
	DumpDefaults( file );
	return -1;
}

if( sysconfig.shutdown ) {
	return -1;
}

if( type == CONFIG_SYSTEM ) {
	//Enter new scaner.
	settings = GetSetting( "Auto Start" );
	sysconfig.autostart = settings->num_value;
	if( settings->num_value ) {
		settings = GetSetting( "Start Second" );
		sysconfig.start.tm_sec = (int)settings->num_value;
		settings = GetSetting( "Start Minute" );
		sysconfig.start.tm_min = (int)settings->num_value;
		settings = GetSetting( "Start Hour" );
		sysconfig.start.tm_hour = (int)settings->num_value;
		settings = GetSetting( "Start Day" );
		sysconfig.start.tm_mday = (int)settings->num_value;
		settings = GetSetting( "Start Month" );
		sysconfig.start.tm_mon = ( settings->num_value > 0 ) ? (int)( settings->num_value - 1 ) : -1;
		settings = GetSetting( "Start Year" );
		sysconfig.start.tm_year = ( settings->num_value > 0 ) ? (int)( settings->num_value + 100 ) : -1;
	}
	settings = GetSetting( "Auto Stop" );
	sysconfig.autostop = settings->num_value;
	if( settings->num_value ) {
		settings = GetSetting( "Stop Second" );
		sysconfig.stop.tm_sec = (int)settings->num_value;
		settings = GetSetting( "Stop Minute" );
		sysconfig.stop.tm_min = (int)settings->num_value;
		settings = GetSetting( "Stop Hour" );
		sysconfig.stop.tm_hour = (int)settings->num_value;
		settings = GetSetting( "Stop Day" );
		sysconfig.stop.tm_mday = (int)settings->num_value;
		settings = GetSetting( "Stop Month" );
		sysconfig.stop.tm_mon = ( settings->num_value > 0 ) ? (int)( settings->num_value - 1 ) : -1;
		settings = GetSetting( "Stop Year" );
		sysconfig.stop.tm_year = ( settings->num_value > 0 ) ? (int)( settings->num_value + 100 ) : -1;

	}
	//End config scanning... handle variables.
	settings = GetSetting( "HTTP Port" );
	options.port[PORT_HTTP] = options.port[PORT_HTTP] ? options.port[PORT_HTTP] : (int)settings->num_value;
	#if HTTPS_SUPPORT
	settings = GetSetting( "HTTPS Port" );
	options.port[PORT_HTTPS] = options.port[PORT_HTTPS] ? options.port[PORT_HTTPS] : (int)settings->num_value;
	#endif
	settings = GetSetting( "Log Handle" );
	if( strlen( settings->string_value ) && strcmp( settings->string_value, "LOG_SYSLOG" ) ) {
		closelog();
		if( strcmp( settings->string_value, "LOG_DAEMON" ) == 0 ) {
			logfac = LOG_DAEMON;
		} else if( strcmp( settings->string_value, "LOG_USER" ) == 0 ) {
			logfac = LOG_USER;
		} else if( strcmp( settings->string_value, "LOG_LOCAL0" ) == 0 ) {
			logfac = LOG_LOCAL0;
		} else if( strcmp( settings->string_value, "LOG_LOCAL1" ) == 0 ) {
			logfac = LOG_LOCAL1;
		} else if( strcmp( settings->string_value, "LOG_LOCAL2" ) == 0 ) {
			logfac = LOG_LOCAL2;
		} else if( strcmp( settings->string_value, "LOG_LOCAL3" ) == 0 ) {
			logfac = LOG_LOCAL3;
		} else if( strcmp( settings->string_value, "LOG_LOCAL4" ) == 0 ) {
			logfac = LOG_LOCAL4;
		} else if( strcmp( settings->string_value, "LOG_LOCAL5" ) == 0 ) {
			logfac = LOG_LOCAL5;
		} else if( strcmp( settings->string_value, "LOG_LOCAL6" ) == 0 ) {
			logfac = LOG_LOCAL6;
		} else if( strcmp( settings->string_value, "LOG_LOCAL7" ) == 0 ) {
			logfac = LOG_LOCAL7;
		}
		settings = GetSetting( "Log Tag" );
		openlog( settings->string_value, LOG_CONS | LOG_PID | LOG_NDELAY, logfac);
	}
} else if( type == CONFIG_BANNED ) {
	if( ( ( ini = iniparser_load(file) ) == NULL ) || ( ini == NULL ) ) {
			error( "Loading INI File: %s", file );
		return -1;
	}
	if( ( sysconfig.banlist.ip ) && ( sysconfig.banlist.number ) ) {
		free( sysconfig.banlist.ip );
	}
	sysconfig.banlist.number = iniparser_getint(ini, "banned_ips:number", 0);
	if( sysconfig.banlist.number > 0 ) {
		sysconfig.banlist.ip = calloc( sysconfig.banlist.number, sizeof(*sysconfig.banlist.ip) );
	}
	for(a = 0; a < sysconfig.banlist.number; a++) {
		sprintf(DIRCHECKER,"banned_ips:ip%d",(a+1));
		sysconfig.banlist.ip[a] = strdup(iniparser_getstring(ini, DIRCHECKER, "0.0.0.0"));
	}
	iniparser_freedict(ini);
} else if( type == CONFIG_TICKS ) {
	if( ( ( ini = iniparser_load(file) ) == NULL ) || ( ini == NULL ) ) {
		return -1;
	}
	ticks.status = iniparser_getboolean(ini, "ticks:status", false);
	ticks.locked = iniparser_getboolean(ini, "ticks:locked", false);
	ticks.number = iniparser_getint(ini, "ticks:number", 0);
	ticks.round = iniparser_getint(ini, "ticks:round", 0 );
	settings = GetSetting( "Tick Speed" );
	ticks.speed = iniparser_getint(ini, "ticks:speed", (int)settings->num_value );
	ticks.last = iniparser_getint(ini, "ticks:last", 0);
	ticks.next = iniparser_getint(ini, "ticks:next", 0);
	iniparser_freedict(ini);
}

return 1;
}


int savetickconfig() {
	char DIRCHECKER[PATH_MAX];
	ConfigArrayPtr settings;
	FILE *file;
	inikey ini;

settings = GetSetting( "Directory" );
sprintf( DIRCHECKER, "%s/ticks.ini", settings->string_value );
file = fopen( DIRCHECKER, "rb+" );
if(!file)
	file = fopen( DIRCHECKER, "wb" );
if(file) {
	ini = dictionary_new(0);
	fprintf( file, "%s\n", ";Auto generated, there should be no need to edit this file!" );
	iniparser_set( ini,"ticks",NULL);
	iniparser_set( ini,"ticks:status",ticks.status ? "true" : "false");
	iniparser_set( ini,"ticks:locked",ticks.locked ? "true" : "false");
	iniparser_set_string( ini,"ticks:number", "%d", ticks.number);
	iniparser_set_string( ini,"ticks:round", "%d", ticks.round);
	iniparser_set_string( ini,"ticks:speed", "%d", ticks.speed);
	iniparser_set_string( ini,"ticks:last", "%d", ticks.last);
	iniparser_set_string( ini,"ticks:next", "%d", ticks.next);
	
	iniparser_set_string( ini,"ticks:debug_id", "%d", ticks.debug_id);
	iniparser_set_string( ini,"ticks:debug_pass", "%d", ticks.debug_pass);
	
	iniparser_set_string( ini,"ticks:uonline", "%d", ticks.uonline);
	iniparser_set_string( ini,"ticks:uactive", "%d", ticks.uactive);
	iniparser_set_string( ini,"ticks:uregist", "%d", ticks.uregist);

	iniparser_dump_ini( ini, file);
	iniparser_freedict( ini );
	fflush( file );
	fclose( file );
}


return 1;
}


static int checkops(int argc, char **argv) {
	int opt_index;
	char DIRCHECKER[512] = {0};
	bool result;
	int index;
	int opt;

	static char short_opt[] =
	#if HTTPS_SUPPORT
	"c:fm:p:e:qs:";
	#else
	"c:fm:p:qs:";
	#endif

static struct option long_opt[] =
   {
      {"config", required_argument, 0, 'c'},
      {"forked", no_argument, 0, 'f'},
      {"mapini", required_argument, 0, 'm'},
      {"port", required_argument, 0, 'p'},
      #if HTTPS_SUPPORT
      {"https", required_argument, 0, 'e'},
      #endif
      {"quiet", no_argument, 0, 'q'},
      {"send", optional_argument, 0, 's'},
      {NULL, 0, 0, 0 }
   };

opterr = 0;
result = false;
while( (opt = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1) {
	switch(opt) {
		case -1: // no more arguments
         		break;
         	case 0: // long options toggles
            		break;
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
		#if HTTPS_SUPPORT
		case 'e':
			options.port[PORT_HTTPS] = atoi(optarg);
			break;
		#endif
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
		sprintf(options.sysini, "%s/config/evconfig.ini" ,DIRCHECKER);
	} else {
		perror("getcwd() error");
		result = true;
	}
} else {
	char *input = strdup( options.sysini );
	char *pointer = strrchr( input, '/' );
	if( ( options.sysini[0] != '/' ) ) {
		pointer = NULL;
	}
	if( !( pointer ) ) {
		if (getcwd(DIRCHECKER, sizeof(DIRCHECKER)) != NULL) {
			sprintf(options.sysini, "%s/%s" ,DIRCHECKER, input);
		} else {
			perror("getcwd() error");
			result = true;
		}
	}
	free( input );
}

for( index = optind; index < argc; index++ ) {
	printf ("Non-option argument: \'%s\'\n", argv[index]);
	result = true;
}

return result;
}


int main( int argc, char *argv[] ) {
	ConfigArrayPtr settings;
	int a;
	char DIRCHECKER[PATH_MAX];
	#if PIPEFILE_SUPPORT
	int num;
	#endif
	int test;

if( checkops(argc,argv) ) {
	printf("Error: Invalid usage detected...\n");
	exit(true);
}

if( file_exist(options.sysini) == 0 ) {
	printf("File does not exist: \'%s\'\n",options.sysini);
	//printf("The above file will be created with a default set, please review the file and reload.\n");
	printf("Use \'-c /path/to/evconfig.ini\' to specify ini file to load (including the file name)\n");
	firstload = true;
} else if( options.verbose ) {
	info("Using config file: \'%s\'",options.sysini);
}

(void)pthread_mutex_init (&mutex, NULL);


dirstructurecheck(TMPDIR);

memset( &sysconfig, 0, sizeof(SystemCoreDef) );

openlog(argv[0], LOG_CONS | LOG_PID | LOG_NDELAY, LOG_SYSLOG);


if( !(loadconfig(options.sysini,CONFIG_SYSTEM)) ) {
	critical( "Unable to load system config, unable to continue." );
	exit(true);
}

if( ( firstload ) || ( sysconfig.shutdown ) )
	goto BAILOUT;
#if PIPEFILE_SUPPORT	
sprintf( DIRCHECKER, "%s/%d.pipe", TMPDIR, options.port[PORT_HTTP] );
if ( file_exist(DIRCHECKER) ) {
	info( "Pipe file detected, auto switching to client mode" );
	goto CLIENT;
}
#endif
memset( &ticks, 0, sizeof(TickInfoDef) );
settings = GetSetting( "Directory" );
sprintf( DIRCHECKER, "%s/ticks.ini", settings->string_value );
loadconfig(DIRCHECKER,CONFIG_TICKS);

loadconfig(options.sysini,CONFIG_BANNED);

//check basic dir structure and create as needed.	
sprintf( DIRCHECKER, "%s/data", settings->string_value );
dirstructurecheck(DIRCHECKER);
sprintf( DIRCHECKER, "%s/users", settings->string_value );
dirstructurecheck(DIRCHECKER);
sprintf( DIRCHECKER, "%s/logs", settings->string_value );
dirstructurecheck(DIRCHECKER);
sprintf( DIRCHECKER, "%s/rankings", settings->string_value );
dirstructurecheck(DIRCHECKER);

//well its not really public yet now is it... <<<WORKNEEDED>>>
sprintf( DIRCHECKER, "%s/forum", settings->string_value );
dirstructurecheck(DIRCHECKER);

#define SPAWNABLE_DIRS 3
	ConfigArrayPtr Files[3];
Files[0] = GetSetting( "HTTP Files" );
Files[1] = GetSetting( "HTTP Images" );
Files[3] = GetSetting( "HTTP Text" );
	char *location[SPAWNABLE_DIRS] = { Files[3]->string_value, Files[0]->string_value, Files[1]->string_value };
	char *getfile[SPAWNABLE_DIRS] = { "text.tar.gz", "files.tar.gz", "images.tar.gz" };
settings = GetSetting( "Download Source" );
for( a = 0; a < SPAWNABLE_DIRS; a++ ) {
	
	if( !( file_exist( location[a] ) ) ) {
		dirstructurecheck(location[a]);
		if( !(file_exist(location[a]) ) ) {
			info( "Directory creation failed, unable to continue.");
			return 1;
		}
		printf("Doc base not found, fetching \"%s/%s\" with wget ...", settings->string_value, getfile[a]);
		fflush(stdout);
		syslog(LOG_INFO, "Doc base not found, fetching \"%s/%s\" with wget.\n", settings->string_value, getfile[a]);
		sprintf(DIRCHECKER,"wget -q \"%s/%s\" -O %s/%s", settings->string_value, getfile[a], TMPDIR, getfile[a]);
		test = system(DIRCHECKER);
		printf(" %s!\n", test ? "Fail" : "Done");
		fflush(stdout);
		
		if(test)
			return 1;
		printf("Extracting files to: \"%s\" ...", location[a]);
		fflush(stdout);
		syslog(LOG_INFO, "Extracting files to: \"%s\"\n", location[a]);
		sprintf(DIRCHECKER,"tar -xzf %s/%s -C %s", TMPDIR, getfile[a], location[a]);
		test = system(DIRCHECKER);
		printf(" %s!\n", test ? "Fail" : "Done");
		fflush(stdout);
		
		if(test)
			return 1;
	}

}
/*
spawn_map();
exit(0);
*/
printf("\n");
settings = GetSetting( "Directory" );
sprintf( DIRCHECKER, "%s/data/map", settings->string_value );
if( !( file_exist(DIRCHECKER) ) ) {
	info("No map detected now generating...");
	if( spawn_map() == NO ) {
		goto BAILOUT;
	}
}
//Begin deamonization and initate server loop.

if( daemon_init( ) == NO ) {
	critical("<<CRITICAL>> Daemon initiation failed <<CRITICAL>>");
	return 1;
}

if( options.mode == MODE_FORKED ) {
	info("Returning to shell, daemon has loaded in the background.");
}

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
NULL,
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

int bitflag( int dest, int flag ) {
	int set;
set = ( 1 << flag );
return ( ( set & dest ) ? true : false );
}

void bitflag_add( int *dest, int flag ) {

if( bitflag( *dest, flag ) == false ) {
	*dest |= ( 1 << flag );
}

return;
}


void bitflag_remove( int *dest, int flag ) {

if( bitflag( *dest, flag ) == true ) {
	 *dest = *dest & ~( 1 << flag );
}

return;
}


void bitflag_toggle( int *dest, int flag ) {

*dest ^= flag;

return;
}

//Just a pointer not a define, is memory is needed we assign it dynamicly.
static StringBufferPtr LogNoFile;

void loghandle( int flag, int error, char *fmt, ... ) {
	char font[32];
	char timebuf[512];
	char fname[PATH_MAX];
	char ebuffer[MAXLOGSTRING];
	StringBufferDef internal;
	ConfigArrayPtr settings;
	FILE *file = NULL;
	time_t time_r;
	va_list ap;

memset( &internal, 0, sizeof(StringBufferDef) );
va_start(ap, fmt);
vsnprintf(ebuffer, MAXLOGSTRING, fmt, ap);
va_end(ap);

time_r = ( time(0) + SESSION_TIME );
strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &time_r ) );

settings = GetSetting( "Directory" );
if( settings->string_value ) {
	sprintf( fname, "%s/logs/core_server.log", settings->string_value );
	if( !( file = fopen( fname, "a" ) ) ) {
		if( !( file = fopen( fname, "w+" ) ) && ( LogNoFile == NULL ) ) {
			//File unable to be created, probably because the dir doesn't exist yet... so lets start buffering output.
			LogNoFile = calloc( 1, sizeof(StringBufferDef) );
		}
	}
}

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

AddBufferString( &internal, timebuf );
AddBufferString( &internal, " -- " );

if ( error ) {
	if( options.verbose ) {
		printf("%s%s"RESET,font, ebuffer);
		printf(" -- "BOLDBLUE"#%d, %s"RESET"\n", error, strerror(error) );
	}
	AddBufferPrint( &internal, "%s -- #%d, %s", ebuffer, error, strerror(error)  );
	syslog(flag, "%s -- #%d, %s", ebuffer, error, strerror(error) );
} else {
	if( ( options.verbose ) && strncmp(ebuffer, BREAKER_TAG, strlen( BREAKER_TAG ) ) ) {
		printf("%s%s\n"RESET,font, ebuffer);
	}
	AddBufferString( &internal, ebuffer );
	syslog(flag, "%s", ebuffer );
}

if( options.verbose ) {
	printf(RESET);
	fflush(stdout);
}

if( file ) {
	//Check to see if this function was called before the file/folder was ready
	if( LogNoFile != NULL ) {
		fputs( LogNoFile->buf, file );
		free( LogNoFile->buf );
		free( LogNoFile );
		//Reset pointer to NULL, this prevents double free.
		LogNoFile = NULL;
	}
	if( internal.buf ) {
		fputs( internal.buf, file );
	} else {
		if( options.verbose ) {
			printf( "Blank String, how strange...\n" );
			fflush(stdout);
		}
		fputs( "Blank String, how strange...", file );
	}
	fputs( "\n", file );
	fflush( file );
	fclose( file );
} else {
	AddBufferPrint( LogNoFile, "%s\n", internal.buf );
}

if( internal.buf != NULL )
	free( internal.buf );


errno = 0;

return;
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; ( tmp = strstr(ins, rep) ); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

