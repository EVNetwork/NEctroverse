#ifndef DB_H
#define DB_H
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.
extern char *dbFileList[DB_FILE_BASE_TOTAL];
extern FILE *dbFilePtr[DB_FILE_BASE_TOTAL];

typedef struct
{
  int id;
  int level;
  int flags;
  int newmail;
  int lasttime;

  char name[USER_NAME_MAX];
  char faction[USER_NAME_MAX];
  char forumtag[USER_FTAG_MAX];

  char http_session[SESSION_SIZE];

  #if IRCBOT_SUPPORT
  char irc_session[SESSION_SIZE];
  #endif
  //Facebook Linkage
  #if FACEBOOK_SUPPORT
  char fbid[USER_NAME_MAX];
  #endif

  //Pointers for next/last user in list.
  void *next;
  void **prev;
} dbUserDef, *dbUserPtr;

#if FACEBOOK_SUPPORT

typedef struct {
	int expire;
	char val[DEFAULT_BUFFER];
} FBTokenDef, *FBTokenPtr;

typedef struct {
	bool connected;
	//Facebook Notifications
	bool notice_mail;
	bool notice_build;
	bool notice_fleet;
	bool notice_trade;
	//Facebook UserData
	double timezone;
	char id[USER_NAME_MAX];
	char gender[16];
	char full_name[USER_NAME_MAX*3];
	char first_name[USER_NAME_MAX];
	char last_name[USER_NAME_MAX];
	char pic[PATH_MAX];
	char bio[USER_DESC_MAX];
	//Access Token Data
	time_t updated;
	FBTokenDef token;
} FBUserDef, *FBUserPtr;

#endif

typedef struct
{
  int id;
  int rank;
  int level;
  int flags;
  int createtime;
  int lasttime;
  int tagpoints;
  //int forumposts;
  int forumtagtype;
  char name[USER_NAME_MAX];
  char faction[USER_NAME_MAX];
  char email[USER_NAME_MAX];
  char password[USER_PASS_MAX];
  char http_session[SESSION_SIZE];
  char forumtag[USER_FTAG_MAX];
  char desc[USER_DESC_MAX];
  struct in_addr sin_addr[MAXIPRECORD];
  #if FACEBOOK_SUPPORT
  FBUserDef fbinfo;
  #endif
} dbUserInfoDef, *dbUserInfoPtr;


int dbUserInfoSet( int id, dbUserInfoPtr info );
int dbUserInfoRetrieve( int id, dbUserInfoPtr info );

extern dbUserPtr dbUserList;
extern int dbRegisteredInfo[DB_TOTALS_USERS_NUMUSED];

int dbInit();
void dbEnd();
void dbFlush();

int dbMapFindSystem( int x, int y );
int dbMapFindValid( int x, int y );


int dbUserSearch( char *name );
int dbUserFBSearch( char *name );
int dbUserSessionSearch( char *cookie );
int dbUserSearchFaction( char *name );
int dbUserAdd( dbUserInfoPtr adduser );
int dbUserRemove( int id );
dbUserPtr dbUserLinkID( int id );

int dbUserSave( int id, dbUserPtr user );

int dbUserSetPassword( int id, char *pass );
int dbUserRetrievePassword( int id, char *pass );
int dbUserLinkDatabase( void *cnt, int id );

int dbSessionSet( dbUserPtr user, char *session );
int dbSessionRetrieve( dbUserPtr user, char *session );


typedef struct
{
  char faction[USER_NAME_MAX];
  int64_t ressource[CMD_RESSOURCE_NUMUSED+2];
  int empire;
  int64_t infos[INFOS_TOTAL_NUMUSED];
  int64_t research[CMD_RESEARCH_NUMUSED];
  int readiness[CMD_READY_NUMUSED];
  int home; // ( y << 20 ) + ( x << 8 ) + planet
  int64_t totalbuilding[CMD_BLDG_NUMUSED+1];
  int64_t totalunit[CMD_UNIT_NUMUSED];
  int64_t totalresearch[CMD_RESEARCH_NUMUSED];
  int64_t networth;
  int allocresearch[CMD_RESEARCH_NUMUSED];
  int64_t fundresearch;
  int planets;
  int config_fleet;
  int config_flee[4];
  int config_mapsize;
  int config_map[8];
  int raceid;
  int artefacts;
  int rank;
  int benefactors[2];
  int benefactor[5];
  float autofund[3];
} dbUserMainDef, *dbUserMainPtr;

int dbUserMainSet( int id, dbUserMainPtr main );

#define dbUserMainRetrieve( id, maind ) dbUserMainRetrieve_real( id, maind, __FILE__, __LINE__ )
int dbUserMainRetrieve_real( int id, dbUserMainPtr main, char *sourcefile, int sourceline );




typedef struct
{
  int type;
  int time;
  int plnid;
  int plnpos;
  int64_t quantity;
  int64_t cost[CMD_RESSOURCE_NUMUSED];
} dbBuildDef, *dbBuildPtr;

#define dbUserBuildAdd( id, type, cost, quantity, time, plnid, plnloc ) dbBuildAdd( YES, id, type, cost, quantity, time, plnid, plnloc );
#define dbEmpireBuildAdd( id, type, cost, quantity, time, plnid, plnloc ) dbBuildAdd( NO, id, type, cost, quantity, time, plnid, plnloc );
int dbBuildAdd( bool isuser, int id, int type, int64_t *cost, int64_t quantity, int time, int plnid, int plnloc );

#define dbUserBuildRemove( id, bldid ) dbBuildRemove( YES, id, bldid );
#define dbEmpireBuildRemove( id, bldid ) dbBuildRemove( NO, id, bldid );
int dbBuildRemove( bool isuser, int id, int bldid );

#define dbUserBuildList( id, build ) dbBuildList( YES, id, build );
#define dbEmpireBuildList( id, build ) dbBuildList( NO, id, build );
int dbBuildList( bool isuser, int id, dbBuildPtr *build );

#define dbUserBuildListReduceTime( id, build ) dbBuildListReduceTime( YES, id, build );
#define dbEmpireBuildListReduceTime( id, build ) dbBuildListReduceTime( NO, id, build );
int dbBuildListReduceTime( bool isuser, int id, dbBuildPtr *build );

#define dbUserBuildEmpty( id ) dbBuildEmpty( YES, id );
#define dbEmpireBuildEmpty( id ) dbBuildEmpty( NO, id );
int dbBuildEmpty( bool isuser, int id );



int dbUserPlanetNumber( int id );
int dbUserPlanetAdd( int id, int plnid, int sysid, int plnloc, int flags );
int dbUserPlanetRemove( int id, int plnid );

int dbUserPlanetSetFlags( int id, int plnid, int flags );
int dbUserPlanetListIndicesSorted( int id, int **list, int sort );
int dbUserPlanetListIndices( int id, int **list );
int dbUserPlanetListCoords( int id, int **list );
int dbUserPlanetListSystems( int id, int **list );
int dbUserPlanetListFull( int id, int **list );

int dbUserPortalsList( int id, int **list );
int dbUserPortalsListIndices( int id, int **list );
int dbUserPortalsListCoords( int id, int **list );


typedef struct
{
  int64_t unit[CMD_UNIT_NUMUSED];
  int order;
  int destination;
  int destid;
  int sysid;
  int source;
  int flags;
  int time;
  int basetime;
} dbUserFleetDef, *dbUserFleetPtr;

int dbUserFleetAdd( int id, dbUserFleetPtr fleetd );
int dbUserFleetRemove( int id, int fltid );
int dbUserFleetList( int id, dbUserFleetPtr *fleetd );
int dbUserFleetSet( int id, int fltid, dbUserFleetPtr fleetd );
int dbUserFleetRetrieve( int id, int fltid, dbUserFleetPtr fleetd );



int dbUserNewsAdd( int id, int64_t *data, int64_t flags );
int64_t dbUserNewsGetFlags( int id );
int dbUserNewsList( int id, int64_t **data );
int64_t dbUserNewsListUpdate( int id, int64_t **data, int64_t time );
int dbUserNewsEmpty( int id );



int dbFamNewsAdd( int id, int64_t *data );
int dbFamNewsList( int id, int64_t **data, int time );



int dbEmpireMessageSet( int id, int num, char *text );
int dbEmpireMessageRetrieve( int id, int num, char *text );


int dbMapRetrieveMain( int *binfo );
int dbMapSetMain( int *binfo );

extern int dbMapBInfoStatic[MAP_TOTAL_INFO];

int dbMapSetSystem( int sysid, dbMainSystemPtr systemd );
int dbMapRetrieveSystem( int sysid, dbMainSystemPtr systemd );


int dbMapSetPlanet( int plnid, dbMainPlanetPtr planetd );
int dbMapRetrievePlanet( int plnid, dbMainPlanetPtr planetd );

typedef struct
{
  int tick;
  int type;
  int empire;
  int flags;
} dbEmpireRelationsDef, *dbEmpireRelationsPtr;

int dbEmpireSetInfo( int famid, dbMainEmpirePtr empired );
int dbEmpireGetInfo( int famid, dbMainEmpirePtr empired );



int dbEmpireRelsAdd( int id, int *rel );
int dbEmpireRelsRemove( int id, int relid );
int dbEmpireRelsList( int id, int **rel );
int dbEmpireRelsGet( int id, int relid, int *rel );


////////


typedef struct
{
   int nummaxbids;
   int listfree;
   int totalofbids;

   int firstid;
   int lastid;
} dbMarketIndexDef, *dbMarketIndexPtr;

typedef struct
{
   int user;
   int next;
   int previous;
   int64_t quantity;
} dbMarketBidDef, *dbMarketBidPtr;

typedef struct
{  
  int id;
  int price;
  int action;
  int resource;
  int quantity;
//  int64_t quantity;
} dbMarketUserDef, *dbMarketUserPtr;

int dbMarketReset();
int dbMarketFull( int *list );
int dbMarketAdd( int *bid );
int dbMarketRemove( int *bid, int lcur );
int dbMarketListStart( int *bid );
int dbMarketListNext( int lcur, int *result );
int dbMarketSetQuantity( int *bid, int lcur, int quantity, int loss );

int dbUserMarketReset( int id );
int dbUserMarketAdd( int id, int bidid, int action, int resource, int price, int quantity );
int dbUserMarketList( int id, int **list );
int dbUserMarketQuantity( int id, int bidid, int quantity );
int dbUserMarketRemove( int id, int bidid );



typedef struct
{
  int length;
  int tick;
  int flags;
  int authorid;
  int authorempire;
  char authorname[USER_NAME_MAX];
  time_t time;
} dbMailInDef, *dbMailInPtr;

typedef struct
{
  char *text;
  dbMailInDef mail;
} dbMailDef, *dbMailPtr;

int dbMailList( int id, int type, int base, int end, dbMailPtr *mails, int *rtnum );
int dbMailAdd( int id, int type, dbMailPtr maild );
int dbMailRemove( int id, int type, int message );
int dbMailEmpty( int id, int type );









typedef struct
{
  int type; // 0xF000=ops batch  | 0x10000 if casted by someone on the current faction
  int plnid; // planet ID, if applicable
  int plnpos; // planet position, if applicable  ( y << 20 ) + ( x << 8 ) + planet
  int factionid; // target faction, or where the op comes from
  int time;
  int vars[3];
} dbUserSpecOpDef, *dbUserSpecOpPtr;



int dbUserSpecOpAdd( int id, dbUserSpecOpPtr specopd );
int dbUserSpecOpRemove( int id, int specopid );
int dbUserSpecOpList( int id, dbUserSpecOpPtr *specopd );
int dbUserSpecOpSet( int id, int specopid, dbUserSpecOpPtr specopd );
int dbUserSpecOpRetrieve( int id, int specopid, dbUserSpecOpPtr specopd );
int dbUserSpecOpEmpty( int id );







typedef struct
{
  int roundid;
  char roundflag[USER_NAME_MAX];
  int rank;
  int planets;
  int64_t networth;
  char faction[USER_NAME_MAX];
  char forumtag[USER_FTAG_MAX];

  int empire;
  int famplanets;
  int64_t famnetworth;
  char famname[USER_NAME_MAX];
  int famrank;
  int artefacts;
  int famflags;

  int reseved[16];
} dbUserRecordDef, *dbUserRecordPtr;

int dbUserRecordAdd( int id, dbUserRecordPtr recordd );
int dbUserRecordList( int id, dbUserRecordPtr *records );




/////////////////////////////////////////////////////////////////////////////////

extern int dbArtefactPos[ARRAY_MAX];

extern int dbArtefactMax;


enum 
{
DB_FILE_EMPIRE_INFO,
DB_FILE_EMPIRE_BUILD,
DB_FILE_EMPIRE_NEWS,
DB_FILE_EMPIRE_MESSAGES,
DB_FILE_EMPIRE_RELATIONS,

DB_FILE_EMPIRE_TOTAL,
};


FILE *dbFileEmpireOpen( int id, int num );

int dbEmpireSetMessage( int famid, dbEmpireMessagePtr message );
int dbEmpireGetMessage( int famid, dbEmpireMessagePtr message );

#endif
