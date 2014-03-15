#ifndef MAP_H
#define MAP_H
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct
{
  int sizex;
  int sizey;
  int systems;
  int planets;
  int megasys;
  int families;
  int fmembers;
  int capacity;
  int artitimer;
  int timempire;
} dbMainMapDef, *dbMainMapPtr;

typedef struct
{
  int id;
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

typedef struct
{
  int id;
  int position;
  int indexplanet;
  int numplanets;
  int empire;
  int unexplored;
  int flags;
  int timer;
} dbMainSystemDef, *dbMainSystemPtr;

typedef struct
{
  char leader[USER_DESC_MAX];
  char relations[USER_DESC_MAX];
} dbEmpireMessageDef, *dbEmpireMessagePtr;

/*
 * The use of ARRAY_MAX in here, is un-desirable... but it prevents any future over-flow as its limit is 65k 
 * This is the same limit as max players, so each Empire is capable of holding all players... technicly. Not that it should.
 */
 
typedef struct
{
  int id;
  int rank;
  int flags;
  int numplayers;
  int politics[CMD_EMPIRE_POLITICS_TOTAL];
  int player[ARRAY_MAX];
  int vote[ARRAY_MAX];
  int depreciated;
  int homeid;
  int homepos; // ( y << 16 ) + x
  int picture;
  int planets;
  int artefacts;
  int construction;
  int building[CMD_BLDG_EMPIRE_NUMUSED];
  int counters[16];
  float taxation;
  int64_t networth;
  int64_t fund[CMD_RESSOURCE_NUMUSED];
  int64_t infos[CMD_RESSOURCE_NUMUSED];
  char name[USER_NAME_MAX];
  char password[USER_PASS_MAX];
} dbMainEmpireDef, *dbMainEmpirePtr;

int spawn_map();

extern char *mapStarName[];

#endif
