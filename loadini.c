#include "loadini.h"
//Yep, so I don't like the way I was calling setting from the INI file... so to heck with it.
//I'm going to write a few new functions using a list table similar to HTML pages...
//Lets hope I don't make a mess of this one like I did the last hehe =P

ConfigListArrayDef config_listing[] = {
	{ INI_TYPE_STRING, "system:downfrom", "http://www.sknill.com/evbasic", "Download Source" },
	{ INI_TYPE_STRING, "syslog:facility", "LOG_LOCAL6", "Log Handle" },
	{ INI_TYPE_STRING, "syslog:tag", "EVServer", "Log Tag" },
	
	{ INI_TYPE_INT, "system:http_port", "9990", "HTTP Port" },
	
	{ INI_TYPE_STRING, "system:name", "NEctroverse", "Server Name" },
	{ INI_TYPE_STRING, "system:cookiedomain", "false", "Cookie Domain" },

	{ INI_TYPE_STRING, "system:directory", "/tmp/evcore/data", "Directory" },
	{ INI_TYPE_STRING, "system:httpimages", "/tmp/evcore/html/images", "HTTP Images" },
	{ INI_TYPE_STRING, "system:httpfiles", "/tmp/evcore/html/files", "HTTP Files" },
	{ INI_TYPE_STRING, "system:httpread", "/tmp/evcore/html/read", "HTTP Text" },
	{ INI_TYPE_STRING, "system:publicforum", "/tmp/evcore/data", "Public Forum" },

	{ INI_TYPE_STRING, "system:round_flag", "", "Round Flag" },
	{ INI_TYPE_INT, "system:notices", "5", "Display Notices" },

	#if HTTPS_SUPPORT
	{ INI_TYPE_INT, "https:port", "9991", "HTTPS Port" },
	{ INI_TYPE_STRING, "https:key", "", "HTTPS Key" },
	{ INI_TYPE_STRING, "https:cert", "", "HTTPS Cert" },
	{ INI_TYPE_STRING, "https:trust", "", "HTTPS Trust" },
	#endif

	//{ INI_TYPE_INT, "ticks:round", "0", "Round Number" },
	{ INI_TYPE_INT, "ticks:tick_time", "3600", "Tick Speed" },
	{ INI_TYPE_INT, "ticks:stockpile", "0", "Stockpile" },
	{ INI_TYPE_INT, "ticks:victory_afterticks", "0", "Auto Victory" },
	{ INI_TYPE_INT, "ticks:endwar_afterticks", "0", "End Wars" },
	

	//Auto Start
	{ INI_TYPE_BOOL, "auto_start:enable", "false", "Auto Start" },
	{ INI_TYPE_INT, "auto_start:second", "-1", "Start Second" },
	{ INI_TYPE_INT, "auto_start:minute", "-1", "Start Minute" },
	{ INI_TYPE_INT, "auto_start:hour", "-1", "Start Hour" },
	{ INI_TYPE_INT, "auto_start:day", "-1", "Start Day" },
	{ INI_TYPE_INT, "auto_start:month", "-1", "Start Month" },
	{ INI_TYPE_INT, "auto_start:year", "-1", "Start Year" },
	//Auto Stop
	{ INI_TYPE_BOOL, "auto_stop:enable", "false", "Auto Stop" },
	{ INI_TYPE_INT, "auto_stop:second", "-1", "Stop Second" },
	{ INI_TYPE_INT, "auto_stop:minute", "-1", "Stop Minute" },
	{ INI_TYPE_INT, "auto_stop:hour", "-1", "Stop Hour" },
	{ INI_TYPE_INT, "auto_stop:day", "-1", "Stop Day" },
	{ INI_TYPE_INT, "auto_stop:month", "-1", "Stop Month" },
	{ INI_TYPE_INT, "auto_stop:year", "-1", "Stop Year" },
	
	#if FACEBOOK_SUPPORT
	{ INI_TYPE_STRING, "facebook:app_id", "", "Facebook Application" },
	{ INI_TYPE_STRING, "facebook:app_secret", "", "Facebook Secret" },
	#endif
	
	#if MYSQL_SUPPORT
	{ INI_TYPE_BOOL, "mysql:enable", "false", "mySql Enable" },
	{ INI_TYPE_STRING, "mysql:host", "localhost", "mySql Host" },
	{ INI_TYPE_INT, "mysql:port", "3306", "mySql Port" },
	{ INI_TYPE_STRING, "mysql:user", "localhost", "mySql User" },
	{ INI_TYPE_STRING, "mysql:password", "", "mySql Password" },
	{ INI_TYPE_STRING, "mysql:database", "evgame", "mySql Database" },
	#endif
	
	#if IRCBOT_SUPPORT
	{ INI_TYPE_STRING, "irc:host", "irc.freenode.net", "IRC Host" },
	{ INI_TYPE_INT, "irc:port", "6667", "IRC Port" },
	{ INI_TYPE_STRING, "irc:channel", "ectroverse", "IRC Channel" },
	{ INI_TYPE_STRING, "irc:bot_nick", "EVBot", "IRC Bot Nick" },
	{ INI_TYPE_STRING, "irc:bot_pass", "botpass", "IRC Bot Pass" },
	{ INI_TYPE_BOOL, "irc:bot_enable", "false", "IRC Enable" },
	{ INI_TYPE_BOOL, "irc:bot_announcetick", "false", "IRC Announce" },
	#endif
	
	{ INI_TYPE_INT, "debug:create_fake_accounts", "0", "Create Fakes" },
	{ INI_TYPE_INT, "admin:number", "0", "Create Admins" },
	{ INI_TYPE_INT, "admin:empire", "0", "Admin Empire Number" },
	{ INI_TYPE_STRING, "admin:empire_password", "password", "Admin Empire Password" },
	{ INI_TYPE_STRING, "admin:empire_name", "Administration", "Admin Empire Name" },
	{ INI_TYPE_BOOL, "admin:ommit_from_ranks", "false", "Admin Empire Ommit" },

	{ INI_TYPE_INT, "map:size", "100", "Map Size" },
	//{ INI_TYPE_INT, "map:size", mapcfg.sizex, "Admin Empire Ommit" },
	{ INI_TYPE_INT, "map:systems", "250", "Map Systems" },
	{ INI_TYPE_INT, "map:families", "10", "Map Families" },
	{ INI_TYPE_INT, "map:members_perfamily", "10", "Map Family Members" },
	{ INI_TYPE_INT, "map:border", "20", "Map Border" },
	{ INI_TYPE_INT, "map:lenghtbase", "2", "Map Lenght" },
	{ INI_TYPE_INT, "map:lenghtvar", "24", "Map Lenght Var" },
	{ INI_TYPE_INT, "map:linknum", "60", "Map Link" },
	{ INI_TYPE_DOUBLE, "map:linkradius", "8.0", "Map Link Radius" },
	{ INI_TYPE_DOUBLE, "map:anglevar", "1024.0", "Map Angle Var" },

	{ INI_TYPE_DOUBLE, "map:solar", "0", "Map Solar Bonus" },
	{ INI_TYPE_DOUBLE, "map:mineral", "0", "Map Mineral Bonus" },
	{ INI_TYPE_DOUBLE, "map:crystal", "0", "Map Crystal Bonus" },
	{ INI_TYPE_DOUBLE, "map:ectrolium", "0", "Map Ectrolium Bonus" },
	{ INI_TYPE_DOUBLE, "map:research", "0", "Map Research Bonus" },
	{ INI_TYPE_DOUBLE, "map:disease", "0", "Map Disease Bonus" },
	{ INI_TYPE_DOUBLE, "map:fission", "0", "Map Fission Bonus" }, //Normaly 0, because nuked planets get this.

	{ false, NULL, NULL, NULL }, // NULL terminator, just to be "safe"	
};

ConfigArrayPtr ConfigList;
ConfigArrayPtr ConfigTable[ARRAY_MAX];

inikey config;

//Lets make a Config Array with an index, we won't actually store much in here... it just stores pointers and nice names etc.
ConfigArrayPtr loadfromconfig( char *name, char *sourcefile, int sourceline ) {
	int index;
	double store;
	double out;
	char *setting = NULL;
	ConfigArrayPtr newset;

store = out = 0;
index = 0;
while( ( config_listing[index].nicename != NULL ) && ( strcmp(config_listing[index].nicename, name ) != 0 ) )
	index++;

if( ( config_listing[index].origin == NULL) ) {
	if( sysconfig.shutdown == true )
		return NULL;
	loghandle(LOG_CRIT, 0, "End of list reached, setting not listed: \'%s\' ( Called from %s on line:%d )", name, sourcefile, sourceline);
	loghandle(LOG_CRIT, 0, "Shutting down to avoid serious errors, setting has no default so NULL will be returned", name, sourcefile, sourceline);
	Shutdown();
	return NULL;
}

//Double check before returning data from index...
if( ( ConfigTable[index] != NULL ) && ( ConfigTable[index]->id == index ) )
	return ConfigTable[index];

//Ok, setting not found in the existing hash table... But we have it listed in our setting struct
//Reload the config file and fetch it if possible. If not, we'll default and pass an info notice.
if( config == NULL )
	config = iniparser_load( options.sysini );

//Yes, we always check for this line... it means the server has not been configured correctly.
if( iniparser_find_entry( config,"NEED_TO_DELETE_ME" ) ) {
	printf( RED"A default, non-usable version of the ini file has been detected: %s\n"RESET, options.sysini );
	printf( RED"You must edit this file before the game server is able to run correctly!\n"RESET );
	exit(0);
}

if( ( iniparser_find_entry(config, config_listing[index].origin ) == 0 ) ) {
	info( "Setting: \'%s\' referance to \'%s\' defaulting as: \'%s\' ( Called from %s on line:%d )", config_listing[index].nicename, config_listing[index].origin, config_listing[index].ifblank, sourcefile, sourceline );
} 

//Fetch default settings, and check is setting exists in ini file... 
if( config_listing[index].type == INI_TYPE_STRING ) {
	setting = iniparser_getstring(config, config_listing[index].origin, config_listing[index].ifblank );
} else if ( config_listing[index].type == INI_TYPE_DOUBLE ) {
	if( sscanf( config_listing[index].ifblank, "%lf", &store ) != 1 )
		error( "Getting Default Value for setting: %s", name );
	out = iniparser_getdouble(config, config_listing[index].origin, store );
} else if ( config_listing[index].type == INI_TYPE_INT ) {
	if( sscanf( config_listing[index].ifblank, "%lf", &store ) != 1 )
		error( "Getting Default Value for setting: %s", name );
	out = iniparser_getint(config, config_listing[index].origin, (int)store );
} else if ( config_listing[index].type == INI_TYPE_BOOL ) {
	out = iniparser_getboolean(config, config_listing[index].origin, ( strcmp( config_listing[index].ifblank, "false" ) ? true : false ) );
}

//Got all that, so now than lets make it go!
if( ( ( ConfigTable[index] != NULL ) && ( ConfigTable[index]->id == index ) ) ) {
	/*
	 * Shoulden't really get here... if we do, it's because settings are being called too fast. 
	 * -- Ignorable, just informational. Causes no trouble... as the index is fixed with build.
	 * 
	 */
	loghandle(LOG_ERR, 0, "Entry already indexed: \'%s\' ( Called from %s on line:%d )", name, sourcefile, sourceline);
} else {
	newset = calloc( 1, sizeof(ConfigArrayDef) );
	newset->id = index;
	newset->type = config_listing[index].type;
	newset->name = config_listing[index].nicename;
	newset->origin = config_listing[index].origin;
	if( setting != NULL )
	newset->string_value = strdup( setting );
	else
	newset->string_value = NULL;
	newset->num_value = out;
	newset->next = ConfigList;
	ConfigList = newset;
	ConfigTable[index] = newset;
}

if( config != NULL ) {
	iniparser_freedict( config );
	config = NULL;
}

return ConfigTable[index];
}
/*
 *  -- This is ONLY for cmd.c Admin creation --
 * Fetch list of Admin details needed for account creation, number was already loaded and so it's just passed in
 * Rember that returns of this function need to be free'd once finished with 
 *
 */
dbUserInfoPtr ListAdmins( int num ) {
	dbUserInfoPtr array;
	char buffer[DEFAULT_BUFFER];
	int a;

if( config == NULL )
	config = iniparser_load( options.sysini );
	
if( ( array = calloc( num, sizeof(dbUserInfoDef) ) ) == NULL )
	critical( "Allocating Admins" );
	
for( a = 0; a < num; a++ ) {
	snprintf(buffer, sizeof(buffer), "admin%d:name", a+1 );
	strcpy( array[a].name, iniparser_getstring(config, buffer, "" ) );
	snprintf(buffer, sizeof(buffer), "admin%d:password", a+1 );
	strcpy( array[a].password, iniparser_getstring(config, buffer, "" ) );
	snprintf(buffer, sizeof(buffer), "admin%d:faction", a+1 );
	strcpy( array[a].email, iniparser_getstring(config, buffer, "" ) );
	snprintf(buffer, sizeof(buffer), "admin%d:forumtag", a+1 );
	strcpy( array[a].forumtag, iniparser_getstring(config, buffer, "" ) );
	snprintf(buffer, sizeof(buffer), "admin%d:level", a+1 );
	array[a].level = iniparser_getint(config, buffer, 0 );
	snprintf(buffer, sizeof(buffer), "admin%d:race", a+1 );
	array[a].flags = iniparser_getint(config, buffer, 0 );
}

if( config != NULL ) {
	iniparser_freedict( config );
	config = NULL;
}

return array;
}

/*
 * Hmmz, I'm so tired of calling settings one-by-one... so here's that listing function
 * Yer, this was cause of map generation... it needs a fair few settings at once, and I can either call them again and again
 *... Or, make one list and just use them over and over, just remember to free any usage of this function.
 *  ----- End of List should contain NULL, for safe termination. -----
 *
 */
ConfigArrayPtr makelistfromconfig( char **list, char *sourcefile, int sourceline ) {
	ConfigArrayPtr array;
	int a, count;

count = 0;
for( a = 0; list[a] != NULL ; a++ ) {
	count++;
}

if( ( a != count) ) {
	error( "Tally mismatch" );
	return NULL;
}


if( ( array = calloc( count, sizeof(ConfigArrayDef) ) ) == NULL ) {
	critical( "Allocating List Array" );
	return NULL;
}

for( a = 0; list[a]; a++ ) {
	array[a] = *loadfromconfig( list[a], sourcefile, sourceline );;
}


return array;
}

//And now a function to unload a setting from ConfigArray
int unloadfromconfig( char *name, char *sourcefile, int sourceline ) {
	ConfigArrayPtr pos;
	ConfigArrayPtr prev;
	ConfigArrayPtr next;

prev = NULL;
pos = ConfigList;

while( NULL != pos ) {
	next = pos->next;
	if( 0 == strcmp( name, pos->name ) ) {
		if (NULL == prev) {
			ConfigList = pos->next;
		} else {
			prev->next = next;
		}
		if( pos->string_value != NULL )
			free( pos->string_value );
		free( pos );
		return MHD_YES;
	} else {
	        prev = pos;
        }
	pos = next;
}

loghandle(LOG_ERR, 0, "Setting Not found in Array: \'%s\' ( Called from %s on line:%d )", name, sourcefile, sourceline);

return MHD_NO;
}

void FreeList( ConfigArrayPtr list ) {

for( ; list != NULL; list = list->next ) {
	if( list->string_value != NULL )
		free( list->string_value );
	free( list );
}

return;
}

void UnloadConfig() {

if( ConfigList ) {
	FreeList( ConfigList );
	ConfigList = NULL;
}

if( config != NULL ) {
	iniparser_freedict( config );
	config = NULL;
}


}

//And now to dump a default config to the location specified (since no file was found to exist).
void DumpDefaults( char *filename ) {
	int index, count;
	char **split;
	FILE *dumpfile = NULL;
	inikey ini;

ini = dictionary_new(0);
index = 0;
while( ( config_listing[index].origin != NULL ) ) {
	char *copy = strdup( config_listing[index].origin );
	split = str_split( copy, ':', &count );
	if( ( split == NULL ) || ( count < 2 ) ) {
		printf( "str_split returned NULL" );
		continue;
	}
	if( !( iniparser_find_entry(ini,split[0]) ) )
		iniparser_set( ini, split[0], NULL);
	iniparser_set( ini, config_listing[index].origin, config_listing[index].ifblank );
	free( copy );
	free( split );
	index++;
}

iniparser_set(ini,"admin:number", "2" );
iniparser_set(ini,"admin1", NULL );
iniparser_set(ini,"admin1:name", "admin" );
iniparser_set(ini,"admin1:password","password");
iniparser_set(ini,"admin1:faction", "Admins Faction" );
iniparser_set(ini,"admin1:forumtag", "<img src=\"/files?type=image&name=admin.gif\">" );
iniparser_set(ini,"admin1:level", "3" );
iniparser_set(ini,"admin1:race", "2" );
iniparser_set(ini,"admin2", NULL );
iniparser_set(ini,"admin2:name", "help" );
iniparser_set(ini,"admin2:password","password");
iniparser_set(ini,"admin2:faction", "Admins Helper" );
iniparser_set(ini,"admin2:forumtag", "Helper" );

iniparser_set(ini,"banned_ips",NULL);
iniparser_set(ini,"banned_ips:number", "3" );
iniparser_set(ini,"banned_ips:ip1", "10.0.0.*" );
iniparser_set(ini,"banned_ips:ip2","192.168.0.*");
iniparser_set(ini,"banned_ips:ip3", "127.0.0.1" );
iniparser_set(ini,"NEED_TO_DELETE_ME",NULL);

dumpfile = fopen( filename, "w");
if( !( dumpfile ) ) {
	printf( RED"Unable to load config file, and unable to spawn in specified location: \'%s\'\n"RESET, filename );
	dumpfile = fopen( TMPDIR"/evconfig.ini", "w");
	printf( RED"A default, non-usable version of the evsystem.ini has been dumped to: \'%s\'\n"RESET, TMPDIR"/evconfig.ini" );
	printf( RED"You will need to edit and move this file before you can run the server, its best if you use the config directory!\n"RESET );
} else {
	printf( RED"A default, non-usable version of the evsystem.ini has been dumped to: \'%s\'\n"RESET, filename );
	printf( RED"You must edit this file according to your needs before you run the game server!\n"RESET );
}
if( dumpfile ) {
	fprintf( dumpfile, "%s\n", "; NEctroverse Alpha Config file" );
	fprintf( dumpfile, "%s\n", "; Why did I change from config.h to config.ini ??" );
	fprintf( dumpfile, "%s\n", "; Simple, changes here can be implemented without a rebuild!" );
	fprintf( dumpfile, "%s\n", "; Just change and restart. Simple =D" );
	fprintf( dumpfile, "%s\n", "; -- Necro" );
	fprintf( dumpfile, "\n" );
	iniparser_dump_ini( ini, dumpfile );
	fprintf( dumpfile, "%s\n", ";Auto generated, You will need to delete the DELETE settings before this file will load.!" );
	fprintf( dumpfile, "%s\n", ";This file was automaticly generated as no ini file was present!" );
	fflush( dumpfile );
	fclose( dumpfile );
} else {
	printf( RED"Unable to spawn default config file into: \'%s\'\n"RESET, filename );
	printf( RED"You may have specified an invalid path...\n"RESET );	
}

iniparser_freedict( ini );
sysconfig.shutdown = true;


return;
}

