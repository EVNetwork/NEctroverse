#ifndef DATABASEINCLUDES
#define DATABASEINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

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
  int aidaccess;
  int flags;
} dbUserMainDef, *dbUserMainPtr;

int dbUserMainSet( int id, dbUserMainPtr main );

#define dbUserMainRetrieve( id, maind ) dbUserMainRetrieve_real( id, maind, __FILE__, __LINE__ )
int dbUserMainRetrieve_real( int id, dbUserMainPtr main, char *sourcefile, int sourceline );




typedef struct
{
  int type;
  int quantity;
  int time;
  int plnid;
  int plnpos;
  int64_t cost[CMD_RESSOURCE_NUMUSED];
} dbUserBuildDef, *dbUserBuildPtr;

int dbUserBuildAdd( int id, int type, int64_t *cost, int quantity, int time, int plnid, int plnloc );
int dbUserBuildRemove( int id, int bldid );
int dbUserBuildList( int id, dbUserBuildPtr *build );
int dbUserBuildListReduceTime( int id, dbUserBuildPtr *build );
int dbUserBuildEmpty( int id );



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



typedef struct
{
  int sizex;
  int sizey;
  int systems;
  int planets;
  int families;
  int fmembers;
  int capacity;
  int artitimer;
  int timempire;
} dbMainMapDef, *dbMainMapPtr;

int dbMapRetrieveMain( int *binfo );
int dbMapSetMain( int *binfo );

extern int dbMapBInfoStatic[MAP_TOTAL_INFO];


typedef struct
{
  int position;
  int indexplanet;
  int numplanets;
  int empire;
  int unexplored;
  int flags;
  int timer;
} dbMainSystemDef, *dbMainSystemPtr;

int dbMapSetSystem( int sysid, dbMainSystemPtr systemd );
int dbMapRetrieveSystem( int sysid, dbMainSystemPtr systemd );


typedef struct
{
  int flags;
  int size;
  int system;
  int position;
  int64_t population;
  int64_t maxpopulation;
  int64_t unit[CMD_UNIT_NUMUSED];
  int64_t building[CMD_BLDG_NUMUSED];
  int construction;
  int protection;
  int special[5];
  int surrender;
  int nuked;
  int owner;
  int timer;
} dbMainPlanetDef, *dbMainPlanetPtr;

int dbMapSetPlanet( int plnid, dbMainPlanetPtr planetd );
int dbMapRetrievePlanet( int plnid, dbMainPlanetPtr planetd );
/*
 * The use of ARRAY_MAX in here, is un-desirable... but it prevents any future over-flow as its limit is 65k 
 * This is the same limit as max players, so each Empire is capable of holding all players... technicly. Not that it should.
 */
 
typedef struct
{
  char leader[USER_DESC_MAX];
  char moc[USER_DESC_MAX];
  char mod[USER_DESC_MAX];
  char mow[USER_DESC_MAX];
} dbEmpireMessageDef, *dbEmpireMessagePtr;

typedef struct
{
  int rank;
  int flags;
  int numplayers;
  int politics[CMD_EMPIRE_POLITICS_TOTAL];
  int player[ARRAY_MAX];
  int vote[ARRAY_MAX];
  int leader;
  int homeid;
  int homepos; // ( y << 16 ) + x
  int picture;
  int planets;
  int artefacts;
  int construction;
  int building[8];
  int counters[16];
  float taxation;
  int64_t networth;
  int64_t fund[CMD_RESSOURCE_NUMUSED];
  int64_t infos[CMD_RESSOURCE_NUMUSED];
  char name[USER_NAME_MAX];
  char password[USER_PASS_MAX];
} dbMainEmpireDef, *dbMainEmpirePtr;

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

extern int dbArtefactPos[ARTEFACT_NUMUSED];

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
