#include "loadini.h"
//Yep, so I don't like the way I was calling setting from the INI file... so to heck with it.
//I'm going to write a few new functions using a list table similar to HTML pages...
//Lets hope I don't make a mess of this one like I did the last hehe =P

ConfigListArrayDef config_listing[] = {
	{ INI_TYPE_STRING, "system:downfrom", "http://www.sknill.com/evbasic", "Download Source" },
	{ INI_TYPE_STRING, "syslog:facility", "LOG_LOCAL6", "Log Handle" },
	{ INI_TYPE_STRING, "syslog:tag", "EVServer", "Log Tag" },
	
	{ INI_TYPE_STRING, "system:name", "NEctroverse", "Server Name" },
	{ INI_TYPE_STRING, "system:cookiedomain", "", "Cookie Domain" },

	{ INI_TYPE_STRING, "system:directory", "/tmp/evcore/data", "Directory" },
	{ INI_TYPE_STRING, "system:httpimages", "/tmp/evcore/html/images", "HTTP Images" },
	{ INI_TYPE_STRING, "system:httpfiles", "/tmp/evcore/html/files", "HTTP Files" },
	{ INI_TYPE_STRING, "system:httpread", "/tmp/evcore/html/read", "HTTP Text" },
	{ INI_TYPE_STRING, "system:publicforum", "/tmp/evcore/data", "Public Forum" },

	{ INI_TYPE_INT, "system:http_port", "9990", "HTTP Port" },
	#if HTTPS_SUPPORT
	{ INI_TYPE_INT, "system:https_port", "9991", "HTTPS Port" },
	#endif

	{ INI_TYPE_INT, "system:notices", "5", "Display Notices" },
	{ INI_TYPE_INT, "system:stockpile", "0", "Stockpile" },
	{ INI_TYPE_INT, "system:auto_victory_afterticks", "0", "Auto Victory" },
	{ INI_TYPE_INT, "system:auto_endwar_afterticks", "0", "End Wars" },
	{ INI_TYPE_INT, "system:tick_time", "3600", "Tick Speed" },

	{ INI_TYPE_INT, "system:round", "0", "Round Number" },

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
	
	{ INI_TYPE_INT, "debug:create_accounts", "0", "Create Fakes" },
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

static int faultreported[3];

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

if( ( config_listing[index].origin == NULL) && ( bitflag( faultreported[0], ( index >> 16 ) ) == 0 ) ) {
	bitflag_add( &faultreported[0], ( index >> 16 ) );
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
	info( "A default, non-usable version of the ini file has been detected: %s", options.sysini );
	info( "You must edit this file before the game server is able to run correctly!");
	sysconfig.shutdown = true;
}

if( ( iniparser_find_entry(config, config_listing[index].origin ) == 0 ) && ( bitflag( faultreported[1], ( index >> 16 ) ) == 0 ) ) {
	bitflag_add( &faultreported[1], ( index >> 16 ) );
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
if( ( ( ConfigTable[index] != NULL ) && ( ConfigTable[index]->id == index ) ) && ( bitflag( faultreported[2], ( index >> 16 ) ) == 0 ) ) {
	bitflag_add( &faultreported[2], ( index >> 16 ) );
	errno = 0;
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

//  -- This is ONLY for cmd.c Admin creation --
//Fetch list of Admin details needed for account creation, number was already loaded and so it's just passed in
//Rember that returns of this function need to be free'd once finished with 
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
	strcpy( array[a].faction, iniparser_getstring(config, buffer, "" ) );
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

//Hmmz, I'm so tired of calling settings one-by-one... so here's that listing function
//Yer, this was cause of map generation... it needs a fair few settings at once, and I can either call them again and again
//... Or, make one list and just use them over and over, just remember to free any usage of this function.
ConfigArrayPtr makelistfromconfig( char **list, char *sourcefile, int sourceline ) {
	ConfigArrayPtr array;
	ConfigArrayPtr fetch;
	int a, count;

count = 0;
for( a = 0; list[a]; a++ ) {
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
	fetch = loadfromconfig( list[a], sourcefile, sourceline );
	memcpy( &array[a], fetch, sizeof(ConfigArrayDef) );
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

FreeList( ConfigList );

if( config != NULL ) {
	iniparser_freedict( config );
	config = NULL;
}

}





//FIXME -- ADD INTO FUNCTION ( And make it cleaner... eh =P )


/*
if( firstload ) {
	FILE *dumpfile;
	if( !( iniparser_find_entry(ini,"system") ) ){
		iniparser_set(ini,"system",NULL);
		iniparser_set(ini,"system:name",sysconfig.servername);
		iniparser_set(ini,"system:cookiedomain","yourdomain.com");
		iniparser_set(ini,"system:directory",GetSetting( config, "Directory", false ));
		iniparser_set(ini,"system:downfrom",GetSetting( config, "Download Source", false ));
		iniparser_set(ini,"system:httpimages",sysconfig.httpimages);
		iniparser_set(ini,"system:httpfiles",sysconfig.httpfiles);
		iniparser_set(ini,"system:httpread",sysconfig.httpread);
		iniparser_set(ini,"system:pubforum",sysconfig.pubforum);
		iniparser_set_string(ini,"system:http_port","%d", sysconfig.httpport );
		#if HTTPS_SUPPORT
		iniparser_set_string(ini,"system:https_port","%d", sysconfig.httpsport );
		#endif
		iniparser_set(ini,"system:stockpile","14");
		iniparser_set(ini,"system:auto_victory_afterticks","52");
		iniparser_set(ini,"system:auto_endwar_afterticks","26");
		iniparser_set(ini,"system:tick_time","3600");
		iniparser_set(ini,"system:notices","5");
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
		iniparser_set_string(ini,"admin_empire:number", "%d", admincfg.empire );
		iniparser_set(ini,"admin_empire:name", admincfg.ename );
		iniparser_set(ini,"admin_empire:password", admincfg.epassword );
		iniparser_set(ini,"admin_empire:ommit_from_rank", admincfg.rankommit ? "true" : "false" );
	}
	if( !( iniparser_find_entry(ini,"map") ) ){
		iniparser_set(ini,"map",NULL);
		iniparser_set_string(ini,"map:sizex", "%d", mapcfg.sizex );
		iniparser_set_string(ini,"map:systems", "%d", mapcfg.systems );
		iniparser_set_string(ini,"map:families", "%d", mapcfg.families );
		iniparser_set_string(ini,"map:members_perfamily", "%d", mapcfg.fmembers );
		iniparser_set_string(ini,"map:border", "%d", mapcfg.border );
		iniparser_set_string(ini,"map:anglevar", "%d", mapcfg.anglevar );
		iniparser_set_string(ini,"map:linknum", "%d", mapcfg.linknum );
		iniparser_set_string(ini,"map:linkradius", "%d", mapcfg.linkradius );
		iniparser_set_string(ini,"map:lenghtbase", "%d", mapcfg.lenghtbase );
		iniparser_set_string(ini,"map:lenghtvar", "%d", mapcfg.lenghtvar );
		for(a = 0; a < CMD_BONUS_NUMUSED; a++) {
			sprintf(DIRCHECKER,"map:%s",cmdBonusName[a]);
			for(i = 0; DIRCHECKER[i]; i++){
				DIRCHECKER[i] = tolower(DIRCHECKER[i]);
			}
			iniparser_set_string(ini,DIRCHECKER, "%d", (rand() % 35) );
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
		loghandle(LOG_CRIT, errno, "Unable to spawn default config file into: \'%s\'",file);
		loghandle(LOG_CRIT, false, "%s", "You may have specified an invalid path...");	
	}
	sysconfig.shutdown = true;
}
*/



