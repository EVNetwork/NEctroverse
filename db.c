#ifndef GLOBALINCLUDED
#include "config/global.h"
#endif

enum 
{
DB_FILE_USERS,
DB_FILE_MAP,
DB_FILE_MARKET,
DB_FILE_FORUM,
DB_FILE_TOTAL,
};

char dbFileUsersName[] = "%s/userdb";
char dbFileMapName[] = "map";
char dbFileMarketName[] = "market";
char dbFileForumName[] = "%s/forums";

char *dbFileList[DB_FILE_TOTAL] = { dbFileUsersName, dbFileMapName, dbFileMarketName, dbFileForumName };
FILE *dbFilePtr[DB_FILE_TOTAL];

char *dbImageDirs[HTTP_DIR_TOTAL] = {
"%s",
"%s/avatars",
"%s/filetypes",
"%s/ostypes",
"%s/smilies",
};

enum 
{
DB_FILE_USER_INFO,
DB_FILE_USER_MAIN,
DB_FILE_USER_BUILD,
DB_FILE_USER_PLANETS,
DB_FILE_USER_FLEETS,
DB_FILE_USER_NEWS,
DB_FILE_USER_MARKET,
DB_FILE_USER_MAILIN,
DB_FILE_USER_MAILOUT,
DB_FILE_USER_SPECOPS,
DB_FILE_USER_RECORD,
DB_FILE_USER_FLAGS,

DB_FILE_USER_TOTAL,
};



char dbFileUserInfoName[] = "%s/user%d/info";
char dbFileUserMailInName[] = "%s/user%d/mailin";
char dbFileUserMailOutName[] = "%s/user%d/mailout";
char dbFileUserRecordName[] = "%s/user%d/record";

char dbFileUserMainName[] = "%s/user%d/main";
char dbFileUserBuildName[] = "%s/user%d/build";
char dbFileUserPlanetsName[] = "%s/user%d/planets";
char dbFileUserFleetsName[] = "%s/user%d/fleets";
char dbFileUserNewsName[] = "%s/user%d/news";
char dbFileUserMarketName[] = "%s/user%d/market";
char dbFileUserSpecOpsName[] = "%s/user%d/specops";
char dbFileUserGameFlags[] = "%s/user%d/flags";

char *dbFileUserList[DB_FILE_USER_TOTAL] = { dbFileUserInfoName, dbFileUserMainName, dbFileUserBuildName, dbFileUserPlanetsName, dbFileUserFleetsName, dbFileUserNewsName, dbFileUserMarketName, dbFileUserMailInName, dbFileUserMailOutName, dbFileUserSpecOpsName, dbFileUserRecordName, dbFileUserGameFlags };

int64_t dbFileUserListDat0[] = { 0, -1, -1, 0, 0 };
int64_t dbFileUserListDat1[] = { 0, 8 };

int dbFileUserListBase[DB_FILE_USER_TOTAL] = { 0, 0, 4, 4, 4, 40, 8, 8, 8, 4, 4, 0 };
int64_t *dbFileUserListData[DB_FILE_USER_TOTAL] = { 0, 0, dbFileUserListDat0, dbFileUserListDat0, dbFileUserListDat0, dbFileUserListDat0, dbFileUserListDat0, dbFileUserListDat1, dbFileUserListDat1, dbFileUserListDat0, dbFileUserListDat0, dbFileUserListDat0 };


int dbMapBInfoStatic[MAP_TOTAL_INFO];

int dbRegisteredInfo[DB_TOTALS_USERS_NUMUSED];

int dbArtefactPos[ARTEFACT_NUMUSED];

int dbArtefactMax;



/*
Database format

users
  4:next user ID
  4:number of free IDs
4*X:list of free IDs

map
  4:size X
  4:size Y
  4:number of systems
  4:number of planets
  4:number of families
  4:number of players per empire
  4:reserved
 32:reserved
struct ( 20 )
  4:position ( y << 16 ) + x
  4:index first planet
  4:number of planets
  4:empire home system, -1:none
  4:reserved
struct ( 184 )
  4:system indice
  4:position, ( y << 20 ) + ( x << 8 ) + planet
  4:planet owner ID, none if < 0
  4:size
  4:flags
  4:population
  4:maxpopulation
3*4:special
 64:number of buildings
 64:number of units
  4:construction
  4:protection
  4:surrender
  4:reserved
struct ( 336 )
  4:number of players
128:players ID, 32 fixed maximum
  4:home system ID
  4:home system position ( y << 16 ) + x
 64:empire name
  4:leader ID
 32:vote index in players IDs
  4:picture mime
  4:picture time
  4:planets
  4:networth
  4:artefacts
  4:rank
 72:reserved

market
   4:num max bids
   4:list free
3 * 2 * 80 * 12 ; res*action*prices*struct
struct( 12 )
   4:total of bids
   4:first list ID
   4:last list ID
struct( 16 )
   4:previous id
   4:next id
   4:quantity
   4:owner ID


user%d/user
  4:user ID
  4:level
  4:flags
  4:reserved
 32:name
 32:password

user%d/main
 nn:dbUserMainDef

user%d/build
  4:number of builds
struct ( 20 )
  4:type, |0x10000 if units
  4:quantity
  4:time remaining
  4:planet ID
  4:planet position, ( y << 20 ) + ( x << 8 ) + planet
  4*8:Cost

user%d/planets
  4:number of planets
struct ( 20 )
  4:planet index
  4:system index
  4:location, ( y << 20 ) + ( x << 8 ) + planet
  4:flags, &1:portal
  4:reserved

user%d/fleets
  4:number of fleets
struct ( 96 )
 64:units
  4:order
  4:destination, ( y << 20 ) + ( x << 8 ) + planet
  4:destination planet ID
  4:destination system ID
  4:source, ( y << 20 ) + ( x << 8 )
  4:flags
  4:time
  8:reserved

user%d/news
  8:num
  8:list used
  8:list free
  8:news flags
  8:new num
struct ( 128 )
  8:previous news id
  8:next news id
  8:time
  8:flags
224:news data

user%d/market
  4:number of bids
  4:flags
struct ( 20 )
  4:action
  4:resource
  4:price
  4:quantity
  4:bid ID

user%d/specops
  4:number of specops
struct ( nn )
 nn:dbUserSpecOpDef


fam%d/news
  4:num
  4:list used
  4:list free
  4:news flags
  4:new num
struct ( 128 )
  4:previous news id
  4:next news id
  4:time
  4:flags
112:news data

And Mal forgot the mail file
....


*/


FILE *dbFileGenOpen( int num ) {
	char fname[PATH_MAX];
	char COREDIR[PATH_MAX];
	ConfigArrayPtr settings;
	
settings = GetSetting( "Directory" );
if(num == DB_FILE_USERS) {
	snprintf( COREDIR, sizeof(COREDIR), "%s/users", settings->string_value );
	sprintf(fname, dbFileList[num], COREDIR);
} else {
	snprintf( COREDIR, sizeof(COREDIR), "%s/data", settings->string_value );
	sprintf(fname, dbFileList[num], COREDIR);
}
if( dbFilePtr[num] ) {
	return dbFilePtr[num];
}
   
if( !( dbFilePtr[num] = fopen( fname, "rb+" ) ) ) {
	error( "Opening File: %s", fname );
	return 0;
}

return dbFilePtr[num];
}

void dbFileGenClose( int num )
{
  if( dbFilePtr[num] )
  {
    fclose( dbFilePtr[num] );
    dbFilePtr[num] = 0;
  }
  return;
}

void dbFlush()
{
  int a;
  for( a = 0 ; a < DB_FILE_TOTAL ; a++ )
    dbFileGenClose( a );
  return;
}


FILE *dbFileUserOpen( int id, int num ) {
	char fname[PATH_MAX];
	char COREDIR[PATH_MAX];
	ConfigArrayPtr settings;
	FILE *file;

settings = GetSetting( "Directory" );
if((num&0xFFFF) == DB_FILE_USER_INFO) {
	snprintf( COREDIR, sizeof(COREDIR), "%s/users", settings->string_value );  
	snprintf( fname, sizeof(fname), dbFileUserList[num&0xFFFF], COREDIR, id );
} else {
	snprintf( COREDIR, sizeof(COREDIR), "%s/data", settings->string_value );  
  	snprintf( fname, sizeof(fname), dbFileUserList[num&0xFFFF], COREDIR, id );
}
  
if( !( file = fopen( fname, "rb+" ) ) ) {
		// mooooooo
	if( ( file = fopen( fname, "wb" ) ) ) {
		file_w( dbFileUserListData[num&0xFFFF], 1, dbFileUserListBase[num&0xFFFF], file );
		fclose( file );
		return fopen( fname, "rb+" );
	}

	if( num < 0x10000 ) {
		error( "Opening File: %s", fname);
	}

	return 0;
}


return file;
}

FILE *dbFileFamOpen( int id, int num )
{
  int a, b;
  char fname[PATH_MAX];
  FILE *file;
  snprintf( fname, sizeof(fname), "fam%02dn%02d", id, num );
  if( !( file = fopen( fname, "rb+" ) ) )
  {
    if( ( file = fopen( fname, "wb+" ) ) )
    {
      if( num == 0 )
      {
        a = 0;
        file_w( &a, 1, sizeof(int), file );
      }
      else
      {
        a = 0;
        for( b = 0 ; b < 4096*2/4 ; b++ )
          file_w( &a, 1, sizeof(int), file );
      }
      file_s( file, 0 );
      return file;
    }
	if( num < 0x10000 ) {
		error( "Opening File: %s", fname);
	}
    return 0;
  }
  return file;
}




dbUserPtr dbUserList;
dbUserPtr dbUserTable[ARRAY_MAX];



dbUserPtr dbUserAllocate( int id ) {
	dbUserPtr user;

if( !( user = malloc( sizeof(dbUserDef) ) ) ) {
	critical("Database Malloc Failed");
	return 0;
}

memset( user, 0, sizeof(dbUserDef) );
user->prev = (void **)&(dbUserList);
user->next = dbUserList;
if( dbUserList )
	dbUserList->prev = &(user->next);
dbUserList = user;
user->id = id;
user->lasttime = time(NULL);
dbUserTable[id] = user;

return user;
}

void dbUserFree( dbUserPtr user ) {
	dbUserPtr next;

if( (unsigned int)user->id >= ARRAY_MAX )
	return;

dbUserTable[user->id] = 0;

next = user->next;
*(user->prev) = (void *)next;
if( next )
	next->prev = user->prev;
free( user );

return;
}


/*
users
  4:next user ID
  4:number of free IDs
4*X:list of free IDs
*/
int dbInitUsersReset()
{
  int a, last, freenum;
  //dbUserMainDef maind;
  FILE *file;

  if( !( dbFileGenOpen( DB_FILE_USERS ) ) )
    return -3;
  last = 0;
  for( a = 0 ; a < ARRAY_MAX ; a++ )
  {
    if( !( file = dbFileUserOpen( a, 0x10000 | DB_FILE_USER_MAIN ) ) )
      continue;
    last = a;
    fclose( file );
  }

  file_s( dbFilePtr[DB_FILE_USERS], 8 );
  freenum = 0;
  for( a = 0 ; a <= last ; a++ )
  {
    if( ( file = dbFileUserOpen( a, 0x10000 | DB_FILE_USER_MAIN ) ) )
    {
      fclose( file );
      continue;
    }
    file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
    freenum++;
  }

  file_s( dbFilePtr[DB_FILE_USERS], 0 );
  last++;
  file_w( &last, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );

  file_s( dbFilePtr[DB_FILE_USERS], 4 );
  file_w( &freenum, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );

  return 1;
}



int dbInit() {
	int a, b;
	int array[4];
	time_t now;
	dbUserPtr user;
	dbUserInfoDef infod;
	dbMainPlanetDef planetd;
	dbForumForumDef forumd;
	ConfigArrayPtr settings[2];
	FILE *file;
	char fname[PATH_MAX];
	char COREDIR[PATH_MAX];

time( &now );
settings[0] = GetSetting( "Directory" );
settings[1] = GetSetting( "Public Forum" );
snprintf( COREDIR, sizeof(COREDIR), "%s/data", settings[0]->string_value );
if( chdir( COREDIR ) == -1 ) {
	critical( "Change DIR" );
	return 0;
}

if( ( dbMapRetrieveMain( dbMapBInfoStatic ) < 0 ) )
	return 0;

if( !( dbFilePtr[DB_FILE_MARKET] = fopen( dbFileList[DB_FILE_MARKET], "rb+" ) ) ) {
	info( "Market database not found, creating..." );

	if( !( dbFilePtr[DB_FILE_MARKET] = fopen( dbFileList[DB_FILE_MARKET], "wb+" ) ) ) {
		critical( "Error, could not create market database!" );
		return 0;
	}

	file_s( dbFilePtr[DB_FILE_MARKET], 0 );
	array[0] = 0;
	array[1] = -1;
	file_w( array, 1, 2*sizeof(int), dbFilePtr[DB_FILE_MARKET] );

	array[0] = 0;
	array[1] = -1;
	array[2] = -1;
	for( a = 0 ; a < 6*DB_MARKET_RANGE ; a++ )
		file_w( array, 1, 3*sizeof(int), dbFilePtr[DB_FILE_MARKET] );
}

snprintf( COREDIR, sizeof(COREDIR), "%s/data/forums", settings[0]->string_value );
if( !( dbFilePtr[DB_FILE_FORUM] = fopen( COREDIR, "rb+" )  ) ) {
	info("Forum database not found, creating...");
	
	if( !( dbFilePtr[DB_FILE_FORUM] = fopen( COREDIR, "wb+" ) ) ) {
		critical( "Error, could not create forum database!" );
		return 0;
	}
	a = 0;
	file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_FORUM] );
	forumd.threads = 0;
	forumd.time = 0;
	forumd.tick = 0;
	forumd.flags = 0;
	for( a = 0 ; a < dbMapBInfoStatic[MAP_EMPIRES] ; a++ ) {
		sprintf( forumd.title, "Empire %d forum", a );
		forumd.rperms = 2;
		forumd.wperms = 2;
		forumd.flags = DB_FORUM_FLAGS_FORUMFAMILY;
		dbForumAddForum( true, a, &forumd );
	}
	info( "Created Forums for %d Empires.", a );
}


snprintf( COREDIR, sizeof(COREDIR), "%s/forums", settings[1]->string_value );
if( !( file = fopen( COREDIR, "rb+" ) ) ) {
	info( "Public Forum database not found, creating..." );
	if( !( file = fopen( COREDIR, "wb+" ) ) ) {
		critical( "Error, could not create public forum database!" );
		return 0;
	}
	a = 0;
	file_w( &a, 1, sizeof(int), file );
}
fclose( file );

snprintf( COREDIR, sizeof(COREDIR), "%s/users", settings[0]->string_value );
sprintf( fname, dbFileList[DB_FILE_USERS], COREDIR );
if( !( dbFilePtr[DB_FILE_USERS] = fopen( fname, "rb+" ) ) ) {
	info( "User database not found, creating..." );

	if( !( dbFilePtr[DB_FILE_USERS] = fopen( fname, "wb+" ) ) ) {
		critical( "Error, could not create user database!" );
		return 0;
	}
	file_s( dbFilePtr[DB_FILE_USERS], 0 );
	a = 0;
	file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
	file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
	return 1;
} else {
	dbInitUsersReset();
}


file_s( dbFilePtr[DB_FILE_USERS], 0 );
file_r( &b, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
dbRegisteredInfo[DB_TOTALS_USERS_REGISTERED] = b;
dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] = 0;
dbRegisteredInfo[DB_TOTALS_USERS_ONLINE] = 0;
for( a = 0 ; a < b ; a++ ) {
	if( !( file = dbFileUserOpen( a, 0x10000 | DB_FILE_USER_INFO ) ) ) {
		error( "User %d Open", a );
		continue;
	}
	if( !( user = dbUserAllocate( a ) ) ) {
		fclose( file );
		continue;
	}

	if( !(	dbUserInfoRetrieve( a, &infod ) ) ) {
	 	error( "Failure reading file for user; %d", a );
	}
	user->level = infod.level;
	user->flags = infod.flags;
	user->lasttime = infod.lasttime;
	dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] += bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED );
	dbRegisteredInfo[DB_TOTALS_USERS_ONLINE] += ( (now - user->lasttime) < (SESSION_TIME / 4) );
	#if FACEBOOK_SUPPORT
	if( bitflag( user->flags, CMD_USER_FLAGS_FBLINK ) )
		strncpy( user->fbid, infod.fbinfo.id, sizeof(user->fbid) );
	#endif
	strncpy( user->name, infod.name, sizeof(user->name) );
	strncpy( user->faction, infod.faction, sizeof(user->faction) );
	strncpy( user->forumtag, infod.forumtag, sizeof(user->forumtag) );
	strncpy( user->http_session, infod.http_session, sizeof(user->http_session) );
	fclose( file );
}
ticks.uregist = dbRegisteredInfo[DB_TOTALS_USERS_REGISTERED];
ticks.uactive = dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED];
ticks.uonline= dbRegisteredInfo[DB_TOTALS_USERS_ONLINE];

dbFlush();

for( a = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ ) {
	dbMapRetrievePlanet( a, &planetd );
	if( ( b = (int)artefactPrecense( &planetd ) ) < 0 )
		continue;
	dbArtefactPos[b] = planetd.position;
}
	

return 1;
}


void dbEnd() {
	dbUserPtr user, next;
dbFlush();
for( user = dbUserList ; user ; user = next ) {
	next = user->next;
	dbUserFree( user );
}

return;
}




// Quick map search

int dbMapFindSystem( int x, int y ) {
	dbMainSystemDef systemd;
	int a, position = ( y << 16 ) + x;

for( a = 0 ; a < dbMapBInfoStatic[MAP_SYSTEMS] ; a++ ) {
	dbMapRetrieveSystem( a, &systemd );
	if( systemd.position == position )
		return a;
}

return -1;
}

int dbMapFindValid( int x, int y ) {
	int binfo[MAP_TOTAL_INFO];
	
dbMapRetrieveMain( binfo );
if( (unsigned int)x >= binfo[MAP_SIZEX] )
	return 0;
if( (unsigned int)y >= binfo[MAP_SIZEY] )
	return 0;

return 1;
}




// Users functions
int dbUserSearch( char *name ) {
	dbUserPtr user;

for( user = dbUserList ; user ; user = user->next ) {
	if( !( ioCompareExact( name, user->name ) ) )
		continue;
	//info( "Name: %s links to ID:%d", name, user->id );
	return user->id;
}

return -1;
}
#if FACEBOOK_SUPPORT
int dbUserFBSearch( char *FBid ) {
	dbUserPtr user;

for( user = dbUserList ; user ; user = user->next ) {
	if( !( bitflag( user->flags, CMD_USER_FLAGS_FBLINK) ) || !( ioCompareExact( FBid, user->fbid ) ) )
		continue;

	return user->id;
}

return -1;
}
#endif

// Users functions
int dbUserSessionSearch( char *session ) {
	dbUserPtr user;

for( user = dbUserList ; user ; user = user->next ) {
	if( !( ioCompareExact( session, user->http_session ) ) )
		continue;
	return user->id;
}

return -1;
}


int dbUserSearchFaction( char *name ) {
	dbUserPtr user;

for( user = dbUserList ; user ; user = user->next ) {
	if( !( ioCompareExact( name, user->faction ) ) )
		continue;

	return user->id;
}

return -1;
}

dbUserPtr dbUserLinkID( int id ) {

if( ( id >= ARRAY_MAX ) || ( id < 0 ) )
	return 0;

return dbUserTable[id];
}


int dbUserAdd( dbUserInfoPtr adduser ) {
	int a, id, freenum;
	char dname[PATH_MAX], fname[PATH_MAX], uname[PATH_MAX];
	char COREDIR[PATH_MAX];
	ConfigArrayPtr settings;
	dbUserPtr user;
	FILE *file;


if( !( dbFileGenOpen( DB_FILE_USERS ) ) )
	return -3;

file_s( dbFilePtr[DB_FILE_USERS], 4 );
file_r( &freenum, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );


if( !( freenum ) ) {
	file_s( dbFilePtr[DB_FILE_USERS], 0 );
	file_r( &id, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
} else {
	a = freenum - 1;
	file_s( dbFilePtr[DB_FILE_USERS], 8 + ( a << 2 ) );
	file_r( &id, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
}
	
if( !( user = dbUserAllocate( id ) ) ) {
	return -3;
}
  
//create both folder for player
settings = GetSetting( "Directory" );
sprintf( dname, "%s/data/user%d", settings->string_value, id );
sprintf( uname, "%s/users/user%d", settings->string_value, id );
if( mkdir( dname, S_IRWXU ) == -1 ) {
	dbUserFree( user );
	error( "mkdir %s\n", dname );
	return -3;
}  
if( mkdir( uname, S_IRWXU ) == -1 ) {
	dbUserFree( user );
	error( "mkdir %s\n", dname );
	return -3;
}  

sprintf( user->name, "%s", adduser->name );
sprintf( user->faction, "%s", adduser->faction );
sprintf( user->forumtag, "%s", adduser->forumtag );
user->lasttime = adduser->lasttime;
user->id = id;
user->flags = 0;
user->level = 0;
  
//Create a db Database in the db other server
for( a = DB_FILE_USER_TOTAL-1 ;  ; a-- ) {
	snprintf( COREDIR, sizeof(COREDIR), "%s/data", settings->string_value );
  	snprintf( fname, sizeof(fname), dbFileUserList[a], COREDIR, id );
    
	if( !( file = fopen( fname, "wb+" ) )) {
		dbUserFree( user );
		rmdir( dname );
		rmdir( uname );
		error( "fopen dbuseradd" );
		return -3;
	}
	if( a == 0 )
		break;
	if( dbFileUserListBase[a] )
		file_w( dbFileUserListData[a], 1, dbFileUserListBase[a], file );
	fclose( file );
}
adduser->id = user->id;
file_w( adduser, 1, sizeof(dbUserInfoDef), file );
fclose( file );
  
//Create a user Database in the db 10Min server
for( a = DB_FILE_USER_TOTAL-1 ;  ; a-- ) {
	snprintf( COREDIR, sizeof(COREDIR), "%s/users", settings->string_value );
  	snprintf( fname, sizeof(fname), dbFileUserList[a], COREDIR, id );

	if( !( file = fopen( fname, "wb+" ) )) {
		dbUserFree( user );
		rmdir( dname );
		rmdir( uname );
		error( "User Creation" );
		return -3;
	}
	if( a == 0 )
		break;
	if( dbFileUserListBase[a] )
		file_w( dbFileUserListData[a], 1, dbFileUserListBase[a], file );
	fclose( file );
}

file_w( adduser, 1, sizeof(dbUserInfoDef), file );
fclose( file );

if( !( freenum ) ) {
	file_s( dbFilePtr[DB_FILE_USERS], 0 );
	a = id + 1;
	file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
} else {
	a = freenum - 1;
	file_s( dbFilePtr[DB_FILE_USERS], 4 );
	file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
}

return id;
}


int dbUserRemove( int id ) {
	int a;
	char COREDIR[PATH_MAX];
	char dname[PATH_MAX], fname[PATH_MAX];
	ConfigArrayPtr settings;
	dbUserPtr user;


if( !( user = dbUserLinkID( id ) ) )
	return 0;
if( !( dbFileGenOpen( DB_FILE_USERS ) ) )
	return 0;

dbUserFree( user );

file_s( dbFilePtr[DB_FILE_USERS], 4 );
file_r( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
a++;
file_s( dbFilePtr[DB_FILE_USERS], 4 );
file_w( &a, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );
file_s( dbFilePtr[DB_FILE_USERS], ( a + 1 ) << 2 );
file_w( &id, 1, sizeof(int), dbFilePtr[DB_FILE_USERS] );

settings = GetSetting( "Directory" );
for( a = 0 ; a < DB_FILE_USER_TOTAL ; a++ ) {
	snprintf( COREDIR, sizeof(COREDIR), "%s/users", settings->string_value );
	snprintf( fname, sizeof(fname), dbFileUserList[a], COREDIR, id );
	unlink( fname );
}

for( a = 0 ; a < DB_FILE_USER_TOTAL ; a++ ) {
	snprintf( COREDIR, sizeof(COREDIR), "%s/data", settings->string_value );
	snprintf( fname, sizeof(fname), dbFileUserList[a], COREDIR, id );
	unlink( fname );
}

sprintf( dname, "%s/users/user%d", settings->string_value, id );
rmdir( dname );
sprintf( dname, "%s/data/user%d", settings->string_value, id );
rmdir( dname );


return 1;
}


int dbUserSave( int id, dbUserPtr user ) {
//	FILE *file;
	dbUserInfoDef uinfo;

if( !( dbUserInfoRetrieve( id, &uinfo ) ) ) {
	error( "Error in user save, getting real info" );
	return -3;
}

uinfo.id = user->id;
uinfo.level = user->level;
strcpy(uinfo.name,user->name);
uinfo.flags = user->flags;
strcpy(uinfo.http_session,user->http_session);
uinfo.lasttime = user->lasttime;
#if FACEBOOK_SUPPORT
if( bitflag( user->flags, CMD_USER_FLAGS_FBLINK ) ) {
	strcpy( uinfo.fbinfo.id, user->fbid );
} else {
	memset( &user->fbid, 0, sizeof(user->fbid) );
	memset( &uinfo.fbinfo, 0, sizeof(FBUserDef) );
}
#endif
if( !( dbUserInfoSet( id, &uinfo ) ) ) {
	error( "Error in user save, getting setting info" );
	return -3;
}

return 1;
}

int dbUserSetPassword( int id, char *pass ) {
	dbUserInfoDef uinfo;

if( !( dbUserInfoRetrieve( id, &uinfo ) ) ) {
	error( "Error in user save, getting real info" );
	return -3;
}

sprintf( uinfo.password, "%s", hashencrypt(pass) );

if( !( dbUserInfoSet( id, &uinfo ) ) ) {
	error( "Error in user save, getting setting info" );
	return -3;
}

return 1;
}

int dbUserRetrievePassword( int id, char *pass ) {
	dbUserInfoDef uinfo;

dbUserInfoRetrieve( id, &uinfo );
sprintf(pass, "%s", uinfo.password);

return 1;
}


int dbUserLinkDatabase( void *cnt, int id ) {
	dbUserPtr user;
	ReplyDataPtr cnt2 = cnt;

if( id < 0 ) {
	(cnt2->session)->dbuser = 0;
	return 1;
}

if( !( user = dbUserLinkID( id ) ) )
	return -2;

(cnt2->session)->dbuser = user;
user->lasttime = time( 0 );

return 1;
}



// session functions

int dbSessionSet( dbUserPtr user, char *session ) {
	dbUserInfoDef uinfo;


if( !( user ) )
	return -3;

strncpy( user->http_session, session, sizeof(uinfo.http_session) );

if( dbUserSave( user->id, user ) < 0 ) {
	error( "Syncing user #%d session", user->id );
	return -1;
}

return 1;
}

int dbSessionRetrieve( dbUserPtr user, char *session ) {
	dbUserInfoDef uinfo;

if( !( user ) )
	return -3;
if( !( dbUserInfoRetrieve( user->id, &uinfo ) ) ) {
	error("Error in user save, getting real info");
	return -3;
}

strncpy( session, user->http_session, sizeof(uinfo.http_session) );

return 1;
}




// user main data functions

int dbUserMainSet( int id, dbUserMainPtr maind ) {
	dbUserPtr user;
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_MAIN ) ) ) {
	error( "User Open" );
	return -3;
}

file_w( maind, 1, sizeof(dbUserMainDef), file );
fclose( file );

if( !( user = dbUserLinkID( id ) ) )
	return -3;

sprintf( user->faction, "%s", maind->faction );

return 1;
}



int dbUserMainRetrieve_real( int id, dbUserMainPtr maind, char *sourcefile, int sourceline ) {
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_MAIN ) ) ) {
	error( "User Open from file %s on line %d", sourcefile, sourceline );
	return -3;
}

memset( maind, 0, sizeof(dbUserMainDef) );
file_r( maind, 1, sizeof(dbUserMainDef), file );

fclose( file );

return 1;
};





// user build functions -- Need to change the calling of this function.

int dbUserBuildAdd( int id, int type, int64_t *cost, int quantity, int time, int plnid, int plnloc ) {
	int pos, i;
	FILE *file;
	dbUserBuildDef buildp;

if( !( memset( &buildp, 0, sizeof(dbUserBuildDef) ) ) ) {
	return -3;
}
if( !( file = dbFileUserOpen( id, DB_FILE_USER_BUILD ) ) ) {
	error( "User Open" );
	return -3;  
}

buildp.type = type;
buildp.quantity = quantity;
buildp.time = time;
buildp.plnid = plnid;
buildp.plnpos = plnloc;


for(i=0;buildp.cost[i];i++)
	buildp.cost[i] = cost[i];

file_r( &pos, 1, sizeof(int), file );
file_s( file, 4+(pos*sizeof(dbUserBuildDef)) );
file_w( &buildp, 1, sizeof(dbUserBuildDef), file );
file_s( file, 0 );
pos++;
file_w( &pos, 1, sizeof(int), file );
fclose( file );

return pos;
}


int dbUserBuildRemove( int id, int bldid ) {
	int a, num, data[sizeof(dbUserBuildDef)];
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_BUILD ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &num, 1, sizeof(int), file );

if( (unsigned int)bldid >= num ) {
	fclose( file );
	return -2;
}

if( bldid+1 < num ) {
	file_s( file, 4+(num*sizeof(dbUserBuildDef))-sizeof(dbUserBuildDef) );
	file_r( data, 1, sizeof(dbUserBuildDef), file );
	file_s( file, 4+(bldid*sizeof(dbUserBuildDef)) );
	file_w( data, 1, sizeof(dbUserBuildDef), file );
}

file_s( file, 0 );
a = num-1;
file_w( &a, 1, sizeof(int), file );
fclose( file );

return 1;
}


int dbUserBuildList( int id, dbUserBuildPtr *build ) {
	int a, num;
	dbUserBuildPtr buildp;
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_BUILD ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &num, 1, sizeof(int), file );

if( !( buildp = malloc( num*sizeof(dbUserBuildDef) ) ) ) {
	fclose( file );
	return -1;
}

for( a = 0 ; a < num ; a++ ) {





	file_s( file, 4+(a*sizeof(dbUserBuildDef)) );
	file_r( &buildp[a], 1, sizeof(dbUserBuildDef), file );
}
fclose( file );
*build = buildp;

return num;
}


void sortlist ( int num, int *list1, int *list2 )
{
  int b, c, smaller;
  int *list3;
  
  if( !( list3 = malloc( (num)*sizeof(int) ) ) )
    return;
  
  for( b = 0 ; b < num ; b++ )
    {
      smaller = 0;
//      list3[0] = list1[0];
      for ( c = 0 ; c < num; c++ )
	{
	  if ( b != c )
	    {
	      if ( ( list2[b] > list2[c] ) || ( ( list2[b] == list2[c]) && ( list1[b] > list1[c] ) ) )
		smaller++;
	    }
	}
      list3[smaller] = list1[b];
    }
  
  for( b = 0 ; b < num ; b++ )
    list1[b] = list3[b];

//info( "Dit is void sortlist" );
  free (list3);
  return;

}

void sortlist2 ( int num, int *list1, int *list2, int *list3 )
{
  int b, c, smaller;
  int *list4;
  
  if( !( list4 = malloc( (num)*sizeof(int) ) ) )
    return;
  
  for( b = 0 ; b < num ; b++ )
    {
      smaller = 0;
      //list4[0] = list1[0];
      for ( c = 0 ; c < num; c++ )
	{
	  if ( b != c )
	    {
	      if ( ( list2[b] > list2[c] ) || ( ( list2[b] == list2[c]) && ( list3[b] > list3[c] ) ) || ( ( list2[b] == list2[c]) && ( list3[b] == list3[c] ) && ( list1[b] > list1[c] ) ) )
		smaller++;
	    }
	}
      list4[smaller] = list1[b];
    }
  
  for( b = 0 ; b < num ; b++ )
    list1[b] = list4[b];
  
//info("dees is void sortlist2" );
  free (list4);
  return;
}

int dbUserBuildListReduceTime( int id, dbUserBuildPtr *build ) {
	int a, num;
	FILE *file;
	dbUserBuildPtr buildp;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_BUILD ) ) ) {
	error( "User Open" );
	return -3;
}

file_r( &num, 1, sizeof(int), file );

if( !( buildp = malloc( num*sizeof(dbUserBuildDef) ) ) ) {
	fclose( file );
	return -1;
}

for( a = 0 ; a < num ; a++ ) {
	file_s( file, 4+(a*sizeof(dbUserBuildDef)) );
	file_r( &buildp[a], 1, sizeof(dbUserBuildDef), file );
	buildp[a].time--;
	file_s( file, 4+(a*sizeof(dbUserBuildDef)) );
	file_w( &buildp[a], 1, sizeof(dbUserBuildDef), file );
}
fclose( file );
*build = buildp;

return num;
}

int dbUserBuildEmpty( int id ) {
	int a;
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_BUILD ) ) ) {
	error( "User Open" );
	return -3;
}

a = 0;
file_w( &a, 1, sizeof(int), file );
fclose( file );

return 1;
}






// user planets functions

int dbUserPlanetNumber( int id ) {
	int pos;
	FILE *file;
if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &pos, 1, sizeof(int), file );
fclose( file );

return pos;
}

int dbUserPlanetAdd( int id, int plnid, int sysid, int plnloc, int flags )
{
  int a, pos;
  dbUserMainDef maind;
  dbMainEmpireDef empired;
  dbMainPlanetDef Planetd;
  FILE *file;
  

	dbMapRetrievePlanet(plnid, &Planetd);
	if(artefactPrecense(&Planetd) == 6)
  {
  	dbMapRetrieveEmpire( id, &empired );
  	for( a = 0 ; a < empired.numplayers ; a++ )
    {
  		if( dbUserMainRetrieve( empired.player[a], &maind ) < 0 )
        continue;
      dbUserMainSet( empired.player[a], &maind );
    }
	}
	
if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &pos, 1, sizeof(int), file );
  file_s( file, 4+(pos*20) );
  file_w( &plnid, 1, sizeof(int), file );
  file_w( &sysid, 1, sizeof(int), file );
  file_w( &plnloc, 1, sizeof(int), file );
  file_w( &flags, 1, sizeof(int), file );
  a = 0;
  file_w( &a, 1, sizeof(int), file );
  file_s( file, 0 );
  pos++;
  file_w( &pos, 1, sizeof(int), file );
  fclose( file );
  return pos;
}

int dbUserPlanetRemove( int id, int plnid )
{
  int a, b, num, data[5];
  dbUserMainDef maind;
  dbMainEmpireDef empired;
  dbMainPlanetDef Planetd;
  FILE *file;
  

	dbMapRetrievePlanet(plnid, &Planetd);
	if(artefactPrecense(&Planetd) == 6)
  {
  	dbMapRetrieveEmpire( id, &empired );
  	for( a = 0 ; a < empired.numplayers ; a++ )
    {
  		if( dbUserMainRetrieve( empired.player[a], &maind ) < 0 )
        continue;
      dbUserMainSet( empired.player[a], &maind );
    }
	}
if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &num, 1, sizeof(int), file );
  if( num >= 2 )
  {
    file_s( file, 4+(num*20)-20 );
    file_r( data, 1, 20, file );
  }
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20) );
    file_r( &b, 1, sizeof(int), file );
    if( b != plnid )
      continue;
    if( ( num >= 2 ) && ( a+1 < num ) )
    {
      file_s( file, 4+(a*20) );
      file_w( data, 1, 20, file );
    }
    file_s( file, 0 );
    num--;
    file_w( &num, 1, sizeof(int), file );
    fclose( file );
    return 1;
  }
  fclose( file );
  return -3;
}

int dbUserPlanetSetFlags( int id, int plnid, int flags )
{
  int num, pos;
  int *list;
  FILE *file;
  if( ( num = dbUserPlanetListIndices( id, &list ) ) < 0 )
    return -3;
  for( pos = 0 ; pos < num ; pos++ )
  {
    if( pos == num )
    {
      free( list );
      return -3;
    }
    if( list[pos] == plnid )
      break;
  }
  free( list );
if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
	error( "User Open" );
	return -3;
}
  file_s( file, 4+(pos*20)+12 );
  file_w( &flags, 1, sizeof(int), file );
  fclose( file );
  return 1;


}

int dbUserPlanetListCoords( int id, int **list )
{
  int a, num;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(int) ) ) )
  {
    fclose( file );
    return -1;
  }
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20)+8 );
    file_r( &listp[a], 1, sizeof(int), file );
  }
  fclose( file );
  *list = listp;
  return num;
}

int dbUserPlanetListIndices( int id, int **list )
{
  int a, num;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(int) ) ) )
  {
    fclose( file );
    return -1;
  }
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20) );
    file_r( &listp[a], 1, sizeof(int), file );
  }
  fclose( file );
  *list = listp;
  return num;
}

int dbUserPlanetListSystems( int id, int **list )
{
  int a, num;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(int) ) ) )
  {
    fclose( file );
    return -1;
  }
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20)+4 );
    file_r( &listp[a], 1, sizeof(int), file );
  }
  fclose( file );
  *list = listp;
  return num;
}



int dbUserPlanetListFull ( int id, int **list )
{
  int a, num;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*4*sizeof(int) ) ) )
  {
    fclose( file );
    return -1;
  }
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20) );
    file_r( &listp[a*4], 1, 4*sizeof(int), file );
  }
  fclose( file );
  *list = listp;
  return num;
}


int dbUserPlanetListIndicesSorted( int id, int **list, int sort )
{
  int a, num, b, d;
  int *listp, *list2p, *list3p;
  dbMainPlanetDef planetd;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(int) ) ) )
    {
      fclose( file );
      return -1;
    }
  
  if( !( num ) )
    {
      free (listp);
      fclose( file );
      return -1;
    }
  for( a = 0 ; a < num ; a++ )
    {
      file_s( file, 4+(a*20) );
      file_r( &listp[a], 1, sizeof(int), file );
    }
  fclose( file );
  if( !( list2p = malloc( (num)*sizeof(int) ) ) )
    {
      free (listp);
      return -1;
    }
  if( !( list3p = malloc( (num)*sizeof(int) ) ) )
    {
      free (listp);
      free (list2p);
      return -1;

    }
  if (sort == 1)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = planetd.size;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 2)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  for( a = d = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
	    d += planetd.building[a];
	  list2p[b]= d + planetd.construction;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 3)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  for( a = d = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
	    d += planetd.building[a];
	  list2p[b]= (int)(100 * ( cmdGetBuildOvercost( planetd.size, d + planetd.construction ) -1 ) );
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 4)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = planetd.population;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 5)

    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = planetd.protection;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 6)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = planetd.special[0];
	  list3p[b] = planetd.special[1];	  
	}
      sortlist2( num, listp, list2p, list3p );
    }
  else if (sort == 10)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = -( ( planetd.position >> 8 ) & 0xFFF );
	  list3p[b] = -( planetd.position >> 20 );
	}
      sortlist2( num, listp, list2p, list3p );
    }
  else if (sort == 11)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = -planetd.size;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 12)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  for( a = d = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
	    d += planetd.building[a];
	  list2p[b]= -d - planetd.construction;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 13)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  for( a = d = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
	    d += planetd.building[a];
	  list2p[b]= -(int)(100 * ( cmdGetBuildOvercost( planetd.size, d + planetd.construction ) -1 ) );
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 14)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = -planetd.population;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 15)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = -planetd.protection;
	}
      sortlist( num, listp, list2p );
    }
  else if (sort == 16)
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = -planetd.special[0];
	  list3p[b] = -planetd.special[1];	  
	}
      sortlist2( num, listp, list2p, list3p );
    }
  else
    {
      for( b = 0; b < num; b++)
	{
	  if( dbMapRetrievePlanet( listp[b], &planetd ) < 0 )
	    continue;
	  list2p[b] = ( ( planetd.position >> 8 ) & 0xFFF );
	  list3p[b] = ( planetd.position >> 20 );
	}
      sortlist2( num, listp, list2p, list3p );
    }
  

  *list = listp;
  free (list2p);
  free (list3p);
  return num;
}



// user planets/portals functions

int dbUserPortalsList( int id, int **list )
{
  int a, b, num, flags;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( 3*num*sizeof(int) ) ) )
    return -1;
  for( a = b = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20) );
    file_r( &listp[b], 1, 3*sizeof(int), file );
    file_r( &flags, 1, sizeof(int), file );
    if( flags & CMD_PLANET_FLAGS_PORTAL )
      b += 3;
  }
  fclose( file );
  *list = listp;
  return (b/3);
}

int dbUserPortalsListCoords( int id, int **list )
{
  int a, b, num, flags;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(int) ) ) )
    return -1;
  for( a = b = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20)+8 );
    file_r( &listp[b], 1, sizeof(int), file );
    file_r( &flags, 1, sizeof(int), file );
    if( flags & CMD_PLANET_FLAGS_PORTAL )
      b++;
  }
  fclose( file );
  *list = listp;
  return b;
}

int dbUserPortalsListIndices( int id, int **list )
{
  int a, b, num, flags;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_PLANETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(int) ) ) )
    return -1;
  for( a = b = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*20) );
    file_r( &listp[b], 1, sizeof(int), file );
    file_r( &flags, 1, sizeof(int), file );
    if( flags & CMD_PLANET_FLAGS_PORTAL )
      b++;
  }
  fclose( file );
  *list = listp;
  return b;
}






// user fleets functions

int dbUserFleetAdd( int id, dbUserFleetPtr fleetd )
{
  int pos;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_FLEETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &pos, 1, sizeof(int), file );
  file_s( file, 4+(pos*sizeof(dbUserFleetDef)) );
  file_w( fleetd, 1, sizeof(dbUserFleetDef), file );
  file_s( file, 0 );
  pos++;
  file_w( &pos, 1, sizeof(int), file );
  fclose( file );
  return (pos-1);
}

int dbUserFleetRemove( int id, int fltid )
{
  int a, num;
  dbUserFleetDef data;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_FLEETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)fltid >= num )
  {
    fclose( file );
    return -2;
  }
  if( fltid+1 < num )
  {
    file_s( file, 4+(num*sizeof(dbUserFleetDef))-sizeof(dbUserFleetDef) );
    file_r( &data, 1, sizeof(dbUserFleetDef), file );
    file_s( file, 4+(fltid*sizeof(dbUserFleetDef)) );
    file_w( &data, 1, sizeof(dbUserFleetDef), file );
  }
  file_s( file, 0 );
  a = num-1;
  file_w( &a, 1, sizeof(int), file );
  fclose( file );
  return 1;
}

int dbUserFleetList( int id, dbUserFleetPtr *fleetd )
{
  int a, num;
  dbUserFleetPtr fleetp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_FLEETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( fleetp = malloc( num*sizeof(dbUserFleetDef) ) ) )
  {
    fclose( file );
    return -1;
  }
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, 4+(a*sizeof(dbUserFleetDef)) );
    file_r( &fleetp[a], 1, sizeof(dbUserFleetDef), file );
  }
  fclose( file );
  *fleetd = fleetp;
  return num;
}

int dbUserFleetSet( int id, int fltid, dbUserFleetPtr fleetd )
{
  int num;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_FLEETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)fltid >= num )
  {
    fclose( file );
    return -2;

  }
  file_s( file, 4+(fltid*sizeof(dbUserFleetDef)) );
  file_w( fleetd, 1, sizeof(dbUserFleetDef), file );
  fclose( file );
  return num;
}

int dbUserFleetRetrieve( int id, int fltid, dbUserFleetPtr fleetd )
{
  int num;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_FLEETS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)fltid >= num )
  {
    fclose( file );
    return -2;
  }
  file_s( file, 4+(fltid*sizeof(dbUserFleetDef)) );
  file_r( fleetd, 1, sizeof(dbUserFleetDef), file );
  fclose( file );
  return num;
}





// user news functions

int dbUserNewsAdd( int id, int64_t *data, int64_t flags )
{
  int64_t a, num, lused, lfree, numnext, lcur, lnext, cflags;
  FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_NEWS ) ) ) {
	critical( "Database News Add" );
	return -3;
}
  

file_r( &num, 1, sizeof(int64_t), file );
file_r( &lused, 1, sizeof(int64_t), file );
file_r( &lfree, 1, sizeof(int64_t), file );
file_r( &cflags, 1, sizeof(int64_t), file );
cflags |= flags;
file_s( file, 24 );
file_w( &cflags, 1, sizeof(int64_t), file );

file_r( &numnext, 1, sizeof(int64_t), file );

  if( lfree != -1 )
  {
    file_s( file, 40+lfree*DB_USER_NEWS_SIZE+8 );
    file_r( &lnext, 1, sizeof(int64_t), file );
    file_s( file, 16 );
    file_w( &lnext, 1, sizeof(int64_t), file );
    lcur = lfree;
  }
  else
  {
    lcur = numnext;
    numnext++;
    file_s( file, 32 );
    file_w( &numnext, 1, sizeof(int64_t), file );
  }

  file_s( file, 40+lcur*DB_USER_NEWS_SIZE );
  a = -1;
  file_w( &a, 1, sizeof(int64_t), file );


  file_w( &lused, 1, sizeof(int64_t), file );

  file_w( data, 1, DB_USER_NEWS_BASE*sizeof(int64_t), file );
  if( lused != -1 )
  {
    file_s( file, 40+lused*DB_USER_NEWS_SIZE );
    file_w( &lcur, 1, sizeof(int64_t), file );
  }
  file_s( file, 8 );
  file_w( &lcur, 1, sizeof(int64_t), file );

  num++;
  file_s( file, 0 );
  file_w( &num, 1, sizeof(int64_t), file );

  fclose( file );
  return 1;
}

int64_t dbUserNewsGetFlags( int id ) {
	int64_t flags;
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_NEWS ) ) ) {
	critical( "Database News Get Flags" );
	return -3;
}

file_s( file, 24 );
file_r( &flags, 1, sizeof(int64_t), file );
fclose( file );

return flags;
}

int dbUserNewsList( int id, int64_t **data )
{
  int64_t a, b, c, num, lused, lfree;
  FILE *file;
  int64_t *datap;
  *data = 0;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_NEWS ) ) ) {
	critical( "Database News List" );
	return -3;
}
file_r( &num, 1, sizeof(int64_t), file );
file_r( &lused, 1, sizeof(int64_t), file );
file_r( &lfree, 1, sizeof(int64_t), file );

  if( !( datap = malloc( num*DB_USER_NEWS_BASE*sizeof(int64_t) ) ) )
  {
    fclose( file );
    return -3;
  }
  for( a = lused, b = 0, c = num ; c ; c--, b += (DB_USER_NEWS_BASE) )
  {
    file_s( file, 40+a*DB_USER_NEWS_SIZE+8 );
    file_r( &a, 1, sizeof(int64_t), file );
    file_r( &datap[b], 1, DB_USER_NEWS_BASE*sizeof(int64_t), file );
  }
  *data = datap;

  fclose( file );
  return num;
}

// I hate doing linked lists with files!! The one who fully understand this function on the first read gets a cookie.
int64_t dbUserNewsListUpdate( int id, int64_t **data, int64_t time )
{
  int64_t a, b, c, d, num, numnew, lused, lfree, lprev, lnext;
  FILE *file;
  int64_t *datap;
  *data = 0;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_NEWS ) ) ) {
	critical( "Database News Update" );
	return -3;
}
file_r( &num, 1, sizeof(int64_t), file );
file_r( &lused, 1, sizeof(int64_t), file );
file_r( &lfree, 1, sizeof(int64_t), file );
  a = 0;
  file_w( &a, 1, sizeof(int64_t), file );
  numnew = num;

  if( !( datap = malloc( num*DB_USER_NEWS_BASE*sizeof(int64_t) ) ) )
  {
    fclose( file );
    return -3;
  }
  for( a = lused, b = 0, c = num ; c ; c--, a = lnext )
  {
    file_s( file, 40+a*DB_USER_NEWS_SIZE );
    file_r( &lprev, 1, sizeof(int64_t), file );
    file_r( &lnext, 1, sizeof(int64_t), file );
    file_r( &datap[b], 1, 2*sizeof(int64_t), file );
    if( !( datap[b+1] & CMD_NEWS_FLAGS_NEW ) )
    {
      numnew--;
      if( datap[b+0]+CMD_NEWS_EXPIRE_TIME >= time )
        continue;
      num--;

      if( lprev != -1 ) {
        file_s( file, 40+lprev*DB_USER_NEWS_SIZE+8 );
      } else {
        file_s( file, 8 );
        }
      file_w( &lnext, 1, sizeof(int64_t), file );
      if( lnext != -1 )
      {
        file_s( file, 40+lnext*DB_USER_NEWS_SIZE );
        file_w( &lprev, 1, sizeof(int64_t), file );
      }

      file_s( file, 40+a*DB_USER_NEWS_SIZE );
      d = -1;
      file_w( &d, 1, sizeof(int64_t), file );
      file_w( &lfree, 1, sizeof(int64_t), file );
      if( lfree != -1 )
      {
        file_s( file, 40+lfree*DB_USER_NEWS_SIZE );
        file_w( &a, 1, sizeof(int64_t), file );
      }
      file_s( file, 16 );
      lfree = a;
      file_w( &lfree, 1, sizeof(int64_t), file );

      file_s( file, 0 );
      file_w( &num, 1, sizeof(int64_t), file );
    }
    file_s( file, 40+a*DB_USER_NEWS_SIZE+16+8 );
    datap[b+1] &= 0xFFFFFFFF - CMD_NEWS_FLAGS_NEW;
    file_w( &datap[b+1], 1, sizeof(int64_t), file );
    file_r( &datap[b+2], 1, (DB_USER_NEWS_BASE-2)*sizeof(int64_t), file );
    b += DB_USER_NEWS_BASE;
  }
  *data = datap;

  fclose( file );
  return numnew;
}

int dbUserNewsEmpty( int id ) {
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_NEWS ) ) ) {
	critical( "Database News Empty" );
	return -3;
}

file_w( dbFileUserListData[DB_FILE_USER_NEWS], 1, dbFileUserListBase[DB_FILE_USER_NEWS], file );
fclose( file );

return 1;
}





// fam news functions

int dbFamNewsAdd( int id, int64_t *data )
{
  int64_t a, num, lused, lfree, numnext, lcur, lnext;
  FILE *file;
  char fname[PATH_MAX];
  snprintf( fname, sizeof(fname), "fam%dnews", id );
if( !( file = fopen( fname, "rb+" ) ) ) {
	critical( "Database Empire News Add" );
  	return -3;
}
  
file_r( &num, 1, sizeof(int64_t), file );
file_r( &lused, 1, sizeof(int64_t), file );
file_r( &lfree, 1, sizeof(int64_t), file );
  file_s( file, 32 );
file_r( &numnext, 1, sizeof(int64_t), file );

  if( lfree != -1 )
  {
    file_s( file, 40+lfree*DB_USER_NEWS_SIZE+8 );
    file_r( &lnext, 1, sizeof(int64_t), file );
    file_s( file, 16 );
    file_w( &lnext, 1, sizeof(int64_t), file );
    lcur = lfree;
  }
  else
  {
    lcur = numnext;
    numnext++;
    file_s( file, 32 );
    file_w( &numnext, 1, sizeof(int64_t), file );
  }

  file_s( file, 40+lcur*DB_USER_NEWS_SIZE );
  a = -1;
  file_w( &a, 1, sizeof(int64_t), file );

  file_w( &lused, 1, sizeof(int64_t), file );

  file_w( data, 1, DB_USER_NEWS_BASE*sizeof(int64_t), file );
  if( lused != -1 )
  {
    file_s( file, 40+lused*DB_USER_NEWS_SIZE );
    file_w( &lcur, 1, sizeof(int64_t), file );
  }
  file_s( file, 8 );
  file_w( &lcur, 1, sizeof(int64_t), file );

  num++;
  file_s( file, 0 );
  file_w( &num, 1, sizeof(int64_t), file );

  fclose( file );
  return 1;
}

int dbFamNewsList( int id, int64_t **data, int time )
{
  int64_t a, b, c, d, num, lused, lfree, lprev, lnext;
  FILE *file;
  int64_t *datap;
  char fname[PATH_MAX];
  *data = 0;
  snprintf( fname, sizeof(fname), "fam%dnews", id );

if( !( file = fopen( fname, "rb+" ) ) ) {
	critical( "Database Empire News List" );
	return -3;
}

file_r( &num, 1, sizeof(int64_t), file );
file_r( &lused, 1, sizeof(int64_t), file );
file_r( &lfree, 1, sizeof(int64_t), file );
  a = 0;
  file_w( &a, 1, sizeof(int), file );
  if( !( datap = malloc( num*DB_USER_NEWS_BASE*sizeof(int64_t) ) ) )
  {
    fclose( file );
    return -3;
  }
  for( a = lused, b = 0, c = num ; c ; c--, a = lnext )
  {
    file_s( file, 40+a*DB_USER_NEWS_SIZE );
    file_r( &lprev, 1, sizeof(int64_t), file );
    file_r( &lnext, 1, sizeof(int64_t), file );
    file_r( &datap[b], 1, 1*sizeof(int64_t), file );
    if( datap[b+0]+CMD_NEWS_EXPIRE_TIME < time )
    {
      num--;
      if( lprev != -1 ) {
        file_s( file, 40+lprev*DB_USER_NEWS_SIZE+8 );
      } else {
        file_s( file, 8 );
      }
      file_w( &lnext, 1, sizeof(int64_t), file );
      if( lnext != -1 )
      {
        file_s( file, 40+lnext*DB_USER_NEWS_SIZE );


        file_w( &lprev, 1, sizeof(int64_t), file );
      }

      file_s( file, 40+a*DB_USER_NEWS_SIZE );
      d = -1;
      file_w( &d, 1, sizeof(int64_t), file );
      file_w( &lfree, 1, sizeof(int64_t), file );
      if( lfree != -1 )
      {
        file_s( file, 40+lfree*DB_USER_NEWS_SIZE );
        file_w( &a, 1, sizeof(int64_t), file );
      }
      file_s( file, 16 );
      lfree = a;
      file_w( &lfree, 1, sizeof(int64_t), file );

      file_s( file, 0 );
      file_w( &num, 1, sizeof(int64_t), file );
    }
    file_s( file, 40+a*DB_USER_NEWS_SIZE+16+8 );
    file_r( &datap[b+1], 1, (DB_USER_NEWS_BASE-1)*sizeof(int64_t), file );
    b += DB_USER_NEWS_BASE;
  }
  *data = datap;

  fclose( file );
  return num;
}






// map functions

int dbMapRetrieveMain( int *binfo ) {
	FILE *file;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

file_s( file, 0 );
file_r( binfo, 1, sizeof(dbMainMapDef), file );

return 1;
}

int dbMapSetMain( int *binfo ) {
	FILE *file;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

file_s( file, 0 );
file_w( binfo, 1, sizeof(dbMainMapDef), file );

return 1;
}


int dbMapSetSystem( int sysid, dbMainSystemPtr systemd ) {
	FILE *file;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

if( (unsigned int)sysid >= dbMapBInfoStatic[MAP_SYSTEMS] )
	return -3;

file_s( file, sizeof(dbMainMapDef)+(sysid*sizeof(dbMainSystemDef)) );
file_w( systemd, 1, sizeof(dbMainSystemDef), file );

return 1;
}

int dbMapRetrieveSystem( int sysid, dbMainSystemPtr systemd ) {
	FILE *file;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

if( (unsigned int)sysid >= dbMapBInfoStatic[MAP_SYSTEMS] )
	return -3;

file_s( file, sizeof(dbMainMapDef)+(sysid*sizeof(dbMainSystemDef)) );
file_r( systemd, 1, sizeof(dbMainSystemDef), file );

return 1;
}

int dbMapSetPlanet( int plnid, dbMainPlanetPtr planetd ) {
	FILE *file;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

if( (unsigned int)plnid >= dbMapBInfoStatic[MAP_PLANETS] )
	return -3;

file_s( file, sizeof(dbMainMapDef)+(dbMapBInfoStatic[MAP_SYSTEMS]*sizeof(dbMainSystemDef))+(plnid*sizeof(dbMainPlanetDef)) );
file_w( planetd, 1, sizeof(dbMainPlanetDef), file );

return 1;
}

int dbMapRetrievePlanet( int plnid, dbMainPlanetPtr planetd ) {
	FILE *file;

if( plnid == -1)
	return -3;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

if( (unsigned int)plnid >= dbMapBInfoStatic[MAP_PLANETS] )
	return -3;

file_s( file, sizeof(dbMainMapDef)+(dbMapBInfoStatic[MAP_SYSTEMS]*sizeof(dbMainSystemDef))+(plnid*sizeof(dbMainPlanetDef)) );
file_r( planetd, 1, sizeof(dbMainPlanetDef), file );

return 1;
}


int dbMapSetEmpire( int famid, dbMainEmpirePtr empired ) {
	FILE *file;
//	dbUserPtr user;


if(!( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

if((unsigned int)famid >= dbMapBInfoStatic[MAP_EMPIRES])
	return -3;
/*
//---------------------
if ( empired->numplayers == 1) {
	empired->leader = empired->player[0];
	if(( user = dbUserLinkID( empired->leader ) ) ) {
		//user->flags &= 0xFFFF;
		bitflag_add( &user->flags, CMD_USER_FLAGS_LEADER] | CMD_USER_FLAGS_ACTIVATED );
		dbUserSave( empired->leader, user );
	}
}
//-----------------------
*/

file_s( file, sizeof(dbMainMapDef)+(dbMapBInfoStatic[MAP_SYSTEMS]*sizeof(dbMainSystemDef))+(dbMapBInfoStatic[MAP_PLANETS]*sizeof(dbMainPlanetDef))+(famid*sizeof(dbMainEmpireDef)) );

file_w( empired, 1, sizeof(dbMainEmpireDef), file );

return 1;
}

int dbMapRetrieveEmpire( int famid, dbMainEmpirePtr empired ) {
	FILE *file;

if( !( file = dbFileGenOpen( DB_FILE_MAP ) ) )
	return -3;

if( (unsigned int)famid >= dbMapBInfoStatic[MAP_EMPIRES] )
	return -3;

file_s( file, sizeof(dbMainMapDef)+(dbMapBInfoStatic[MAP_SYSTEMS]*sizeof(dbMainSystemDef))+(dbMapBInfoStatic[MAP_PLANETS]*sizeof(dbMainPlanetDef))+(famid*sizeof(dbMainEmpireDef)) );
file_r( empired, 1, sizeof(dbMainEmpireDef), file );

return 1;
}




/*
relation
  4:tick
  4:type
  4:empire
  4:flags
*/

int dbEmpireRelsAdd( int id, int *rel )
{
  int pos;
  FILE *file;
  if( (unsigned int)id >= dbMapBInfoStatic[MAP_EMPIRES] )
    return -3;
  if( !( file = dbFileFamOpen( id, 0 ) ) )
    return -3;
  file_r( &pos, 1, sizeof(int), file );
  file_s( file, 4+(pos*sizeof(dbEmpireRelationsDef)) );
  file_w( rel, 1, sizeof(dbEmpireRelationsDef), file );
  file_s( file, 0 );
  pos++;
  file_w( &pos, 1, sizeof(int), file );
  fclose( file );
  return (pos-1);
}

int dbEmpireRelsRemove( int id, int relid )
{
  int a, num, data[4];
  FILE *file;
  if( (unsigned int)id >= dbMapBInfoStatic[MAP_EMPIRES] )
    return -3;
  if( !( file = dbFileFamOpen( id, 0 ) ) )
    return -3;
  file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)relid >= num )
  {
    fclose( file );
    return -2;
  }
  if( relid+1 < num )
  {
    file_s( file, 4+(num*sizeof(dbEmpireRelationsDef))-sizeof(dbEmpireRelationsDef) );
    file_r( data, 1, sizeof(dbEmpireRelationsDef), file );
    file_s( file, 4+(relid*sizeof(dbEmpireRelationsDef)) );
    file_w( data, 1, sizeof(dbEmpireRelationsDef), file );
  }
  file_s( file, 0 );
  a = num-1;
  file_w( &a, 1, sizeof(int), file );
  fclose( file );
  return 1;
}

int dbEmpireRelsList( int id, int **rel )
{
  int num;
  FILE *file;
  int *relp;
  if( (unsigned int)id >= dbMapBInfoStatic[MAP_EMPIRES] )
    return -3;
  if( !( file = dbFileFamOpen( id, 0 ) ) )
    return -3;
  file_r( &num, 1, sizeof(int), file );
  if( !( relp = malloc( num*sizeof(dbEmpireRelationsDef) ) ) )
  {
    fclose( file );
    return -1;
  }
  if( num )
  	file_r( relp, 1, num*sizeof(dbEmpireRelationsDef), file );
  fclose( file );
  *rel = relp;
  return num;
}

int dbEmpireRelsGet( int id, int relid, int *rel )
{
  int num;
  FILE *file;
  if( (unsigned int)id >= dbMapBInfoStatic[MAP_EMPIRES] )
    return -3;
  if( !( file = dbFileFamOpen( id, 0 ) ) )
    return -3;
  file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)relid >= num )
  {
    fclose( file );
    return -1;
  }
  file_s( file, 4+relid*sizeof(dbEmpireRelationsDef) );
  file_r( rel, 1, sizeof(dbEmpireRelationsDef), file );
  fclose( file );
  return num;
}


/*
market
   4:num max bids
   4:list free

3 * 2 * 80 * 12 ; res*action*prices*struct
struct( 12 )
   4:total of bids
   4:first list ID
   4:last list ID
struct( 16 )
   4:previous id
   4:next id
   4:quantity
   4:owner ID


bid :
4:action (0buy,1sell)
4:ressource
4:price
4:quantity
4:user ID
*/


int dbMarketReset()
{
  int a, array[3];
  FILE *file;
  if( !( file = fopen( dbFileList[DB_FILE_MARKET], "wb+" ) ) )
    return 0;
  file_s( file, 0 );
  array[0] = 0;
  array[1] = -1;
  file_w( array, 1, 2*sizeof(int), file );
  array[0] = 0;
  array[1] = -1;
  array[2] = -1;
  for( a = 0 ; a < 6*DB_MARKET_RANGE ; a++ )
    file_w( array, 1, 3*sizeof(int), file );
  fclose( file );
  return 1;
}

int dbMarketFull( int *list )
{
  int a;
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;
  file_s( file, 8 );
  for( a = 0 ; a < 3*2*DB_MARKET_RANGE ; a++ )
  {
    file_s( file, 8+a*12 );
    file_r( &list[a], 1, sizeof(int), file );
  }
  return 1;
}


int dbMarketReplaceFull( int *list )
{
  int a;
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;
  file_s( file, 8 );
  for( a = 0 ; a < 3*2*DB_MARKET_RANGE ; a++ )
  {
    file_s( file, 8+a*12 );
    file_w( &list[a], 1, sizeof(int), file );
  }
  return 1;
}





int dbMarketAdd( int *bid )
{
  int a, offs, num, lcur, lfree, databid[4];
  int mhead[3];
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;

  offs = 8;
  offs += bid[DB_MARKETBID_RESSOURCE] * 2 * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_ACTION] * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_PRICE] * 12;

  file_s( file, offs );
  file_r( mhead, 1, 12, file );

  file_s( file, 0 );
  file_r( &num, 1, sizeof(int), file );
  file_r( &lfree, 1, sizeof(int), file );

  if( lfree != -1 )
  {
    file_s( file, DB_MARKET_BIDSOFF+lfree*16 + 4 );
    file_r( &a, 1, sizeof(int), file );
    file_s( file, 4 );
    file_w( &a, 1, sizeof(int), file );
    lcur = lfree;
  }
  else
  {
    lcur = num;
    num++;
    file_s( file, 0 );
    file_w( &num, 1, sizeof(int), file );
  }

  databid[0] = mhead[2];
  databid[1] = -1;
  databid[2] = bid[DB_MARKETBID_QUANTITY];
  databid[3] = bid[DB_MARKETBID_USERID];

  mhead[0] += bid[DB_MARKETBID_QUANTITY];
  if( mhead[1] == -1 )
    mhead[1] = lcur;
  else
  {
    file_s( file, DB_MARKET_BIDSOFF+mhead[2]*16 + 4 );
    file_w( &lcur, 1, sizeof(int), file );
  }
  mhead[2] = lcur;

  file_s( file, DB_MARKET_BIDSOFF+lcur*16 );
  file_w( databid, 1, 16, file );

  file_s( file, offs );
  file_w( mhead, 1, 12, file );

  return lcur;
}

int dbMarketRemove( int *bid, int lcur )
{
  int offs, lfree, databid[4];
  int mhead[3];
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;

  offs = 8;
  offs += bid[DB_MARKETBID_RESSOURCE] * 2 * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_ACTION] * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_PRICE] * 12;

  file_s( file, offs );
  file_r( mhead, 1, 12, file );
  file_s( file, DB_MARKET_BIDSOFF+lcur*16 );
  file_r( databid, 1, 16, file );

  if( databid[0] != -1 )
  {
    file_s( file, DB_MARKET_BIDSOFF+databid[0]*16+4 );
    file_w( &databid[1], 1, sizeof(int), file );
  }
  if( databid[1] != -1 )
  {
    file_s( file, DB_MARKET_BIDSOFF+databid[1]*16 );
    file_w( &databid[0], 1, sizeof(int), file );
  }

  mhead[0] -= databid[2];
  if( mhead[1] == lcur )
    mhead[1] = databid[1];
  if( mhead[2] == lcur )
    mhead[2] = databid[0];
  file_s( file, offs );
  file_w( mhead, 1, 12, file );

  file_s( file, 4 );
  file_r( &lfree, 1, sizeof(int), file );

  databid[0] = -1;
  databid[1] = lfree;
  databid[2] = -1;
  databid[3] = -1;
  file_s( file, DB_MARKET_BIDSOFF+lcur*16 );
  file_w( databid, 1, 16, file );

  file_s( file, 4 );
  file_w( &lcur, 1, sizeof(int), file );

  return lcur;
}

int dbMarketListStart( int *bid )
{
  int a, offs;
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;

  offs = 8;
  offs += bid[DB_MARKETBID_RESSOURCE] * 2 * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_ACTION] * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_PRICE] * 12;

  file_s( file, offs + 4 );
  file_r( &a, 1, 4, file );

  return a;
}

int dbMarketListNext( int lcur, int *result )
{
  int databid[4];
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;
  file_s( file, DB_MARKET_BIDSOFF+lcur*16 );
  file_r( databid, 1, 16, file );
  result[0] = databid[2];
  result[1] = databid[3];
  return databid[1];
}

int dbMarketSetQuantity( int *bid, int lcur, int quantity, int loss )
{
  int offs;
  int mhead[3];
  FILE *file;
  if( !( file = dbFileGenOpen( DB_FILE_MARKET ) ) )
    return -3;

  offs = 8;
  offs += bid[DB_MARKETBID_RESSOURCE] * 2 * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_ACTION] * DB_MARKET_RANGE * 12;
  offs += bid[DB_MARKETBID_PRICE] * 12;

  file_s( file, offs );
  file_r( mhead, 1, 12, file );
  mhead[0] -= loss;
  file_s( file, offs );
  file_w( mhead, 1, 12, file );

  file_s( file, DB_MARKET_BIDSOFF+lcur*16 + 8 );
  file_w( &quantity, 1, 4, file );

  return 0;
}




/*
user%d/market
  4:number of bids
  4:flags
struct ( 20 )
  4:action
  4:resource
  4:price
  4:quantity
  4:bid ID

*/

int dbUserMarketReset( int id )
{
  int a;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_MARKET ) ) ) {
  	error( "User Open" );
    return -3;
    }
  a = 0;
  file_w( &a, 1, sizeof(int), file );
  file_w( &a, 1, sizeof(int), file );
  fclose( file );
  return 1;
}

int dbUserMarketAdd( int id, int bidid, int action, int resource, int price, int quantity )
{
  int pos;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_MARKET ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &pos, 1, sizeof(int), file );
  file_s( file, (2*sizeof(int))+(pos*sizeof(dbMarketUserDef)) );
  file_w( &action, 1, sizeof(int), file );
  file_w( &resource, 1, sizeof(int), file );
  file_w( &price, 1, sizeof(int), file );
  file_w( &quantity, 1, sizeof(int), file );
  file_w( &bidid, 1, sizeof(int), file );
  file_s( file, 0 );
  pos++;
  file_w( &pos, 1, sizeof(int), file );
  fclose( file );
  return 1;
}

int dbUserMarketList( int id, int **list )
{
  int num;
  int *listp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_MARKET ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( !( listp = malloc( num*sizeof(dbMarketUserDef) ) ) )
    return -1;
  if( num == 0 )
	goto RETURN;
  file_s( file, 2*sizeof(int) );
  file_r( listp, 1, num*sizeof(dbMarketUserDef), file );
  RETURN:
  fclose( file );
  *list = listp;
  return num;
}

int dbUserMarketQuantity( int id, int bidid, int quantity )
{
  int a, b, num;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_MARKET ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, (2*sizeof(int))+(a*sizeof(dbMarketUserDef))+16 );
    file_r( &b, 1, sizeof(int), file );
    if( b != bidid )
      continue;
    file_s( file, (2*sizeof(int))+(a*sizeof(dbMarketUserDef))+12 );
    file_w( &quantity, 1, sizeof(int), file );
    fclose( file );
    return 1;
  }
  fclose( file );
  return -3;
}

int dbUserMarketRemove( int id, int bidid )
{
  int a, b, num, data[5];
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_MARKET ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( num >= 2 )
  {
    file_s( file, (2*sizeof(int))+(num*sizeof(dbMarketUserDef))-sizeof(dbMarketUserDef) );
    file_r( data, 1, sizeof(dbMarketUserDef), file );
  }
  file_s( file, (2*sizeof(int)) );
  for( a = 0 ; a < num ; a++ )
  {
    file_s( file, (2*sizeof(int))+(a*sizeof(dbMarketUserDef))+16 );
    file_r( &b, 1, sizeof(int), file );
    if( b != bidid )
      continue;
    if( ( num >= 2 ) && ( a+1 < num ) )
    {
      file_s( file, (2*sizeof(int))+(a*sizeof(dbMarketUserDef)) );
      file_w( data, 1, sizeof(dbMarketUserDef), file );
    }
    file_s( file, 0 );
    num--;
    file_w( &num, 1, sizeof(int), file );
    fclose( file );
    return 1;
  }
  fclose( file );
  return -3;
}







/*
forums
  4:number
struct ( X )
N:X:dbForumForumDef

forum%d/threads
  4:num
  4:list used
  4:list free
  4:new num
struct ( X )
  X;dbForumForumDef
struct ( X )
  4:previous
  4:next
  X:dbForumThreadDef

forum%d/thread%d
  4:number
  4:offset
struct ( X )
  X:dbForumThreadDef
struct ( X+Y )
  4:offset
  X:dbForumPostInDef
  Y:text
*/

int dbForumListForums( int flags, int perms, dbForumForumPtr *forums ) {
	int num;
	FILE *file;
	dbForumForumPtr forumsp;
	ConfigArrayPtr settings;
	char fname[PATH_MAX];

if( flags ) {
	settings = GetSetting( "Directory" );
	sprintf(fname, "%s/data/forums", settings->string_value);
} else {
	settings = GetSetting( "Public Forum" );
	sprintf(fname, "%s/forums", settings->string_value);
}

if( !( file = fopen( fname, "rb+" ) ))
	return -3;
file_r( &num, 1, sizeof(int), file );
if( !( forumsp = malloc( num * sizeof(dbForumForumDef) ) ) )
	return -3;
if( num == 0 )
	goto RETURN;
file_r( forumsp, 1, num*sizeof(dbForumForumDef), file );

RETURN:
*forums = forumsp;
fclose( file );

return num;
}


int dbForumListThreads( int flags, int forum, int base, int end, dbForumForumPtr forumd, dbForumThreadPtr *threads ) {
	ConfigArrayPtr settings;
  int a, b, c, d, num, lused, lfree, numnext;
  FILE *file;
  char fname[PATH_MAX];
  dbForumThreadPtr threadsp;

  *threads = 0;
  if( base < 0 )
    return -3;
  if( flags ) {
  	settings = GetSetting( "Directory" ); 
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, forum );
  } else {
  	settings = GetSetting( "Public Forum" ); 
  	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, forum );
  }
  
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
file_r( &lused, 1, sizeof(int), file );
file_r( &lfree, 1, sizeof(int), file );
file_r( &numnext, 1, sizeof(int), file );
file_r( forumd, 1, sizeof(dbForumForumDef), file );

  if( end < num )
    num = end;
  if( base > num )
  {
    fclose( file );
    return -3;
  }

  if( !( threadsp = malloc( num * sizeof(dbForumThreadDef) ) ) )
    return -3;
  if( base == num )
  {
    fclose( file );
    *threads = threadsp;
    return 0;
  }
  for( a = lused, b = 0 ; b < base ; b++ )
  {
    file_s( file, 16+sizeof(dbForumForumDef) + a * ( sizeof(dbForumThreadDef) + 8 ) + 4 );
    file_r( &a, 1, sizeof(int), file );
  }
  for( d = 0 ; b < num ; b++, d++ )
  {
    file_s( file, 16+sizeof(dbForumForumDef) + a * ( sizeof(dbForumThreadDef) + 8 ) + 4 );
    c = a;
    file_r( &a, 1, sizeof(int), file );
    file_r( &threadsp[d], 1, sizeof(dbForumThreadDef), file );
    threadsp[d].id = c;
  }

  *threads = threadsp;
  fclose( file );


  return d;
}


int dbForumListPosts( int flags, int forum, int thread, int base, int end, dbForumThreadPtr threadd, dbForumPostPtr *posts ) {
	ConfigArrayPtr settings;
  int a, b, offset, num;
  char fname[PATH_MAX];
  FILE *file;
  dbForumPostPtr postsp;

  *posts = 0;
  if( base < 0 )
    return -3;

if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/thread%d", settings->string_value, forum, thread );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/thread%d", settings->string_value, forum, thread );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );

  file_s( file, 8 );
file_r( threadd, 1, sizeof(dbForumThreadDef), file );

  if( end < num )
    num = end;
  if( base >= num )
  {
    fclose( file );
    return -3;
  }

  if( !( postsp = malloc( num * sizeof(dbForumPostDef) ) ) )
    return -3;
  offset = 8+sizeof(dbForumThreadDef);
  for( a = 0 ; a < base ; a++ )
  {
    file_s( file, offset );
    file_r( &offset, 1, sizeof(int), file );
  }

  for( b = 0 ; a < num ; a++, b++ )
  {
    file_s( file, offset );
    file_r( &offset, 1, sizeof(int), file );
    file_r( &postsp[b].post, 1, sizeof(dbForumPostInDef), file );
    postsp[b].text = 0;
    if( (unsigned int)((postsp[b].post).length) >= FORUM_MAX )
    {
      (postsp[b].post).flags |= DB_FORUM_FLAGS_POSTERROR;
      continue;
    }
    if( !( postsp[b].text = malloc( (postsp[b].post).length + 1 ) ) )
    {
      (postsp[b].post).flags |= DB_FORUM_FLAGS_POSTERROR;
      continue;
    }
    file_r( postsp[b].text, 1, (postsp[b].post).length, file );
    postsp[b].text[ (postsp[b].post).length ] = 0;
  }

  *posts = postsp;
  fclose( file );
  return b;
}




int dbForumRetrieveForum( int flags, int forum, dbForumForumPtr forumd ) {
	ConfigArrayPtr settings;
	FILE *file;
	char fname[PATH_MAX];
  
if( flags ) {
	settings = GetSetting( "Directory" );
	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, forum );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
  file_s( file, 16 );
  file_r( forumd, 1, sizeof(dbForumForumDef), file );
  fclose( file );
  return 1;
}



int dbForumAddForum( int flags, int nid, dbForumForumPtr forumd ) {
	ConfigArrayPtr settings;
  int a, num;
  FILE *file;
  char fname[PATH_MAX];
  num = nid;

if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forums", settings->string_value);
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forums", settings->string_value);
}

if( !( file = fopen( fname, "rb+" ) ))
	return -3;
file_r( &num, 1, sizeof(int), file );
file_s( file, 4+num*sizeof(dbForumForumDef) );
file_w( forumd, 1, sizeof(dbForumForumDef), file );
num++;
file_s( file, 0 );
file_w( &num, 1, sizeof(int), file );
num--;

fclose( file );

	
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d", settings->string_value, num );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d", settings->string_value, num );
}

if( mkdir( fname, S_IRWXU ) == -1 ) {
	error( "Making DIR" );
	return -3;
}


if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, num );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, num );
}

if( !( file = fopen( fname, "wb" ) ))
	return -3;
a = 0;
file_w( &a, 1, sizeof(int), file );
a = -1;
file_w( &a, 1, sizeof(int), file );
file_w( &a, 1, sizeof(int), file );
a = 0;
file_w( &a, 1, sizeof(int), file );
file_w( forumd, 1, sizeof(dbForumForumDef), file );
fclose( file );

return num;
}

int dbForumRemoveForum( int flags, int forum )
{
  int a, b, num;
  FILE *file;
  DIR *dirdata;
  struct dirent *direntry;
  char fname[PATH_MAX];
  char *frcopy;
  dbForumForumDef forumd;
  ConfigArrayPtr settings;
  
if( flags ) {
	settings = GetSetting( "Directory" );
	a = snprintf( fname, sizeof(fname), "%s/data/forum%d", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
  	a = snprintf( fname, sizeof(fname),  "%s/forum%d", settings->string_value, forum );
}

if( !( dirdata = opendir( fname ) ) ) {
	error( "Opening DIR" );
	return -3;
}
fname[a] = '/';
while( ( direntry = readdir( dirdata ) ) ) {
	sprintf( &fname[a+1], "%s", direntry->d_name );
	unlink( fname );
}
closedir( dirdata );
if( flags ) {
	settings = GetSetting( "Directory" );
	a = snprintf( fname, sizeof(fname), "%s/data/forum%d", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
  	a = snprintf( fname, sizeof(fname),  "%s/forum%d", settings->string_value, forum );
}  
rmdir( fname );
if( flags ) {
	settings = GetSetting( "Directory" );
	a = snprintf( fname, sizeof(fname), "%s/data/%d/forum", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
  	a = snprintf( fname, sizeof(fname),  "%s/%d/forum", settings->string_value, forum );
}

	
  if( !( file = fopen( fname, "rb+" ) ))

    return -3;
file_r( &num, 1, sizeof(int), file );
  if( forum >= num )
  {
    fclose( file );
    return 0;
  }
  file_s( file, 4+forum*sizeof(dbForumForumDef) );
  file_r( &forumd, 1, sizeof(dbForumForumDef), file );
  forumd.flags |= DB_FORUM_FLAGS_FORUMUNUSED;
  file_s( file, 4+forum*sizeof(dbForumForumDef) );
  file_w( &forumd, 1, sizeof(dbForumForumDef), file );
  a = num - ( forum + 1 );
  if( a )
  {
    b = a * sizeof(dbForumForumDef);
    frcopy = malloc( b );
    file_s( file, 4+(forum+1)*sizeof(dbForumForumDef) );
    file_r( frcopy, 1, b, file );
    file_s( file, 4+(forum+0)*sizeof(dbForumForumDef) );
    file_w( frcopy, 1, b, file );
    free( frcopy );
  }
  num--;
  file_s( file, 0 );
  file_w( &num, 1, sizeof(int), file );
  fclose( file );

  return num;
}



int dbForumAddThread( int flags, int forum, dbForumThreadPtr threadd ) {
	ConfigArrayPtr settings;
	int a, num, lused, lfree, numnext, lcur, lnext;
	FILE *file;
	char fname[PATH_MAX];
	dbForumForumDef forumd;

if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, forum );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
file_r( &lused, 1, sizeof(int), file );
file_r( &lfree, 1, sizeof(int), file );
file_r( &numnext, 1, sizeof(int), file );
file_r( &forumd, 1, sizeof(dbForumForumDef), file );

  if( lfree != -1 )
  {
    file_s( file, 16+sizeof(dbForumForumDef) + lfree * ( sizeof(dbForumThreadDef) + 8 ) + 4 );
    file_r( &lnext, 1, sizeof(int), file );
    file_s( file, 8 );
    file_w( &lnext, 1, sizeof(int), file );
    lcur = lfree;
  }
  else
  {
    lcur = numnext;
    numnext++;
    file_s( file, 12 );
    file_w( &numnext, 1, sizeof(int), file );
  }
  forumd.time = threadd->time;
  forumd.tick = threadd->tick;
  forumd.threads++;

  file_s( file, 16+sizeof(dbForumForumDef) + lcur * ( sizeof(dbForumThreadDef) + 8 ) );
  a = -1;
  file_w( &a, 1, sizeof(int), file );
  file_w( &lused, 1, sizeof(int), file );
  threadd->flags &= 0xFFFFFFFF - DB_FORUM_FLAGS_THREADFREE;
  file_w( threadd, 1, sizeof(dbForumThreadDef), file );

  if( lused != -1 )
  {
    file_s( file, 16+sizeof(dbForumForumDef) + lused * ( sizeof(dbForumThreadDef) + 8 ) );
    file_w( &lcur, 1, sizeof(int), file );
  }

  file_s( file, 0 );
  num++;
  file_w( &num, 1, sizeof(int), file );
  file_w( &lcur, 1, sizeof(int), file );
  file_s( file, 16 );
  file_w( &forumd, 1, sizeof(dbForumForumDef), file );
  fclose( file );
if( flags ) {
	settings = GetSetting( "Directory" );
  	  snprintf( fname, sizeof(fname), "%s/data/forums", settings->string_value );
} else {
	settings = GetSetting( "Public Forum" );
  	  snprintf( fname, sizeof(fname), "%s/forums", settings->string_value );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
  file_r( &num, 1, sizeof(int), file );
  if( forum < num )
  {
    file_s( file, 4+forum*sizeof(dbForumForumDef) );
    file_w( &forumd, 1, sizeof(dbForumForumDef), file );
  }
  fclose( file );

if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/thread%d", settings->string_value, forum, lcur );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/thread%d", settings->string_value, forum, lcur );
}

  if( !( file = fopen( fname, "wb+" ) ))
    return -3;
  a = 0;
  file_w( &a, 1, sizeof(int), file );
  a = 8 + sizeof(dbForumThreadDef);
  file_w( &a, 1, sizeof(int), file );
  file_w( threadd, 1, sizeof(dbForumThreadDef), file );
  fclose( file );

  return lcur;
}

int dbForumRemoveThread( int flags, int forum, int thread ) {
	ConfigArrayPtr settings;
	int a, num, lused, lfree, numnext, lprev, lnext;
	FILE *file;
	char fname[PATH_MAX];
	dbForumForumDef forumd;
	dbForumThreadDef threadd;

if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, forum );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
file_r( &lused, 1, sizeof(int), file );
file_r( &lfree, 1, sizeof(int), file );
file_r( &numnext, 1, sizeof(int), file );
file_r( &forumd, 1, sizeof(dbForumForumDef), file );
  if( (unsigned int)thread >= numnext )
  {
    fclose( file );
    return -3;
  }

  file_s( file, 16+sizeof(dbForumForumDef) + thread * ( sizeof(dbForumThreadDef) + 8 ) );
  file_r( &lprev, 1, sizeof(int), file );
  file_r( &lnext, 1, sizeof(int), file );
  file_r( &threadd, 1, sizeof(dbForumThreadDef), file );
  if( ( threadd.flags & DB_FORUM_FLAGS_THREADFREE ) )
  {
    fclose( file );
    return -3;
  }

  file_s( file, 16+sizeof(dbForumForumDef) + thread * ( sizeof(dbForumThreadDef) + 8 ) );
  threadd.flags |= DB_FORUM_FLAGS_THREADFREE;
  a = -1;
  file_w( &a, 1, sizeof(int), file );
  file_w( &lfree, 1, sizeof(int), file );
  file_w( &threadd, 1, sizeof(dbForumThreadDef), file );

  if( lprev != -1 ) {
    file_s( file, 16+sizeof(dbForumForumDef) + lprev * ( sizeof(dbForumThreadDef) + 8 ) + 4 );
  } else {
    file_s( file, 4 );
  }
  file_w( &lnext, 1, sizeof(int), file );
  if( lnext != -1 )
  {
    file_s( file, 16+sizeof(dbForumForumDef) + lnext * ( sizeof(dbForumThreadDef) + 8 ) );
    file_w( &lprev, 1, sizeof(int), file );
  }
  file_s( file, 8 );
  file_w( &thread, 1, sizeof(int), file );

  file_s( file, 0 );
  num--;
  file_w( &num, 1, sizeof(int), file );

  file_s( file, 16 );
  forumd.threads--;
  file_w( &forumd, 1, sizeof(dbForumForumDef), file );
  fclose( file );
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/thread%d", settings->string_value, forum, thread );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/thread%d", settings->string_value, forum, thread );
}
  unlink( fname );
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forums", settings->string_value );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forums", settings->string_value );
}	
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
  file_r( &num, 1, sizeof(int), file );
  if( forum < num )
  {
    file_s( file, 4+forum*sizeof(dbForumForumDef) );
    file_r( &forumd, 1, sizeof(dbForumForumDef), file );
    forumd.threads--;
    file_s( file, 4+forum*sizeof(dbForumForumDef) );
    file_w( &forumd, 1, sizeof(dbForumForumDef), file );
  }
  fclose( file );

  return num;
}


int dbForumAddPost( int flags, int forum, int thread, dbForumPostPtr postd ) {
	ConfigArrayPtr settings;
	int a, num, offset, lused, lprev, lnext;
	FILE *file;
	char fname[PATH_MAX];
	dbForumThreadDef threadd;
	dbForumForumDef forumd;
	
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/thread%d", settings->string_value, forum, thread );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/thread%d", settings->string_value, forum, thread );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
  num++;
  file_s( file, 0 );
  file_w( &num, 1, sizeof(int), file );
file_r( &offset, 1, sizeof(int), file );

file_r( &threadd, 1, sizeof(dbForumThreadDef), file );
  file_s( file, 8 );
  threadd.time = postd->post.time;
  threadd.tick = postd->post.tick;
  threadd.posts++;
  file_w( &threadd, 1, sizeof(dbForumThreadDef), file );

  file_s( file, offset );
  a = offset + ( 4 + sizeof(dbForumPostInDef) + (postd->post).length );
  file_w( &a, 1, sizeof(int), file );
  file_w( &(postd->post), 1, sizeof(dbForumPostInDef), file );
  file_w( postd->text, 1, (postd->post).length, file );

  file_s( file, 4 );
  file_w( &a, 1, sizeof(int), file );

  fclose( file );
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, forum );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
  file_s( file, 4 );
file_r( &lused, 1, sizeof(int), file );

  file_s( file, 16+sizeof(dbForumForumDef) + thread * ( sizeof(dbForumThreadDef) + 8 ) );
file_r( &lprev, 1, sizeof(int), file );
file_r( &lnext, 1, sizeof(int), file );
  file_w( &threadd, 1, sizeof(dbForumThreadDef), file );

  if( lused != thread )
  {
    if( lprev != -1 )
    {
      file_s( file, 16+sizeof(dbForumForumDef) + lprev * ( sizeof(dbForumThreadDef) + 8 ) + 4 );
      file_w( &lnext, 1, sizeof(int), file );
    }
    if( lnext != -1 )
    {
      file_s( file, 16+sizeof(dbForumForumDef) + lnext * ( sizeof(dbForumThreadDef) + 8 ) );
      file_w( &lprev, 1, sizeof(int), file );
    }
    file_s( file, 16+sizeof(dbForumForumDef) + thread * ( sizeof(dbForumThreadDef) + 8 ) );
    a = -1;
    file_w( &a, 1, sizeof(int), file );
    file_w( &lused, 1, sizeof(int), file );
    if( lused != -1 )
    {
      file_s( file, 16+sizeof(dbForumForumDef) + lused * ( sizeof(dbForumThreadDef) + 8 ) );
      file_w( &thread, 1, sizeof(int), file );
    }
    file_s( file, 4 );
    file_w( &thread, 1, sizeof(int), file );
  }

  fclose( file );
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forums", settings->string_value );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forums", settings->string_value );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
  if( forum < num )
  {
    file_s( file, 4+forum*sizeof(dbForumForumDef) );
    file_r( &forumd, 1, sizeof(dbForumForumDef), file );
    forumd.time = threadd.time;
    forumd.tick = threadd.tick;
    file_s( file, 4+forum*sizeof(dbForumForumDef) );
    file_w( &forumd, 1, sizeof(dbForumForumDef), file );
  }
  fclose( file );

  return 1;
}

int dbForumRemovePost( int flags, int forum, int thread, int post ) {
	ConfigArrayPtr settings;
	int a, num, offset, offset2;
	FILE *file;
	char fname[PATH_MAX];
	dbForumThreadDef threadd;
	dbForumPostDef postd;

if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/thread%d", settings->string_value, forum, thread );
} else {
	settings = GetSetting( "Public Forum" );
	snprintf( fname, sizeof(fname), "%s/forum%d/thread%d", settings->string_value, forum, thread );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
file_r( &offset, 1, sizeof(int), file );
  if( (unsigned int)post >= num )
  {
    fclose( file );
    return -3;
  }

file_r( &threadd, 1, sizeof(dbForumThreadDef), file );
  file_s( file, 8 );
  threadd.posts--;
  file_w( &threadd, 1, sizeof(dbForumThreadDef), file );

  file_s( file, 8+sizeof(dbForumThreadDef) );
  offset = 8+sizeof(dbForumThreadDef);
  for( a = 0 ; a < num ; a++ )
  {
    if( a != post )
    {
      file_r( &offset, 1, sizeof(int), file );
      file_s( file, offset );
      continue;
    }

    file_r( &offset2, 1, sizeof(int), file );
    for( a++ ; a < num ; a++ )
    {
      file_s( file, offset2 );
      file_r( &offset2, 1, sizeof(int), file );
      file_r( &postd.post, 1, sizeof(dbForumPostInDef), file );
      if( !( postd.text = malloc( (postd.post).length ) ) )
        (postd.post).length = 0;
      file_r( postd.text, 1, (postd.post).length, file );

      file_s( file, offset );
      offset += 4 + sizeof(dbForumPostInDef) + (postd.post).length;
      file_w( &offset, 1, sizeof(int), file );
      file_w( &postd.post, 1, sizeof(dbForumPostInDef), file );
      file_w( postd.text, 1, (postd.post).length, file );
      if( postd.text )
        free( postd.text );
    }

    file_s( file, 0 );
    num--;
    file_w( &num, 1, sizeof(int), file );
    file_w( &offset, 1, sizeof(int), file );

    fclose( file );
if( flags ) {
	settings = GetSetting( "Directory" );
  	snprintf( fname, sizeof(fname), "%s/data/forum%d/threads", settings->string_value, forum );
} else {
	settings = GetSetting( "Public Forum" );
	snprintf( fname, sizeof(fname), "%s/forum%d/threads", settings->string_value, forum );
}
    if( !( file = fopen( fname, "rb+" ) ))
      return -3;
    file_s( file, 16+sizeof(dbForumForumDef) + thread * ( sizeof(dbForumThreadDef) + 8 ) + 8 );
    file_w( &threadd, 1, sizeof(dbForumThreadDef), file );
    fclose( file );

    return num;
  }

  return -3;
}

int dbForumEditPost( int flags, int forum, int thread, int post, dbForumPostPtr postd ) {
	ConfigArrayPtr settings;
	int a, b, num, offset;
	FILE *file;
	char fname[PATH_MAX];
	dbForumThreadDef threadd;
	dbForumPostPtr posts;
/*
  if( dbForumListPosts( forum, thread, post+1, FORUM_MAX, &threadd, &posts ) < 0 )
    return -3;
*/
  posts = 0;
  dbForumListPosts( flags, forum, thread, post+1, FORUM_MAX, &threadd, &posts );

if( flags ) {
	settings = GetSetting( "Directory" );
	snprintf( fname, sizeof(fname), "%s/data/forum%d/thread%d", settings->string_value, forum, thread );
} else {
	settings = GetSetting( "Public Forum" );
  	snprintf( fname, sizeof(fname), "%s/forum%d/thread%d", settings->string_value, forum, thread );
}
  if( !( file = fopen( fname, "rb+" ) ))
    return -3;
file_r( &num, 1, sizeof(int), file );
file_r( &offset, 1, sizeof(int), file );
  if( (unsigned int)post >= num )
  {
    if( posts )
      free( posts );
    fclose( file );
    return -3;
  }

  file_s( file, 8+sizeof(dbForumThreadDef) );
  offset = 8+sizeof(dbForumThreadDef);
  for( a = 0 ; a < num ; a++ )
  {
    if( a != post )
    {
      file_r( &offset, 1, sizeof(int), file );
      file_s( file, offset );
      continue;
    }

    file_s( file, offset );
    offset += 4 + sizeof(dbForumPostInDef) + (postd->post).length;
    file_w( &offset, 1, sizeof(int), file );
    file_w( &postd->post, 1, sizeof(dbForumPostInDef), file );
    file_w( postd->text, 1, (postd->post).length, file );

    for( b = 0, a++ ; a < num ; a++, b++ )
    {
      file_s( file, offset );
      offset += 4 + sizeof(dbForumPostInDef) + (posts[b].post).length;
      file_w( &offset, 1, sizeof(int), file );
      file_w( &posts[b].post, 1, sizeof(dbForumPostInDef), file );
      file_w( posts[b].text, 1, (posts[b].post).length, file );
      if( posts[b].text )
        free( posts[b].text );
    }
    if( posts )
      free( posts );

    file_s( file, 4 );
    file_w( &offset, 1, sizeof(int), file );

    fclose( file );
    return num;
  }

  if( posts )
    free( posts );
  return -3;
}




int dbMailList( int id, int type, int base, int end, dbMailPtr *mails, int *rtnum ) {
	int a, b;
	int64_t num, offset;
	dbMailPtr mailsp;
	FILE *file;

if( ( type & 0xFFFFFFE ) )
	return -3;
if( !( file = dbFileUserOpen( id, DB_FILE_USER_MAILIN+type ) ) ) {
	error( "User Open" );
	return -3;
}

file_r( &num, 1, sizeof(int64_t), file );

if( rtnum )
	*rtnum = (int)num;
if( end < num )
	num = end;
if( base > num ) {
	fclose( file );
	return -3;
}

if( !( mailsp = malloc( num * sizeof(dbMailDef) ) ) )
	return -3;

if( base == num ) {
	fclose( file );
	*mails = mailsp;
	return 0;
}

offset = 2 * sizeof(int64_t);

for( a = 0 ; a < base ; a++ ) {
	file_s( file, offset );
	file_r( &offset, 1, sizeof(int64_t), file );
}

for( b = 0 ; a < num ; a++, b++ ) {
	file_s( file, offset );
	file_r( &offset, 1, sizeof(int64_t), file );
	file_r( &mailsp[b].mail, 1, sizeof(dbMailInDef), file );
	mailsp[b].text = 0;
	if( (unsigned int)((mailsp[b].mail).length) >= MAIL_MAX )
		continue;
	if( !( mailsp[b].text = malloc( (mailsp[b].mail).length + 1 ) ) )
		continue;
	file_r( mailsp[b].text, 1, (mailsp[b].mail).length, file );
	mailsp[b].text[ (mailsp[b].mail).length ] = 0;
}

*mails = mailsp;
fclose( file );

return b;
}

int dbMailAdd( int id, int type, dbMailPtr maild ) {
	int64_t a, num, offset;
	FILE *file;

if( ( type & 0xFFFFFFE ) )
	return -3;
if( !( file = dbFileUserOpen( id, DB_FILE_USER_MAILIN+type ) ) ) {
	error( "User Open" );
	return -3;
}

file_r( &num, 1, sizeof(int64_t), file );

if( num ) {
	file_r( &offset, 1, sizeof(int64_t), file );
} else {
	offset = 2 * sizeof(int64_t);
}

num++;
file_s( file, 0 );
file_w( &num, 1, sizeof(int64_t), file );



file_s( file, offset );
a = offset + ( sizeof(int64_t) + sizeof(dbMailInDef) + (maild->mail).length );
file_w( &a, 1, sizeof(int64_t), file );
file_w( &(maild->mail), 1, sizeof(dbMailInDef), file );
file_w( maild->text, 1, (maild->mail).length, file );

file_s( file, sizeof(int64_t) );
file_w( &a, 1, sizeof(int64_t), file );
fclose( file );


return 1;
}



int dbMailRemove( int id, int type, int message ) {
	int a;
	int64_t num, offset, offset2;
	dbMailDef maild;
	FILE *file;

if( ( type & 0xFFFFFFE ) )
	return -3;
if( !( file = dbFileUserOpen( id, DB_FILE_USER_MAILIN+type ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &num, 1, sizeof(int64_t), file );
file_r( &offset, 1, sizeof(int64_t), file );

if( (unsigned int)message >= num ) {
	fclose( file );
	return -3;
}

offset = 2 * sizeof(int64_t);
file_s( file, offset );

for( a = 0 ; a < num ; a++ ) {
	if( a != message ) {
		file_r( &offset, 1, sizeof(int64_t), file );
		file_s( file, offset );
		continue;
	}

	file_r( &offset2, 1, sizeof(int64_t), file );
	
	for( a++ ; a < num ; a++ ) {
		file_s( file, offset2 );
		file_r( &offset2, 1, sizeof(int64_t), file );
		file_r( &maild.mail, 1, sizeof(dbMailInDef), file );
		if( !( maild.text = malloc( (maild.mail).length ) ) )
			(maild.mail).length = 0;
		file_r( maild.text, 1, (maild.mail).length, file );

		file_s( file, offset );
		offset += sizeof(int64_t) + sizeof(dbMailInDef) + (maild.mail).length;
		file_w( &offset, 1, sizeof(int64_t), file );
		file_w( &maild.mail, 1, sizeof(dbMailInDef), file );
		file_w( maild.text, 1, (maild.mail).length, file );
		if( maild.text )
			free( maild.text );
	}

	file_s( file, 0 );
	num--;
	file_w( &num, 1, sizeof(int64_t), file );
	file_w( &offset, 1, sizeof(int64_t), file );

	fclose( file );
	return num;
}


return -3;
}

int dbMailEmpty( int id, int type ) {
	FILE *file;

if( ( type & 0xFFFFFFE ) )
	return -3;
if( !( file = dbFileUserOpen( id, DB_FILE_USER_MAILIN+type ) ) ) {
	error( "User Open" );
	return -3;
}

file_w( dbFileUserListData[DB_FILE_USER_MAILIN+type], 1, dbFileUserListBase[DB_FILE_USER_MAILIN+type], file );
fclose( file );
  
return 1;
}








int dbUserSpecOpAdd( int id, dbUserSpecOpPtr specopd )
{
  int pos;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_SPECOPS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &pos, 1, sizeof(int), file );
  file_s( file, 4+(pos*sizeof(dbUserSpecOpDef)) );
  file_w( specopd, 1, sizeof(dbUserSpecOpDef), file );
  file_s( file, 0 );
  pos++;
  file_w( &pos, 1, sizeof(int), file );
  fclose( file );
  return (pos-1);
}

int dbUserSpecOpRemove( int id, int specopid )
{
  int a, num;
  dbUserSpecOpDef data;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_SPECOPS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)specopid >= num )
  {
    fclose( file );
    return -2;
  }
  if( specopid+1 < num )
  {
    file_s( file, 4+(num*sizeof(dbUserSpecOpDef))-sizeof(dbUserSpecOpDef) );
    file_r( &data, 1, sizeof(dbUserSpecOpDef), file );
    file_s( file, 4+(specopid*sizeof(dbUserSpecOpDef)) );
    file_w( &data, 1, sizeof(dbUserSpecOpDef), file );
  }
  file_s( file, 0 );
  a = num-1;
  file_w( &a, 1, sizeof(int), file );
  fclose( file );
  return 1;
}

int dbUserSpecOpList( int id, dbUserSpecOpPtr *specopd )
{
  int num;
  dbUserSpecOpPtr specopp;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_SPECOPS ) ) ) {
  	error( "User Open" );
    return -3;
    }
file_r( &num, 1, sizeof(int), file );
  if( !( specopp = malloc( num*sizeof(dbUserSpecOpDef) ) ) )
  {
    fclose( file );
    return -1;
  }
if( num )
	file_r( specopp, 1, num*sizeof(dbUserSpecOpDef), file );
  fclose( file );
  *specopd = specopp;
  return num;
}

int dbUserSpecOpSet( int id, int specopid, dbUserSpecOpPtr specopd )
{
  int num;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_SPECOPS ) ) ) {
  	error( "User Open" );
    return -3;
    }
file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)specopid >= num )
  {
    fclose( file );
    return -2;
  }
  file_s( file, 4+(specopid*sizeof(dbUserSpecOpDef)) );
  file_w( specopd, 1, sizeof(dbUserSpecOpDef), file );
  fclose( file );
  return num;
}

int dbUserSpecOpRetrieve( int id, int specopid, dbUserSpecOpPtr specopd )
{
  int num;
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_SPECOPS ) ) ) {
  	error( "User Open" );
    return -3;
    }
file_r( &num, 1, sizeof(int), file );
  if( (unsigned int)specopid >= num )
  {
    fclose( file );
    return -2;
  }
  file_s( file, 4+(specopid*sizeof(dbUserSpecOpDef)) );
file_r( specopd, 1, sizeof(dbUserSpecOpDef), file );
  fclose( file );
  return num;
}

int dbUserSpecOpEmpty( int id )
{
  FILE *file;
  if( !( file = dbFileUserOpen( id, DB_FILE_USER_SPECOPS ) ) ) {
  	error( "User Open" );
    return -3;
    }
  file_w( dbFileUserListData[DB_FILE_USER_SPECOPS], 1, dbFileUserListBase[DB_FILE_USER_SPECOPS], file );
  fclose( file );
  return 1;
}










int dbUserInfoSet( int id, dbUserInfoPtr infod ) {
	FILE *file;
	dbUserPtr user;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_INFO ) ) ) {
	error( "User Open" );
	return -3;
}
file_s( file, 0 );
file_w( infod, 1, sizeof(dbUserInfoDef), file );
fclose( file );

if( !( user = dbUserLinkID( id ) ) )
	return -3;

strncpy( user->faction, infod->faction, sizeof(user->faction) );
strncpy( user->forumtag, infod->forumtag, sizeof(user->forumtag) );
#if FACEBOOK_SUPPORT
if( bitflag( user->flags, CMD_USER_FLAGS_FBLINK ) || bitflag( user->flags, CMD_USER_FLAGS_FBMADE ) )
	strncpy( user->fbid, infod->fbinfo.id, sizeof(user->fbid) );
#endif

return 1;
}

int dbUserInfoRetrieve( int id, dbUserInfoPtr infod ) {
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_INFO ) ) ) {
	error( "User Open" );
	return -3;
}

file_s( file, 0 );
file_r( infod, 1, sizeof(dbUserInfoDef), file );
fclose( file );

return 1;
}

int dbUserRecordAdd( int id, dbUserRecordPtr recordd ) {
	int num = 0;
	FILE *file;

if( !( file = dbFileUserOpen( id, DB_FILE_USER_RECORD ) ) ) {
	error( "User Open" );
	return -3;
}

file_r( &num, 1, 4, file );
num++;
file_w( &num, 1, 4, file );
file_s( file, 4 + ( num - 1 ) * sizeof(dbUserRecordDef) );
file_w( recordd, 1, sizeof(dbUserRecordDef), file );
fclose( file );


return num;
}

int dbUserRecordList( int id, dbUserRecordPtr *records ) {
	int num = 0;
	dbUserRecordPtr recordp;
	FILE *file;
if( !( file = dbFileUserOpen( id, DB_FILE_USER_RECORD ) ) ) {
	error( "User Open" );
	return -3;
}
file_r( &num, 1, 4, file );

if( !( recordp = malloc( num * sizeof(dbUserRecordDef) ) ) )
	return -3;
if( num )
	file_r( recordp, 1, num * sizeof(dbUserRecordDef), file );

fclose( file );
*records = recordp;


return num;
}







