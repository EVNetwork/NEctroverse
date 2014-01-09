#ifndef GLOBALINCLUDED
#include "config/global.h"
#endif


char *cmdReadyName[CMD_READY_NUMUSED] =
{
"Fleet",
"Psychics",
"Agents"
};

char *cmdRessourceName[CMD_RESSOURCE_NUMUSED+2] =
{
"Energy",
"Mineral",
"Crystal",
"Ectrolium",
"Time",
"Population"
};

//These aren't used in-game, they are just to simplify the AJAX.
char *cmdMainInfoNames[INFOS_TOTAL_NUMUSED] =
{
"energyincome",
"mineralincome",
"crystalincome",
"ectroliumincome",

"energyproduction",
"energydecay",

"buildingupkeep",
"populationreduction",
"portalsupkeep",
"unitupkeep",

"mineralproduction",

"crystalproduction",
"crystaldecay",

"ectroliumproduction",

"energytax",
"mineraltax",
"crystaltax",
"ectroliumtax",
};

char *cmdBonusName[CMD_BONUS_NUMUSED] =
{
"Solar",
"Mineral",
"Crystal",
"Ectrolium",
"Fission",/*,
"Research",
"Population",
"Disease",*/
};

char *cmdBuildingName[CMD_BLDG_NUMUSED+1] =
{
"Solar collectors",
"Fission reactors",
"Mineral plants",
"Crystal laboratories",
"Refinement stations",
"Cities",
"Research centers",
"Defense satellites",
"Shield network",
"Portal",
};

char *cmdUnitName[CMD_UNIT_NUMUSED] =
{
"Bombers",
"Fighters",
"Transports",
"Cruisers",
"Carriers",
"Soldiers",
"Droids",
"Goliaths",
"Phantoms",

"Psychics",
"Agents",
"Ghost ships",
"Exploration ships"
};

char *cmdResearchName[CMD_RESEARCH_NUMUSED] =
{
"Military",
"Construction",
"Technology",
"Energy production",
"Portals efficiency",
"Culture",
"Operations"
};

char *cmdResearchDesc[CMD_RESEARCH_NUMUSED] =
{
"Increases your units attack and defense strength",
"Reduces construction costs and building time",
"Allow construction of advanced units and buildings",
"Increases energy output",
"Increases the protection range of your portals",
"Increases building upkeep reduction and psychics capabilities",
"Increases the effectiveness of your agents"
};


int64_t cmdBuildingCost[CMD_BLDG_NUMUSED+1][CMD_RESSOURCE_NUMUSED+1] =
{
// energy, mineral, crystal, endurium, time
{ 120, 10,  0,  1,  4 },
{ 450, 20, 12,  8, 14 },
{ 200,  0,  0,  2,  8 },
{ 350,  8,  0, 12,  6 },
{ 400, 36,  4,  0, 12 },
{ 300, 30,  0,  2, 10 },
{ 100,  5,  5,  5,  8 },
{ 400, 35, 20, 40, 16 },
{2000, 10, 60, 30, 24 },

// portals
{ 8000, 200, 500, 400, 128 }
};

float cmdBuildingUpkeep[CMD_BLDG_NUMUSED] =
{
0.0, 20.0, 2.0, 2.0, 2.0, 4.0, 1.0, 4.0, 16.0
};

int cmdBuildingFlags[CMD_BLDG_NUMUSED] =
{
0, 0, 0, 0, 0, 0, 0, 1, 1
};


int64_t cmdUnitCost[CMD_UNIT_NUMUSED][CMD_RESSOURCE_NUMUSED+1] =
{
// energy, mineral, crystal, endurium, time
{ 250, 15,  0,  5,  6 },
{ 150, 10,  0,  3,  5 },
{ 600, 35, 10, 10,  8 },
{1600, 90,  0, 45, 12 },
{2000,160, 15, 20, 12 },
{ 100,  0,  0,  1,  3 },
{  50,  5,  0,  1,  2 },
{ 350, 20,  8, 10,  4 },
{  -1, -1, -1, -1, -1 },

{ 150,  0, 10,  0,  5 },
{ 150,  0,  0, 10,  5 },
{ 200,  8, 60,  5,  7 },
{5000, 50,  0, 50,  4 }
};

int cmdUnitTech[CMD_UNIT_NUMUSED] =
{
60, 0, 0, 40, 20, 0, 80, 120, 0,   0, 0, 160, 0
};

int cmdBuildingTech[CMD_BLDG_NUMUSED+1] =
{
0, 100, 0, 0, 0, 0, 0, 110, 140, 0
};

float cmdUnitUpkeep[CMD_UNIT_NUMUSED] =
{
2.0, 1.6, 3.2, 12.0, 18.0, 0.4, 0.6, 2.8, 0.0,   0.8, 0.8, 2.4, 60.0
};

// to air attack, from air defence, to ground attack, from ground attack, speed, networth
int cmdUnitStats[CMD_UNIT_NUMUSED][CMD_UNIT_STATS_NUMUSED] =
{
{  0, 64, 24,110, 4,  4 },
{ 20,120,  0, 60, 4,  3 },
{  0, 60,  0, 50, 4,  5 },
{ 70,600, 70,600, 4, 12 },
{  0,540,  0,540, 4, 14 },
{  0, 48,  3, 16, 0,  1 },
{  0, 48,  5, 30, 0,  1 },
{ 28,140, 10, 90, 0,  4 },
{ 32,130, 20,130, 0,  5 },

{  0,  0,  0,  0, 8,  2 },
{  0,  0,  0,  0, 0,  2 },
{  0,  0,  0,  0, 8,  6 },
{  0,  0,  0,  0, 3, 30 }
};



char *cmdFleetOrderName[CMD_FLEET_ORDER_NUMUSED] =
{
"Explore the planet",
"Station on planet",
"Move to system",
"Attack the planet",
"Join main fleet",
"Awaiting orders",
"Merging",

"Spy Target",
"Observe Planet",
"Plant Network Virus",
"Infiltrate",
"Infect Population",
"Transfer Energy",
"Military Sabotage",
"Nuke Planet",
"High Infiltrate",
"Planetary Beacon",

"Sense Artefact",
"Survey System",
"Planetary Shielding",
"Portal Force Field",
"Vortex Portal",
"Mind Control",
"Energy Surge"

};




char *cmdAgentopName[CMD_AGENTOP_NUMUSED] =
{
"Spy Target",
"Observe Planet",
"Network Virus",
"Infiltration",
"Bio Infection",
"Energy Transfer",
"Military Sabotage",
"Nuke Planet",
"High Infiltration",
"Planetary Beacon"
};

char *cmdPsychicopName[CMD_PSYCHICOP_NUMUSED] =
{
"Irradiate Ectrolium",
"Dark Web",
"Incandescence",
"Black Mist",
"War Illusions",
"Psychic Assault",
"Phantoms"
};

char *cmdGhostopName[CMD_GHOSTOP_NUMUSED] =
{
"Sense Artefact",
"Survey System",
"Planetary Shielding",
"Portal Force Field",
"Vortex Portal",
"Mind Control",
"Energy Surge"
};


int cmdAgentopTech[CMD_AGENTOP_NUMUSED] =
{
0, 0, 25, 40, 60, 80, 100, 120, 160, 100
};

int cmdPsychicopTech[CMD_PSYCHICOP_NUMUSED] =
{
0, 10, 0, 50, 70, 90, 110
};

int cmdGhostopTech[CMD_GHOSTOP_NUMUSED] =
{
20, 40, 60, 80, 100, 120, 140
};


float cmdAgentopReadiness[CMD_AGENTOP_NUMUSED] =
{
12.0, 5.0, 22.0, 18.0, 25.0, 22.0, 30.0, 20.0, 40.0, 15
};

float cmdPsychicopReadiness[CMD_PSYCHICOP_NUMUSED] =
{
12.0, 22.0, 30.0, 24.0, 30.0, 35.0, 40.0
};

int cmdGhostopReadiness[CMD_GHOSTOP_NUMUSED] =
{
50, 20, 40, 30, 60, 40, 70
};


float cmdAgentopDifficulty[CMD_AGENTOP_NUMUSED] =
{
1.0, 1.0, 3.9, 2.5, 4.0, 3.5, 4.5, 5.0, 6.0, 1.5
};

float cmdPsychicopDifficulty[CMD_PSYCHICOP_NUMUSED] =
{
1.5, 2.4, 2.5, 3.0, 4.0, 1.7, 5.0
};

float cmdGhostopDifficulty[CMD_GHOSTOP_NUMUSED] =
{
1.0, 1.0, 2.0, 1.5, 1.0, 5.0, 6.2
};


int cmdAgentopStealth[CMD_AGENTOP_NUMUSED] =
{
1, 1, 0, 1, 0, 0, 0, 0, 0, 0
};

// 1:stealth  2:self-spell
int cmdPsychicopFlags[CMD_PSYCHICOP_NUMUSED] =
{
0, 2, 2, 0, 2, 0, 2
};

// 1:stealth  2:can be self-spell  4:target system  8:not offensive
int cmdGhostopFlags[CMD_GHOSTOP_NUMUSED] =
{
4, 1|4, 1|2|8, 0, 4, 1, 2
};



cmdRaceDef cmdRace[CMD_RACE_NUMUSED] =
{
 {
 	//Hark
  1.0+(0.8*0.02), 1.4, 0.9, 1.4*2.0,
  {   1.2,   1.2,   1.2,   1.2,   1.2,   0.6,   1.2 },
  { 250.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0 },
  { 1.0, 1.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
  { 0.90, 1.00, 1.25, 1.00 },
  0,
  0 | 0 | 4 | 8 | 16 |  0 | 64 | 128 |   0,
  1 | 0 | 4 | 8 | 16 |  0 |  0,
  0 | 0 | 0 | 8 | 16 |  0 | 64,
 },
 {//manti
  1.0+(0.9*0.02), 0.7, 1.1, 1.0*2.0,
  {   0.9,   0.9,   0.9,   0.9,   0.9,   1.8,   0.9 },
  { 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0 },
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,   1.4, 1.0, 1.2, 1.0 },
  { 1.40, 1.00, 1.00, 1.00 },
  CMD_RACE_SPECIAL_SOLARP15,
  1 | 2 | 0 | 0 |  0 | 32 |  0 |  0 |   0,
  0 | 2 | 0 | 8 | 16 | 32 | 64,
  0 | 0 | 4 | 0 |  0 | 32 |  0,
 },
 {//foohon
  1.0+(1.1*0.02), 1.2, 1.1, 1.0*2.0,
  {   1.5,   1.5,   1.5,   1.5,   1.5,   1.5,   1.5 },
  { 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0 },
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,   1.0, 1.0, 1.1, 1.0 },
  { 0.80, 1.00, 1.00, 1.20 },
  CMD_RACE_SPECIAL_POPRESEARCH,
  1 | 2 | 0 | 8 |  0 |  0 | 64 |  0 | 256 | 512,
  1 | 2 | 4 | 0 |  0 | 32 |  0,
  1 | 2 | 0 | 0 | 16 | 32 |  0,
 },
 {//spaceborne
  1.0+(1.2*0.02), 1.0, 1.2, 1.8*2.0,
  {   1.1,   1.1,   0.6,   1.1,   1.1,   1.1,   1.1 },
  { 200.0, 200.0, 200.0, 250.0, 200.0, 200.0, 200.0 },
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.1, 1.0, 1.0,   0.7, 1.2, 1.0, 1.0 },
  { 1.30, 1.00, 1.00, 1.00 },
  0,
  1 | 2 | 4 | 0 | 16 | 32 |  0 | 128 | 512,
  1 | 0 | 4 | 8 |  0 |  0 |  0,
  0 | 2 | 4 | 0 |  0 |  0 |  0,
 },
 {//dreamweaver
  1.0+(1.1*0.02), 1.0, 0.7, 1.0*2.0,
  {   0.7,   1.4,   2.8,   1.4,   1.4,   1.4,   1.4 },
  { 200.0, 250.0, 200.0, 200.0, 200.0, 200.0, 200.0 },
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,   1.5, 1.0, 1.3, 1.0 },
  { 0.80, 1.00, 1.25, 1.00 },
  0,
  0 | 0 | 4 | 0 | 16 | 32 | 64 |   0 |   0,
  1 | 2 | 4 | 8 | 16 | 32 | 64,
  1 | 0 | 0 | 8 |  0 | 32 | 64,
 },/*
  {//Furtifon
  1.0+(0.9*0.02), 0.90, 1.0, 1.6*2.0,
  {   0.90,  0.90,   0.90,   0.90,   0.90,   0.90,   1.8 },
  { 200.0, 250.0, 200.0, 200.0, 200.0, 200.0, 200.0 },
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,   1.0, 1.2, 1.2, 1.0 },
  { 1.0, 1.0, 1.0, 1.0 },
  CMD_RACE_SPECIAL_CULPROTECT,
  1 | 2 | 8 | 32 | 256 | 512,
  4 | 16,
  1 | 2 | 4 | 8 | 16,
 },
  {//Samsonites
  1.0+(1.0*0.02), 1.3, 1.2, 0.80*2.0,
  {   1.10,  1.10,   1.10,   1.10,   1.10,   1.10,   1.10 },
  { 185.0, 185.0, 185.0, 185.0, 185.0, 185.0, 185.0 },
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,   0.9, 0.9, 1.0, 1.0 },
  { 1.1, 1.0, 1.0, 1.0 },
  CMD_RACE_SPECIAL_IDW,
  16 | 64 | 128,
  8 | 64,
  2 | 32 | 64,
 },
 {//Ultimums
  1.0+(1*0.02), 1.0, 1.0, 1.0*2.0,
  {   1.0,  1.0,   1.0,   1.0,   1.0,   1.0,   1.0 },
  { 250.0, 250.0, 250.0, 250.0, 250.0, 250.0, 250.0 },
  { 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1 },
  { 1.0, 1.0, 1.0, 1.0 },
  CMD_RACE_SPECIAL_POPRESEARCH|CMD_RACE_SPECIAL_SOLARP15,
  1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512,
  1 | 2 | 4 | 8 | 16 | 32 | 64,
  1 | 2 | 4 | 8 | 16 | 32 | 64,
 },*/
};

char *cmdRaceName[CMD_RACE_NUMUSED] =
{
"Harks",
"Manticarias",
"Foohons",
"Spacebornes",
"Dreamweavers"/*,
"Furtifons",
"Samsonites",
"Ultimums"*/
};

int cmdUserFlags[CMD_USER_FLAGS_NUMUSED] =
{
0x1,
0x2,
0x4,
0x8,

0x10,
0x20,
0x40,
0x80,
0x100,

0x200,
0x400,

0x1000,


#if FACEBOOK_SUPPORT
0x10000,
0x20000,
#endif
};


char *cmdErrorString;
char cmdErrorBuffer[1024];


dbUserMainDef cmdUserMainDefault =
{
  { }, //Faction
  { 120000, 6000, 1500, 3000, 0, 0/*, 0, 0*/ }, //ressource
  -1, //Empire
  { }, //infos
  { 0, 0, 0, 0, 0, 0, 0/*, 0*/ }, //Research
  { 100*65536, 100*65536, 100*65536 }, //readiness
  0, //home
  { }, //totalbuilding
  { }, //totalunit
  { }, //totalresearch
  1250, //networth
  { }, //allocresearch
  0, //fundresearch
  0, //planets
  1, //config_fleet
  { 200, 200, 250, 250 }, //config_flee
  15|0x10000, //config_mapsize
  { 0x1|(0x1<<4), 0x2|(0x0<<4), -1, -1, -1, -1, -1, -1 }, //config_map
  -1, //RaceID
  0, //Artefacts
  -1, //rank
  -1, //Aidaccess
};

dbUserFleetDef cmdUserFleetDefault =
{
  { 0, 40, 0, 0, 0, 80, 0, 0, 0, 50, 50, 0, 4 },
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void cmdEmpireNewsAdd( int famid, int id, int64_t *data )
{
  if( data[2] == CMD_NEWS_BUILDING )
    return;
  else if( data[2] == CMD_NEWS_UNIT )
    return;
  else if( data[2] == CMD_NEWS_STATION )
    return;
  else if( data[2] == CMD_NEWS_STATION_FAILED )
    return;
  else if( data[2] == CMD_NEWS_RECALL )
    return;
  else if( data[2] == CMD_NEWS_MARKET_BOUGHT )
    return;
  else if( data[2] == CMD_NEWS_MARKET_SOLD )
    return;
  else if( data[2] == CMD_NEWS_MAIL )
    return;
  else if( data[2] == CMD_NEWS_OPCANCEL )
    return;
  else if( data[2] == CMD_NEWS_SPCANCEL )
    return;
  else if( data[2] == CMD_NEWS_INCANCEL )
    return;
  else if( data[2] == CMD_NEWS_PLANET_OFFER )
    return;
  else if( data[2] == CMD_NEWS_PLANET_GIVEN )
    return;
  else if( data[2] == CMD_NEWS_FLEETS_MERGE)
  	return;
  data[1] = id;
  dbFamNewsAdd( famid, data );
  return;
}

int cmdUserNewsAdd( int id, int64_t *data, int64_t flags )
{
  int a;
  dbUserMainDef maind;
  a = dbUserNewsAdd( id, data, flags );
  if( ( dbUserMainRetrieve( id, &maind ) >= 0 ) && ( maind.empire != -1 ) )
    cmdEmpireNewsAdd( maind.empire, id, data );
  return a;
}



int cmdCheckName( char *name )
{
  int a, b, c;
  c = 1;
  if( ( name[0] >= '0' ) && ( name[0] <= '9' ) )
    return 0;
  for( a = b = 0 ; name[a] ; a++ )
  {
    if( a == NAME_MAX )
      return 0;
    else if( ( name[a] == 13 ) || ( name[a] == 10 ) )
      name[a] = 0;
    else if(  ( ( name[a] >= 'a' ) && ( name[a] <= 'z' ) )  ||  ( ( name[a] >= 'A' ) && ( name[a] <= 'Z' ) )  ||  ( ( name[a] >= '0' ) && ( name[a] <= '9' ) )  ||  ( name[a] == '_' )  ||  ( name[a] == '-' )  )
      c = 0;
    else if( ( name[a] == '+' ) || ( name[a] == ' ' ) )
    {
      name[a] = ' ';
      if( c )
        return 0;
      c = 1;
      b++;
    }
    else
      return 0;
  }
  if( c )
    return 0;
  if( a < 2 )
    return 0;
  if( ( b ) && ( b >= (a>>1) ) )
    return 0;
  return 1;
}

void cmdEmpireLeader( dbMainEmpirePtr empired )
{
  int a, b;
  int *parray;
  dbUserPtr user;

  if( ( parray = calloc( dbMapBInfoStatic[MAP_CAPACITY], sizeof(int) ) ) == NULL )
  	error( "Malloc Failed" );
  for( a = b = 0 ; a < empired->numplayers ; a++ )
  {
    if( empired->vote[a] == -1 )
      continue;
    parray[(int)(empired->vote[a])]++;
    if( empired->leader == empired->player[a] )
      b = a;
  }

/*
  if( ( user = dbUserLinkID( empired->player[b] ) ) && ( user->flags & cmdUserFlags[CMD_USER_FLAGS_LEADER] ) )
  {
    user->flags &= 0xFFFF;
    dbUserSave( empired->player[b], user );
  }
*/

  for( a = 0 ; a < empired->numplayers ; a++ )
  {
    if( parray[a] <= parray[b] )
      continue;
    b = a;
  }
  if( parray[b] >= 1 )
  {
    empired->leader = empired->player[b];
    if( ( user = dbUserLinkID( empired->leader ) ) )
    {
      user->flags &= 0xFFFF;
      user->flags |= cmdUserFlags[CMD_USER_FLAGS_LEADER];
      dbUserSave( empired->leader, user );
    }
  }
  else
  {
    empired->leader = -1;
    b = -1;
  }


  for( a = 0 ; a < empired->numplayers ; a++ )
  {
    if( a == b )
      continue;
    if( ( user = dbUserLinkID( empired->player[a] ) ) && ( user->flags & cmdUserFlags[CMD_USER_FLAGS_LEADER] ) )
    {
      user->flags &= 0xFFFF;
      dbUserSave( empired->player[a], user );
    }
  }

  free( parray );
  return;
}




int cmdTotalsCalculate( int usrid, dbUserMainPtr mainp )
{
  int a, b, num, population;
  int *buffer;
  dbMainPlanetDef planetd;
  dbMainEmpireDef empired;
  dbUserFleetPtr fleetd;

  memset( mainp->totalbuilding, 0, CMD_BLDG_NUMUSED*sizeof(int64_t) );
  memset( mainp->totalunit, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );
  if( ( num = dbUserPlanetListIndices( usrid, &buffer ) ) < 0 )
    return 0;

  population = 0;
  for( a = 0 ; a < num ; a++ )
  {
    dbMapRetrievePlanet( buffer[a], &planetd );

    population += planetd.population;
    for( b = 0 ; b < CMD_BLDG_NUMUSED ; b++ )
      mainp->totalbuilding[b] += planetd.building[b];
    for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ )
      mainp->totalunit[b] += planetd.unit[b];
    if( ( planetd.flags & CMD_PLANET_FLAGS_PORTAL ) )
      mainp->totalbuilding[CMD_BLDG_NUMUSED]++;

    if( ( b = artefactPrecense( &planetd ) ) < 0 )
      continue;
    if( dbMapRetrieveEmpire( mainp->empire, &empired ) < 0 )
      continue;
    empired.artefacts |= 1 << b;
    dbMapSetEmpire( mainp->empire, &empired );

  }
  mainp->planets = num;
  mainp->ressource[CMD_RESSOURCE_POPULATION] = (int64_t)population;
  free( buffer );
  if( ( num = dbUserFleetList( usrid, &fleetd ) ) < 0 )
    return 0;
  for( a = 0 ; a < num ; a++ )
  {
    for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ )
      mainp->totalunit[b] += fleetd[a].unit[b];
  }

  free( fleetd );
  return 1;
}



void cmdExecuteFlush()
{
  dbFlush();
  return;
}




/* |0x10000 for unit */
void cmdGetBuildCosts( dbUserMainPtr maind, int type, int64_t *buffer )
{
  int a;
  int b = 0;
  double da, cost;
  cost = 100.0 / ( 100.0 + maind->totalresearch[CMD_RESERACH_CONSTRUCTION] );
  if( !( type >> 16 ) )
  {
    a = cmdBuildingTech[type] - maind->totalresearch[CMD_RESEARCH_TECH];
    buffer[CMD_RESSOURCE_NUMUSED+1] = 0;

		//Change build time depending on arti
    if( a > 0 )
    {
      da = pow( (double)a, 1.1 );
      if( da >= 100.0 )
      {
        buffer[0] = -1;
        return;
      }
      buffer[CMD_RESSOURCE_NUMUSED+1] = (int64_t)da;
      cost *= 1.0 + 0.01*da;
    }
    for( a = 0 ; a < CMD_RESSOURCE_NUMUSED+1 ; a++ )
    {
      buffer[a] = ceil( cost * cmdBuildingCost[type][a] );
    }
  }
  else
  {
    type &= 0xFFFF;
    b++;
    if( cmdUnitCost[type][0] < 0 )
    {
      buffer[0] = -2;
      return;
    }
    a = cmdUnitTech[type] - maind->totalresearch[CMD_RESEARCH_TECH];
    buffer[CMD_RESSOURCE_NUMUSED+1] = 0;
    if( a > 0 )
    {
      da = pow( (double)a, 1.1 );
      if( da >= 100.0 )
      {
        buffer[0] = -1;
        return;
      }
      buffer[CMD_RESSOURCE_NUMUSED+1] = (int64_t)da;
      cost *= 1.0 + 0.01*da;
    }
    for( a = 0 ; a < CMD_RESSOURCE_NUMUSED+1 ; a++ )
    {
      buffer[a] = ceil( cost * cmdUnitCost[type][a] );
    }

  }
  return;
}

/*
 * We must check for special case (total <= size) and not just (total < size)
 * because floating point errors may cause cost to be smaller than 1,
 * when it should be equal to 1.
 * In particular, when size is cast bigger than total.
 */
float cmdGetBuildOvercost( int size, int total )
{
  float cost;
  if( total <= size )
    return 1.0;
  cost = (float)total / (float)size;
  return ( cost * cost );
}

void cmdGetBuildOverbuild( int size, int total, int64_t *buffer )
{
  int a;
  float cost;
  if( total < size )
    return;
  cost = cmdGetBuildOvercost( size, total );
  for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
    buffer[a] = ceil( cost * buffer[a] );
  return;
}



/* |0x10000 for unit */
int cmdGetOpPenalty( int research, int requirement )
{
  int a;
  double da;
  a = requirement - research;
  if( a <= 0 )
    return 0;
  da = pow( (double)a, 1.20 );
  if( da >= 150.0 )
    return -1;
  return (int)da;
}




#define IOHTTP_TAGNUM (23)

char *cmdTagNames[IOHTTP_TAGNUM] =
{
"Player",
"Veteran",
"Chicken-soup-machine Repairman",
"3rd Technician",
"2nd Technician",
"1st Technician",
"Helmsman",
"Master-at-Arms",
"3rd Officer",
"2nd Officer",
"1st Officer",
"Patrol Officer",
"Squad Lieutenant",
"Lieutenant Commander",
"Wing Commander",
"Cruiser Captain",
"Squadron Commander",
"Fleet Admiral",
"Elite Strategist",
"Dear Leader",
"Master Wizard",
"Transcend",
"(personalized tag)",
};

int cmdTagPoints[IOHTTP_TAGNUM] =
{
45,
80,
160,
240,
380,
460,
600,
850,
1150,
1320,
1700,
2250,
2600,
3100,
3500,
3900,
4600,
5800,
7000,
9000,
12500,
0xFFFFFFF
};


char *cmdTagFind( int points )
{
  int a;
  for( a = 0 ; a < IOHTTP_TAGNUM-1 ; a++ )
  {
    if( points < cmdTagPoints[a] )
      break;
  }
  return cmdTagNames[a];
}




int64_t cmdFleetActionNewd[DB_USER_NEWS_BASE];

void specopFleetPerform( dbUserFleetPtr fleetd, int *newd );

int cmdFleetAction( dbUserFleetPtr fleetd, int id, int fltid, int postnews )
{
  int a, andl, i, nFltid;
  int64_t newd[DB_USER_NEWS_BASE];
  dbMainPlanetDef planetd;
  dbMainSystemDef systemd;
  dbUserFleetDef fleet2d;
  dbUserFleetPtr fleetp;

  newd[1] = 0;
  andl = 0;
  if( postnews )
  {
    newd[1] = CMD_NEWS_FLAGS_NEW;
    andl = 0xFFFFFFFF;
  }
  newd[0] = ticks.number;
  memcpy( cmdFleetActionNewd, newd, 2*sizeof(int64_t) );

  if( fleetd->order == CMD_FLEET_ORDER_EXPLORE )
  {
    dbMapRetrievePlanet( fleetd->destid, &planetd );
    if( ( planetd.owner != -1 ) || ( planetd.flags & CMD_PLANET_FLAGS_HOME ) )
    {
      fleetd->order = CMD_FLEET_ORDER_CANCELED;
      fleetd->time = -1;
      dbUserFleetSet( id, fltid, fleetd );
      newd[2] = CMD_NEWS_EXPLORE_FAILED;
      newd[3] = fleetd->destid;
      newd[4] = planetd.position;
      cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_FLEET & andl );
      return 1;
    }
    if( dbUserPlanetAdd( id, fleetd->destid, fleetd->sysid, fleetd->destination, 0 ) < 0 )
      return 1;
    dbMapRetrieveSystem( planetd.system, &systemd );
    systemd.unexplored--;
    dbMapSetSystem( planetd.system, &systemd );

    planetd.maxpopulation = (float)( planetd.size * CMD_POPULATION_SIZE_FACTOR );
    planetd.flags = 0;
    planetd.owner = id;
    planetd.population = planetd.size * CMD_POPULATION_BASE_FACTOR;
    planetd.protection = 0.0;
    memset( planetd.building, 0, CMD_BLDG_NUMUSED*sizeof(int) );
    memset( planetd.unit, 0, CMD_UNIT_NUMUSED*sizeof(int) );
    planetd.construction = 0;
    dbMapSetPlanet( fleetd->destid, &planetd );
    dbUserFleetRemove( id, fltid );
    newd[2] = CMD_NEWS_EXPLORE;
    newd[3] = fleetd->destid;
    newd[4] = planetd.position;
    newd[5] = artefactPrecense( &planetd );

    cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_FLEET & andl );
  }
  else if( fleetd->order == CMD_FLEET_ORDER_STATION )
  {
    dbMapRetrievePlanet( fleetd->destid, &planetd );
    if( planetd.owner != id )
    {
      fleetd->order = CMD_FLEET_ORDER_CANCELED;
      fleetd->time = -1;
      dbUserFleetSet( id, fltid, fleetd );
      newd[2] = CMD_NEWS_STATION_FAILED;
      newd[3] = fleetd->destid;
      newd[4] = planetd.position;
      cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_FLEET & andl );
      return 1;
    }
    for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
      planetd.unit[a] += fleetd->unit[a];
    dbMapSetPlanet( fleetd->destid, &planetd );
    dbUserFleetRemove( id, fltid );
    newd[2] = CMD_NEWS_STATION;
    newd[3] = fleetd->destid;
    newd[4] = planetd.position;
    cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_FLEET & andl );
  }
  else if(fleetd->order == CMD_FLEET_ORDER_MERGE)
  {
  	a = dbUserFleetList(id, &fleetp);
  	//check if there is a merge fleet there
  	nFltid = -1;
  	for(i=1;i<a;i++)
  	{
  		if((fleetp[i].order == CMD_FLEET_ORDER_MERGE)&&(i != fltid)&&(fleetp[i].destid == fleetd->destid)&&(fleetp[i].flags & CMD_FLEET_FLAGS_RMERGE))
  		{
  			nFltid = i;
  			break;
  		}
  	}
  	if(nFltid != -1)
  	{
  		for(i=0;i<CMD_UNIT_NUMUSED;i++)
  		{
  			fleetd->unit[i] += fleetp[nFltid].unit[i];
  		}
  		fleetd->order = CMD_FLEET_ORDER_MOVE;
  		fleetd->time = -1;
  		fleetd->flags = 0;
  		dbUserFleetSet( id, fltid, fleetd );
  		dbUserFleetRemove(id, nFltid);
  		//put some news on the hq when they merge
  		newd[2] = CMD_NEWS_FLEETS_MERGE;
      newd[3] = fleetd->destination;
      newd[4] = nFltid;
      newd[5] = fltid;
      newd[6] = fleetd->basetime;
      cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_FLEET & andl );
  	}
  	else
  	{
  		fleetd->flags |= CMD_FLEET_FLAGS_RMERGE;
  		fleetd->time = -1;
  		dbUserFleetSet( id, fltid, fleetd );
  	}
  	free(fleetp);
  }
  else if( fleetd->order == CMD_FLEET_ORDER_MOVE )
  {
    fleetd->time = -1;
    dbUserFleetSet( id, fltid, fleetd );
  }
  else if( fleetd->order == CMD_FLEET_ORDER_ATTACK )
  {
    fleetd->time = -1;
    dbUserFleetSet( id, fltid, fleetd );
  }
  else if( fleetd->order == CMD_FLEET_ORDER_RECALL )
  {
    if( !( dbUserFleetRetrieve( id, 0, &fleet2d ) ) )
      return 1;
    for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
      fleet2d.unit[a] += fleetd->unit[a];
    dbUserFleetSet( id, 0, &fleet2d );
    if( !( dbUserFleetRetrieve( id, 0, &fleet2d ) ) )
      return 1;
    dbUserFleetRemove( id, fltid );
    newd[2] = CMD_NEWS_RECALL;
    newd[3] = fleetd->basetime;
    cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_FLEET & andl );
  }

  else if( ( fleetd->order >= CMD_FLEET_ORDER_FIRSTOP ) && ( fleetd->order < CMD_FLEET_ORDER_LASTOP ) )
  {
    specopAgentsPerformOp( id, fltid, fleetd, cmdFleetActionNewd );
    cmdExecChangeFleet( id, 0, 0, 0, CMD_FLEET_ORDER_RECALL, fltid );
  }

  else if( ( fleetd->order >= CMD_FLEET_ORDER_FIRSTINCANT ) && ( fleetd->order < CMD_FLEET_ORDER_LASTINCANT ) )
  {
    specopGhostsPerformOp( id, fltid, fleetd, cmdFleetActionNewd );
    cmdExecChangeFleet( id, 0, 0, 0, CMD_FLEET_ORDER_RECALL, fltid );
  }

  return 1;
}






int cmdFindDistPortal( int id, int tx, int ty, int *distptr, int *portalpos )
{
  int a, num, x, y, plnid, ppos;
  float dist, mindist;
  int *buffer;
  if( ( num = dbUserPortalsList( id, &buffer ) ) < 0 )
    return -3;

  num = specopVortexListCalc( id, num, &buffer );

  if( !( num ) )
  {
    free( buffer );
    return -2;
  }
  plnid = -2;
  ppos = -1;
  mindist = (float)0xFFFFFFF;
  num *= 3;
  for( a = 0 ; a < num ; a += 3 )
  {
    x = ( ( buffer[a+2] >> 8 ) & 0xFFF ) - tx;
    y = ( buffer[a+2] >> 20 ) - ty;

    dist = sqrt( x*x + y*y );
    if( dist < mindist )
    {
      mindist = dist;
      plnid = buffer[a+0];
      ppos = buffer[a+2];
    }
  }
  free( buffer );
  if( distptr )
    *distptr = (int)ceil( mindist * 256.0 );
  if( portalpos )
    *portalpos = ppos;
  return plnid;
}

int cmdFindDistCurrent( int x, int y, int tx, int ty, int *distptr )
{
  float dist;
  x -= tx;
  y -= ty;
  dist = sqrt( x*x + y*y );
  if( distptr )
    *distptr = (int)ceil( dist * 256 );
  return 1;
}


void cmdFleetGetPosition( dbUserFleetPtr fleetd, int *x, int *y )
{
  int destx, desty, srcx, srcy;
  float dx, dy, dpos;
  destx = ( fleetd->destination >> 8 ) & 0xFFF;
  desty = fleetd->destination >> 20;
  if( ( fleetd->time == -1 ) || ( fleetd->basetime == 0 ) )
  {
    *x = destx;
    *y = desty;
    return;
  }
  srcx = ( fleetd->source >> 8 ) & 0xFFF;
  srcy = fleetd->source >> 20;
  dx = destx - srcx;
  dy = desty - srcy;
  dpos = (float)fleetd->time / (float)fleetd->basetime;
  *x = destx - (int)( dx*dpos );
  *y = desty - (int)( dy*dpos );
  return;
}




#include "battle.c"

#include "specop.c"

#include "cmdexec.c"

#include "artefact.c"

#include "cmdtick.c"




// Note : this is getting unused and will be removed, an interface independant layer is being created differently.
// Replacement functions are available in cmdexec.c

int cmdExecute( void *DEPRECIATED, int *cmd, void *buffer, int size )
{
  int a, b, c, d;
  int64_t cost[4];
  float fa, fb, fc;
  int *ibuffer;
  int64_t resbuild[CMD_RESSOURCE_NUMUSED+2];
  float costbuild[CMD_RESSOURCE_NUMUSED];
  dbUserMainDef maind;
  dbUserFleetDef fleetd, fleet2d;
  dbMainPlanetDef planetd;
  ReplyDataPtr cnt = DEPRECIATED;

  cmdErrorString = 0;

  if( cmd[0] == CMD_SEARCH_EMPIRE )
  {
    a = dbUserSearchFaction( buffer );
    if( a >= 0 )
      return a;
    else
    {
      cmdErrorString = "This faction does not exist";
      return -3;
    }
  }
  else if( cmd[0] == CMD_CHANGE_KILLUSER )
  {
    if( cmd[1] < 0 )
      return -2;
    if( !( (cnt->session)->dbuser ) )
      return -3;
    if( ( (cnt->session)->dbuser->id != cmd[1] ) && ( (cnt->session)->dbuser->level < LEVEL_ADMINISTRATOR ) )
      return -3;
    return cmdUserDelete( cmd[1] );
  }
  else if( cmd[0] == CMD_SET_USERMAIN )
  {
    return dbUserMainSet( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERMAIN )
  {
    return dbUserMainRetrieve( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERPLIST )
  {
    return dbUserPlanetListCoords( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERPLISTINDICES )
  {
    return dbUserPlanetListIndices( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERPLISTSYSTEMS )
  {
    return dbUserPlanetListSystems( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERPORTALS )
  {
    return dbUserPortalsListCoords( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERPORTALSINDICES )
  {
    return dbUserPortalsListIndices( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_RETRIEVE_USERBUILDLIST )
  {
    return dbUserBuildList( cmd[1], buffer );
  }
  else if( cmd[0] == CMD_ADD_BUILD )
  {
    if( (unsigned int)cmd[3] > 10000000 )
    {
      a = sprintf( cmdErrorBuffer, "I think you should try asking for a more reasonable amount of " );
      if( !( cmd[2] >> 16 ) )
        sprintf( &cmdErrorBuffer[a], "%s", cmdBuildingName[ cmd[2] & 0xFFF ] );
      else
        sprintf( &cmdErrorBuffer[a], "%s", cmdUnitName[ cmd[2] & 0xFFF ] );
      cmdErrorString = cmdErrorBuffer;
      return -1;
    }
    // int id, int type, int quantity, int plnid, oblimit, oblimitlow
    if( dbUserMainRetrieve( cmd[1], &maind ) < 0 )
      return -3;
    cmdGetBuildCosts( &maind, cmd[2], resbuild );
    if( resbuild[0] < 0 )
    {
      cmdErrorString = "Your technology research is too low";
      return -3;
    }
    if( cmd[2] == CMD_BLDG_NUMUSED )
    {
      // portal
      if( !( dbMapRetrievePlanet( cmd[4], &planetd ) ) )
        return -3;
      if( planetd.owner != cmd[1] )
      {
        cmdErrorString = "You do not own this planet";
        return -3;
      }
      if( planetd.flags & ( CMD_PLANET_FLAGS_PORTAL | CMD_PLANET_FLAGS_PORTAL_BUILD ) )
      {
        cmdErrorString = "There is already a portal on this planet";
        return -3;
      }

      for( a = c = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
      {
        maind.ressource[a] -= resbuild[a];
        cost[a] = resbuild[a];
        if( maind.ressource[a] < 0 )
          c = 1;
      }
      planetd.flags |= CMD_PLANET_FLAGS_PORTAL_BUILD;
      dbUserPlanetSetFlags( cmd[1], cmd[4], planetd.flags );
      b = 1;
    }
    else if( !( cmd[2] >> 16 ) )
    {
      if( !( dbMapRetrievePlanet( cmd[4], &planetd ) ) )
        return -3;
      if( planetd.owner != cmd[1] )
      {
        cmdErrorString = "You do not own this planet";
        return -3;
      }
      d = planetd.construction;
      for( a = 0 ; a < CMD_BLDG_OVERBUILD ; a++ )
        d += planetd.building[a];
      fb = ( (float)cmd[5] / 100.0 ) + 1.0;
      if( fb < 1.001 )
        fb = 1.001;
      fc = ( (float)cmd[6] / 100.0 ) + 1.0;

      memset( costbuild, 0, CMD_RESSOURCE_NUMUSED*sizeof(float) );
      for( b = c = 0 ; b < cmd[3] ; b++ )
      {
        fa = cmdGetBuildOvercost( planetd.size, d+b );
        if( fa >= fb )
          break;
        if(fa < fc)
        	break;
        for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
        {
          costbuild[a] += (int64_t)(resbuild[a] * fa);
          if( maind.ressource[a] < costbuild[a] )
          {
            if( (b<<2) < cmd[3] )
              goto cmdExecuteL0;
            else
              c = 1;
          }
        }
      }
      for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
      {
        maind.ressource[a] -= (int64_t)(ceil( costbuild[a] ));
        cost[a] = ceil(costbuild[a]);
      }
    }
    else
    {
      for( a = c = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
      {
        maind.ressource[a] -= (int64_t)resbuild[a] * cmd[3];
        cost[a] = resbuild[a] * cmd[3];
        if( maind.ressource[a] < 0 )
          c = 1;
      }
      cmd[4] = -1;
      planetd.position = -1;
      b = cmd[3];
    }
    if( c )
    {
      d = sprintf( cmdErrorBuffer, "You lack " );
      for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
      {
        if( maind.ressource[a] < 0 )
          d += sprintf( &cmdErrorBuffer[d], "%+lld %s ", (long long)maind.ressource[a], cmdRessourceName[a] );
      }
      if( !( cmd[2] >> 16 ) )
        sprintf( &cmdErrorBuffer[d], "to build %d %s", b, cmdBuildingName[ cmd[2] & 0xFFF ] );
      else
        sprintf( &cmdErrorBuffer[d], "to build %d %s", b, cmdUnitName[ cmd[2] & 0xFFF ] );
      cmdErrorString = cmdErrorBuffer;
      return -1;

      cmdExecuteL0:
      sprintf( cmdErrorBuffer, "You clearly lack the resources, %s especially, to build %d %s", cmdRessourceName[a], cmd[3], cmdBuildingName[ cmd[2] & 0xFFF ] );
      cmdErrorString = cmdErrorBuffer;
      return -1;
    }
    if( !( b ) )
      return 0;
    if( !( cmd[2] >> 16 ) )
    {
      planetd.construction += b;
      dbMapSetPlanet( cmd[4], &planetd );
    }
    a = dbUserBuildAdd( cmd[1], cmd[2], cost, b, resbuild[CMD_RESSOURCE_TIME], cmd[4], planetd.position );
    if( a < 0 )
      return a;
    if( dbUserMainSet( cmd[1], &maind ) < 0 )
      return -3;
    return b;
  }
  else if( cmd[0] == CMD_RAZE_BUILDINGS )
  {
    if( !( dbMapRetrievePlanet( cmd[4], &planetd ) ) )
      return -3;
    if( dbUserMainRetrieve( cmd[1], &maind ) < 0 )
      return -3;
    if( planetd.owner != cmd[1] )
    {
      cmdErrorString = "You do not own this planet";
      return -3;
    }
    if( ( (unsigned int)cmd[3] >= 1000000000 ) || ( planetd.building[ cmd[2] ] < cmd[3] ) )
    {
      sprintf( cmdErrorBuffer, "You do not have so many %s on this planet", cmdBuildingName[ cmd[2] ] );
      cmdErrorString = cmdErrorBuffer;
      return -1;
    }
    planetd.building[ cmd[2] ] -= cmd[3];
    maind.totalbuilding[ cmd[2] ] -= cmd[3];
    dbMapSetPlanet( cmd[4], &planetd );
    if( dbUserMainSet( cmd[1], &maind ) < 0 )
      return -3;
    return 1;
  }
  else if( cmd[0] == CMD_RAZE_PORTAL )
  {
    if( !( dbMapRetrievePlanet( cmd[2], &planetd ) ) )
      return -3;
    if( dbUserMainRetrieve( cmd[1], &maind ) < 0 )
      return -3;
    if( planetd.owner != cmd[1] )
    {
      cmdErrorString = "You do not own this planet";
      return -3;
    }
    planetd.flags &= 0xFFFFFFFF - CMD_PLANET_FLAGS_PORTAL - CMD_PLANET_FLAGS_PORTAL_BUILD;
    dbUserPlanetSetFlags( cmd[1], cmd[2], planetd.flags );
    dbMapSetPlanet( cmd[2], &planetd );
    if( dbUserMainSet( cmd[1], &maind ) < 0 )
      return -3;
    return 1;
  }
  else if( cmd[0] == CMD_DISBAND_UNITS )
  {
    if( dbUserFleetRetrieve( cmd[1], cmd[4], &fleetd ) < 0 )
    {
      cmdErrorString = "This fleet does not exist";
      return -3;
    }
    if( dbUserMainRetrieve( cmd[1], &maind ) < 0 )
      return -3;
    if( ( (unsigned int)cmd[3] >= 1000000000 ) || ( fleetd.unit[ cmd[2] ] < cmd[3] ) )
    {
      sprintf( cmdErrorBuffer, "There aren't so many %s in this fleet", cmdUnitName[ cmd[2] ] );
      cmdErrorString = cmdErrorBuffer;
      return -1;
    }
    fleetd.unit[ cmd[2] ] -= cmd[3];
    maind.totalunit[ cmd[2] ] -= cmd[3];
    if( dbUserFleetSet( cmd[1], cmd[4], &fleetd ) < 0 )
      return -3;
    if( dbUserMainSet( cmd[1], &maind ) < 0 )
      return -3;
    return 1;
  }
  else if( cmd[0] == CMD_RETRIEVE_FLEET )
  {
    return dbUserFleetRetrieve( cmd[1], cmd[2], buffer );
  }
  else if( cmd[0] == CMD_SET_RESEARCH )
  {
    if( dbUserMainRetrieve( cmd[1], &maind ) < 0 )
      return -3;
    ibuffer = buffer;
    for( a = b = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
    {
      if( (unsigned int)ibuffer[a] >= 101 )
      {
        cmdErrorString = "The sum of the research fields percentages must be 100%";
        return -3;
      }
      b += ibuffer[a];
    }
    if( b != 100 )
    {
      cmdErrorString = "The sum of the research fields percetages must be 100%";
      return -3;
    }
    memcpy( maind.allocresearch, ibuffer, CMD_RESEARCH_NUMUSED*sizeof(int) );
    if( dbUserMainSet( cmd[1], &maind ) < 0 )
      return -3;
    return 1;
  }
  else if( cmd[0] == CMD_EXPLORE_INFO )
  {
    ibuffer = buffer;
    if( dbMapRetrievePlanet( cmd[2], &planetd ) < 0 )
      return -2;
    if( planetd.owner != -1 )
    {
      cmdErrorString = "This planet is not available for exploration!";
      return -3;
    }
    b = cmdFindDistPortal( cmd[1], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, &a, 0 );
    if( b == -1 )
    {
      cmdErrorString = "You don't have any portal to send an exploration ship from!";
      return -3;
    }
    else if( b < 0 )
      return -3;
    ibuffer[0] = ( ( a / cmdUnitStats[CMD_UNIT_EXPLORATION][CMD_UNIT_STATS_SPEED] ) >> 8 ) + 1;
    return 1;
  }
  else if( cmd[0] == CMD_EXPLORE )
  {
    ibuffer = buffer;
    if( dbMapRetrievePlanet( cmd[2], &planetd ) < 0 )
      return -2;
    if( planetd.owner != -1 )
    {
      cmdErrorString = "This planet is not available for exploration!";
      return -3;
    }

    if( !( dbUserFleetRetrieve( cmd[1], 0, &fleet2d ) ) )
      return -3;
    if( fleet2d.unit[CMD_UNIT_EXPLORATION] < 1 )
    {
      cmdErrorString = "You don't have any exploration ship to send away!";
      return -3;
    }

    memset( &fleetd, 0, sizeof(dbUserFleetDef) );
    b = cmdFindDistPortal( cmd[1], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, &a, &fleetd.source );
    if( b == -1 )
    {
      cmdErrorString = "You don't have any portal to send an exploration ship from!";
      return -3;
    }
    else if( b < 0 )
      return -3;
    if( dbUserMainRetrieve( cmd[1], &maind ) < 0 )
      return -3;
    if( maind.readiness[CMD_READY_FLEET] < 0 )
    {
      cmdErrorString = "Your people aren't prepared to send another exploration ship!";
      return -3;
    }
/*
    maind.readiness[CMD_READY_FLEET] -= ( (maind.planets<<16) >> 2 ) + 15*65536;
*/
    maind.readiness[CMD_READY_FLEET] -= ( (maind.planets<<16) >> 2 ) + 10*65536;

    ibuffer[0] = ( ( a / cmdUnitStats[CMD_UNIT_EXPLORATION][CMD_UNIT_STATS_SPEED] ) >> 8 ) + 1;


    fleetd.time = ibuffer[0];
    fleetd.basetime = ibuffer[0];
    fleetd.unit[CMD_UNIT_EXPLORATION] = 1;
    fleetd.order = CMD_FLEET_ORDER_EXPLORE;
    fleetd.destination = planetd.position;
    fleetd.destid = cmd[2];
    fleetd.sysid = planetd.system;
    fleetd.flags = 0;
    if( !( dbUserFleetAdd( cmd[1], &fleetd ) ) )
      return -3;

    fleet2d.unit[CMD_UNIT_EXPLORATION]--;
    if( !( dbUserFleetSet( cmd[1], 0, &fleet2d ) ) )
      return -3;

    return dbUserMainSet( cmd[1], &maind );
  }
  else if( cmd[0] == CMD_FIND_SYSTEM )
    return dbMapFindSystem( cmd[1], cmd[2] );
  else if( cmd[0] == CMD_UNSTATION_PLANET )
  {
    if( !( dbMapRetrievePlanet( cmd[2], &planetd ) ) )
      return -3;
    if( planetd.owner != cmd[1] )
    {
      cmdErrorString = "You can't unstation forces from a planet you do not own.";
      return -3;
    }
    for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
    {
      fleetd.unit[a] = planetd.unit[a];
      b |= fleetd.unit[a];
    }
    if( !( b ) )
    {
      cmdErrorString = "No units are stationed on this planet";
      return -3;
    }
    fleetd.order = CMD_FLEET_ORDER_CANCELED;
    fleetd.destination = planetd.position;
    fleetd.destid = cmd[2];
    fleetd.sysid = planetd.system;
    fleetd.source = planetd.position;
    fleetd.time = 0;
    fleetd.flags = 0;
    if( ( a = dbUserFleetAdd( cmd[1], &fleetd ) ) < 0 )
      return -3;
    memset( planetd.unit, 0, CMD_UNIT_NUMUSED*sizeof(int) );
    dbMapSetPlanet( cmd[2], &planetd );
    cmdFleetAction( &fleetd, cmd[1], cmd[2], 0 );
    return 1;
  }
  else if( cmd[0] == CMD_ATTACK_PLANET )
  {
    return battle( cmd[1], cmd[2], buffer );
  }

  return -2;
}


int cmdInit() {
	int id, a, pass, exist;
	char string[2][NAME_MAX];
	dbUserPtr user;
	dbUserInfoDef infod;
	ConfigArrayPtr settings[3];
	dbUserInfoPtr listarray;
	ConfigArrayPtr map;

char *list[3] = { "Map Size", "Map Systems", "Map Families" };
map = ListSettings( list );
free( map );

settings[0] = GetSetting( "Create Admins" );
settings[1] = GetSetting( "Admin Empire Number" );
settings[2] = GetSetting( "Admin Empire Password" );

listarray = ListAdmins( settings[0]->num_value );
for( a = 0; a < settings[0]->num_value; a++ ) {

	if( !( listarray[a].name ) || !( listarray[a].password ) || !( listarray[a].faction ) || ( ( id = dbUserSearch( listarray[a].name ) ) >= 0 ) )
		continue;

	info( "Creating %sAdministrator account named: \"%s\"", (( listarray[a].level >= LEVEL_MODERATOR ) ? "" : "Non-"), listarray[a].name );
	if( ( id = cmdExecNewUser( listarray[a].name, listarray[a].password, listarray[a].faction ) ) < 0 ) {
		error( "Failure Creating %sAdministrator account: \"%s\"", (( listarray[a].level >= LEVEL_MODERATOR ) ? "" : "Non-"), listarray[a].name );
		continue;
	}
	user = dbUserLinkID( id );
	user->flags = 0;
	user->level = (( listarray[a].level >= 0 ) ? listarray[a].level : 0);
	dbUserSave( id, user );
	dbUserInfoRetrieve(id, &infod);
	strncpy( infod.forumtag,listarray[a].forumtag, sizeof(infod.forumtag) );
	dbUserInfoSet(id, &infod);

	if( user->level >= LEVEL_MODERATOR ) { 
		info( "Placing Administrator account: \"%s\"", infod.name );
		if( cmdExecNewUserEmpire( id, (int)settings[1]->num_value, settings[2]->string_value, (( listarray[a].flags >= 0 ) ? listarray[a].flags : 0), user->level ) < 0 ) {
			error( "Failure Placing Administrator account: \"%s\"", infod.name );
			continue;
		}
	} else {
		info( "Placing Non-Administrator account: \"%s\"", infod.name );
		if( cmdExecNewUserEmpire( id, -1, NULL, (( listarray[a].flags >= 0 ) ? listarray[a].flags : 0), user->level ) < 0 ) {
			error( "Failure Placing Non-Administrator account: \"%s\"", infod.name );
			continue;
		}
	}
}
free( listarray );
UnloadSetting( "Create Admins" );
UnloadSetting( "Admin Empire Password" );


settings[0] = GetSetting( "Create Fakes" );
if( settings[0]->num_value > 0 ) {
	pass = exist = 0;
	for( a = 0; a < settings[0]->num_value; a++ ) {
		sprintf(string[0], "fake%05duser", a );
		sprintf(string[1], "Fake Faction %d", a );
		if( ( id = dbUserSearch( string[0] ) ) >= 0 ) {
			exist++;
			continue;
		}
		if( ( id = cmdExecNewUser( string[0], "password", string[1] ) ) < 0 ) {
			error( "Failure Creating account: \"%s\"", string[0] );
			continue;
		}
		user = dbUserLinkID( id );
		user->flags = 0;
		user->level = 0;
		dbUserSave( id, user );
		dbUserInfoRetrieve(id, &infod);
		strncpy( infod.forumtag,"Fake Account", sizeof(infod.forumtag) );
		dbUserInfoSet(id, &infod);
		srand( a + rand() );
		if( cmdExecNewUserEmpire( id, -1, NULL, ( rand() % CMD_RACE_TOTAL ) , user->level ) < 0 ) {
			error( "Failure Placing fake account: \"%s\"", infod.name );
			continue;
		}
	pass++;
	}
	if( exist ) {
		info( "%d fake account%s exist on server...", exist, ( exist > 1 ? "s" : "" ) );
	}
	if( pass ) {
		info( "Created %d fake account%s...", pass, ( pass > 1 ? "s" : "" ) );
	}
	RANDOMIZE_SEED;
}
UnloadSetting( "Create Fakes" );

dbFlush();

return 1;
}


void cmdEnd()
{
  return;
}
