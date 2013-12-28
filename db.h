#ifndef DATABASEINCLUDES
#define DATABASEINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct
{
  int id;
  int level;
  int flags;
  int reserved;
  char name[64];

  //fast access informations -- copied from maind for now. FIXME
  char faction[32];
  char forumtag[256];
  char http_session[129];
  char irc_session[129];
  int lasttime;

  //Pointers for next/last user in list.
  void *next;
  void **prev;
} dbUserDef, *dbUserPtr;

typedef struct
{
  int id;
  int rank;
  int level;
  int flags;
  int createtime;
  int lasttime;
  int tagpoints;
  int forumtagtype;
  char name[64];
  char password[128];
  char email[128];
  char faction[32];
  char http_session[129];
  char irc_session[129];
  char forumtag[256];
  char desc[4096];
  struct in_addr sin_addr[MAXIPRECORD];
} dbUserInfoDef, *dbUserInfoPtr;


int dbUserInfoSet( int id, dbUserInfoPtr info );
int dbUserInfoRetrieve( int id, dbUserInfoPtr info );

extern dbUserPtr dbUserList;


int dbInit();
void dbEnd();
void dbFlush();

int dbMapFindSystem( int x, int y );
int dbMapFindValid( int x, int y );


int dbUserSearch( char *name );
int dbUserSessionSearch( char *cookie );
int dbUserSearchFaction( char *name );
int dbUserAdd( dbUserInfoPtr adduser );
int dbUserRemove( int id );
dbUserPtr dbUserLinkID( int id );

int dbUserSave( int id, dbUserPtr user );

int dbUserSetPassword( int id, char *pass );
int dbUserRetrievePassword( int id, char *pass );
int dbUserLinkDatabase( void *, int id );

int dbSessionSet( dbUserPtr user, char *session );
int dbSessionRetrieve( dbUserPtr user, char *session );


typedef struct
{
  char faction[32];
  int64_t ressource[CMD_RESSOURCE_NUMUSED+2];
  int empire;
  int64_t infos[INFOS_TOTAL_NUMUSED];
  int64_t research[CMD_RESEARCH_NUMUSED];
  int readiness[3];
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
int dbUserMainRetrieve( int id, dbUserMainPtr main );




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
  int unit[CMD_UNIT_NUMUSED];
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
  int jumpgate;
} dbMainSystemDef, *dbMainSystemPtr;

int dbMapSetSystem( int sysid, dbMainSystemPtr systemd );
int dbMapRetrieveSystem( int sysid, dbMainSystemPtr systemd );


typedef struct
{
  int system;
  int position;
  int owner;
  int size;
  int flags;
  double population;
  int maxpopulation;
  int special[3];
  int building[CMD_BLDG_NUMUSED];
  int unit[CMD_UNIT_NUMUSED];
  int construction;
  int protection;
  int surrender;
  int nuked;
} dbMainPlanetDef, *dbMainPlanetPtr;

int dbMapSetPlanet( int plnid, dbMainPlanetPtr planetd );
int dbMapRetrievePlanet( int plnid, dbMainPlanetPtr planetd );


typedef struct
{
  int rank;
  int numplayers;
  int polotics[4];
  int player[128];
  int vote[128];
  int leader;
  int homeid;
  int homepos; // ( y << 16 ) + x
  int picmime;
  int pictime;
  int planets;
  int networth;
  int artefacts;
  int construction;
  int building[8];
  float taxation;
  int64_t fund[CMD_RESSOURCE_NUMUSED];
  int64_t infos[CMD_RESSOURCE_NUMUSED];
  char name[64];
  char password[128];
  char message[2][4096];
} dbMainEmpireDef, *dbMainEmpirePtr;

typedef struct
{
  int tick;
  int type;
  int empire;
  int flags;
} dbEmpireRelationsDef, *dbEmpireRelationsPtr;

int dbMapSetEmpire( int famid, dbMainEmpirePtr empired );
int dbMapRetrieveEmpire( int famid, dbMainEmpirePtr empired );



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
  int authorid;
  char authorname[64];
  int authorempire;
  int time;
  int tick;
  int flags;
  int reserved[2];
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

  int planets;
  int networth;
  char faction[32];
  char forumtag[32];
  int rank;

  int empire;
  int famplanets;
  int famnetworth;
  char famname[64];
  int famrank;
  int artefacts;
  int famflags;

  int reseved[16];
} dbUserRecordDef, *dbUserRecordPtr;

int dbUserRecordAdd( int id, dbUserRecordPtr recordd );
int dbUserRecordList( int id, dbUserRecordPtr *records );




/////////////////////////////////////////////////////////////////////////////////

extern char *dbImageDirs[HTTP_DIR_TOTAL];

extern int dbArtefactPos[ARTEFACT_NUMUSED];

extern int dbArtefactMax;


#endif
