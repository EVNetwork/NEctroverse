#ifndef COMMANDINCLUDES
#define COMMANDINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

int cmdUserNewsAdd( int id, int64_t *data, int64_t flags );

int cmdFindDistPortal( int id, int tx, int ty, int *distptr, int *portalpos );

void cmdExecuteFlush();

int cmdEndofRound();

void cmdTickInit();

int cmdTick();

void cmdTickEnd();



int cmdExecNewUser( char *name, char *pass, char *faction );
int cmdExecNewUserEmpire( int id, int famnum, char *fampass, int raceid, int level );
int cmdExecUserDeactivate( int id, int flags );
int cmdUserDelete( int id );
int cmdExecChangeName( int id, char *faction );
int cmdExecChangePassword( int id, char *pass );

int cmdExecAddBuild( int id, int type, int quantity, int plnid, int maxbuild );
int cmdExecAddBid( int id, int action, int resource, int price, int quantity );
int cmdExecRemoveBid( int id, int bidid );
int cmdExecGetMarket( int *market );

int cmdExecSendAid( int id, int destid, int fam, int64_t *res );
int cmdExecGetAid( int id, int destid, int fam, int64_t *res );

int cmdExecAidAccess( int id, int access );
int cmdExecChangeVote( int id, int vote );
int cmdExecChangFamName( int fam, char *name );
int cmdExecFamMemberFlags( int id, int fam, int flags );
int cmdExecSetFamPass( int fam, char *pass );
int cmdExecGetFamPass( int fam, char *pass );

int cmdExecFindRelation( int fam, int famtarget, int *numallies, int flags );
int cmdExecAddRelation( int fam, int type, int famtarget );
int cmdExecDelRelation( int fam, int relid );

int cmdExecSetSectsize( int id, int size );

int cmdExploreCost( int id, dbUserMainPtr mainp );
int cmdExecSendFleetInfos( int id, int plnid, int *fr );

int cmdExecSendFleet( int id, int x, int y, int z, int order, int64_t *sendunit );
int cmdExecSendAgents( int id, int x, int y, int z, int order, int64_t agents );
int cmdExecSendGhosts( int id, int x, int y, int z, int order, int64_t ghosts );

int cmdExecExploreInfo( int id, int plnid, int *ibuffer );
int cmdExecExplore( int id, int plnid, int *ibuffer );
int cmdExecChangeFleet( int id, int x, int y, int z, int order, int fltid );


int cmdExecOfferPlanet( int id, int destid, int plnid );
int cmdExecTakePlanet( int id, int plnid );



int cmdExecute( void *DEPRECIATED, int *cmd, void *buffer, int size );

void cmdGetBuildCosts( dbUserMainPtr maind, int type, int64_t *buffer );
float cmdGetBuildOvercost( int size, int total );
void cmdGetBuildOverbuild( int size, int total, int64_t *buffer );

int cmdGetOpPenalty( int research, int requirement );

char *cmdTagFind( int points );


int specopAgentsAllowed( int specop, int raceid );
int specopPsychicsAllowed( int specop, int raceid );
int specopGhostsAllowed( int specop, int raceid );


int specopAgentsReadiness( int specop, dbUserMainPtr maind, dbUserMainPtr main2d );
void specopAgentsPerformOp( int id, int fltid, dbUserFleetPtr fleetd, int64_t *newd );
int specopPsychicsReadiness( int specop, dbUserMainPtr maind, dbUserMainPtr main2d );
void specopPsychicsPerformOp( int id, int targetid, int specop, int psychics, int64_t *newd );
int specopGhostsReadiness( int specop, dbUserMainPtr maind, dbUserMainPtr main2d );
void specopGhostsPerformOp( int id, int fltid, dbUserFleetPtr fleetd, int64_t *newd );


float specopDarkWebCalc( int id );
float specopSolarCalc( int id );
float specopWarIllusionsCalc( int id );
int specopShieldingCalc( int id, int plnid );
int specopForcefieldCalc( int id, int plnid );
int specopVortexListCalc( int id, int num, int **buffer );



int artefactPrecense( dbMainPlanetPtr planetd );





void cmdFleetGetPosition( dbUserFleetPtr fleetd, int *x, int *y );

int cmdInit();
void cmdEnd();


extern int64_t cmdFleetActionNewd[DB_USER_NEWS_BASE];



extern char *cmdErrorString;

extern char *cmdBuildingName[CMD_BLDG_NUMUSED+1];
extern char *cmdEmpireBuildingName[CMD_BLDG_EMPIRE_NUMUSED];

extern char *cmdRessourceName[CMD_RESSOURCE_NUMUSED+2];

extern char *cmdBonusName[CMD_BONUS_NUMUSED];
extern char *cmdReadyName[CMD_READY_NUMUSED];
extern char *cmdMainInfoNames[INFOS_TOTAL_NUMUSED];


extern char *cmdUnitName[CMD_UNIT_NUMUSED];
extern int64_t cmdUnitCost[CMD_UNIT_NUMUSED][CMD_RESSOURCE_NUMUSED+1];

extern char *cmdResearchName[CMD_RESEARCH_NUMUSED];
extern char *cmdResearchDesc[CMD_RESEARCH_NUMUSED];

extern int64_t cmdBuildingCost[CMD_BLDG_NUMUSED+1][CMD_RESSOURCE_NUMUSED+1];
extern int64_t cmdEmpireBuildingCost[CMD_BLDG_EMPIRE_NUMUSED][CMD_RESSOURCE_NUMUSED+1];

extern int cmdBuildingTech[CMD_BLDG_NUMUSED+1];
extern float cmdBuildingUpkeep[CMD_BLDG_NUMUSED];
extern int cmdBuildingFlags[CMD_BLDG_NUMUSED];

extern int cmdBuildingNetworth[CMD_BLDG_NUMUSED+1];
extern float cmdBuildingProduction[CMD_BLDG_NUMUSED];

extern int cmdUnitTech[CMD_UNIT_NUMUSED];
extern float cmdUnitUpkeep[CMD_UNIT_NUMUSED];
extern int cmdUnitStats[CMD_UNIT_NUMUSED][CMD_UNIT_STATS_NUMUSED];

extern char *cmdUnitStatsNames[CMD_UNIT_STATS_NUMUSED];

extern float cmdUnitPenalty[2];

extern char *cmdFleetOrderName[CMD_FLEET_ORDER_NUMUSED];



extern char *cmdAgentopName[CMD_AGENTOP_NUMUSED];
extern char *cmdPsychicopName[CMD_PSYCHICOP_NUMUSED];
extern char *cmdGhostopName[CMD_GHOSTOP_NUMUSED];

extern int cmdAgentopTech[CMD_AGENTOP_NUMUSED];
extern int cmdPsychicopTech[CMD_PSYCHICOP_NUMUSED];
extern int cmdGhostopTech[CMD_GHOSTOP_NUMUSED];

extern float cmdAgentopReadiness[CMD_AGENTOP_NUMUSED];
extern float cmdPsychicopReadiness[CMD_PSYCHICOP_NUMUSED];
extern int cmdGhostopReadiness[CMD_GHOSTOP_NUMUSED];

extern float cmdAgentopDifficulty[CMD_AGENTOP_NUMUSED];
extern float cmdPsychicopDifficulty[CMD_PSYCHICOP_NUMUSED];
extern float cmdGhostopDifficulty[CMD_GHOSTOP_NUMUSED];

extern int cmdAgentopStealth[CMD_AGENTOP_NUMUSED];
extern int cmdPsychicopFlags[CMD_PSYCHICOP_NUMUSED];
extern int cmdGhostopFlags[CMD_GHOSTOP_NUMUSED];




typedef struct
{
  float growth;
  float attack;
  float defense;
  float speed;
  float researchpoints[CMD_RESEARCH_NUMUSED];
  float researchmax[CMD_RESEARCH_NUMUSED];
  float unit[CMD_UNIT_NUMUSED];
  float resource[CMD_RESSOURCE_NUMUSED];
  int special;

  int operations;
  int spells;
  int incantations;

} cmdRaceDef, *cmdRacePtr;


cmdRaceDef cmdRace[CMD_RACE_NUMUSED];

extern char *cmdRaceName[CMD_RACE_NUMUSED];



#endif
