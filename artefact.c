#include "config/global.h"

int randnum(void){
	int randnum;
	randnum = rand()&32767; 
	
	if (randnum < 5462)
		return 0;
	else if(randnum >= 5462 && randnum < 10923)
		return 1;
	else if(randnum >= 10923 && randnum < 16384)
		return 2;
	else if(randnum >= 16384 && randnum < 21845)
		return 3;
	else if(randnum >= 21845 && randnum < 27306)
		return 4;
	else
		return 5;
}


//arti code get planet in exhcnge of lost
int exchange_planet(int id, dbMainPlanetDef planetd){
	int a, i;
	double plnx, plnx2, plny, plny2, dist, max_dist;
	dbMainPlanetDef planetd2;
	dbMainSystemDef systemd;
	dbUserMainDef maind;
	plnx = (double)(( planetd.position >> 8 ) & 0xFFF);
	plny = (double)(planetd.position >> 20);
	int num_of_planets;
	num_of_planets = 1;
	
	if( dbUserMainRetrieve( id, &maind ) < 0 )
		return -3;
	
	max_dist = 7 * cmdRace[maind.raceid].speed * specopEnlightemntCalc(id,CMD_ENLIGHT_SPEED);			
	i = 0;
	for ( ; max_dist < 30; max_dist++){
		for( a = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
		{
			  dbMapRetrievePlanet( a, &planetd2 );
			  plnx2 = (double)(( planetd2.position >> 8 ) & 0xFFF);
			  plny2 = (double)(planetd2.position >> 20);
			  dist = sqrt((pow((plnx2-plnx),2)+pow((plny2-plny),2)) );
			  if(dist > max_dist || planetd2.flags & CMD_PLANET_FLAGS_HOME || planetd2.owner != -1 || planetd2.nuked != -1)
				continue;
			  if(rand() % 100 < 10){
				++i;
				if( dbUserPlanetAdd( id, a, planetd2.system, planetd2.position, 0 ) < 0 )
					return -3;
				
				dbMapRetrieveSystem( planetd2.system, &systemd );
				systemd.unexplored--;
				dbMapSetSystem( planetd2.system, &systemd );

				planetd2.maxpopulation = (float)( planetd2.size * CMD_POPULATION_SIZE_FACTOR );
				planetd2.flags = ( bitflag( systemd.flags, MAP_SYSTEM_FLAG_MEGA ) ? CMD_PLANET_FLAGS_MEGA : 0 );
				planetd2.owner = id;
				planetd2.population = planetd2.size * CMD_POPULATION_BASE_FACTOR;
				planetd2.protection = 0.0;
				memset( planetd2.building, 0, CMD_BLDG_NUMUSED*sizeof(int64_t) );
				memset( planetd2.unit, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );
				planetd2.construction = 0;
				dbMapSetPlanet( a, &planetd2 );
			  }
			  if (i >= num_of_planets){
				  printf("TEST %d %d\n",a, planetd2.system);
				  return a;
				  
			  }
		}
	}
	
	return -2;
}				

	
	//arti code metamorphosis stone
void metamorphosis_effect(dbUserMainDef *maind, int id)
{	
	int a;
	dbUserMainDef maind22;
	dbMainEmpirePtr empirep;
	int64_t *stats;
		
	stats = malloc( 6*dbMapBInfoStatic[MAP_EMPIRES]*sizeof(int64_t) + sizeof(dbMainEmpireDef)*dbMapBInfoStatic[MAP_EMPIRES] ) ;
	empirep = (void *)&stats[6*dbMapBInfoStatic[MAP_EMPIRES]];	
		
	for( a = 0 ; a < empirep[maind->empire].numplayers ; a++ )
		{
		  if( dbUserMainRetrieve( empirep[maind->empire].player[a], &maind22 ) < 0 )
			continue;
		  
		  maind22.raceid = randnum();
		  dbUserMainSet( empirep[maind->empire].player[a], &maind22 ); 
		}
	
	maind->raceid = randnum();
	dbUserMainSet( id, maind );
	free(stats);	
}
//arti code

int artefactPrecense( dbMainPlanetPtr planetd )
{
  if( !( planetd->special[2] ) )
    return -1;
  return ( planetd->special[2] - 1 );
}

/*
int ARTEFACT_NUMUSED = 0;
int ArtefactTotal = 0;
dbArtefactPtr ArtefactList;
dbArtefactPtr *ArtefactTable;

int LoadArtefacts( ) {
	dbArtefactPtr ArtefactRead;
	ConfigArrayPtr settings;
	int a;
	char fname[PATH_MAX];
	char COREDIR[PATH_MAX];

settings = GetSetting( "Directory" );
snprintf( fname, sizeof(fname), "%s/data", settings->string_value );
snprintf( COREDIR, sizeof(COREDIR), dbFileList[DB_FILE_BASE_ARTES], fname );
if( !( dbFilePtr[DB_FILE_BASE_ARTES] = fopen( COREDIR, "rb+" ) ) ) {
	info( "Artefact database not found, creating..." );

	if( !( dbFilePtr[DB_FILE_BASE_ARTES] = fopen( COREDIR, "wb+" ) ) ) {
		critical( "Error, could not create artefact database!" );
		return NO;
	}

	file_s( dbFilePtr[DB_FILE_BASE_ARTES], 0 );
	ArtefactTotal = 0;
	file_w( &ArtefactTotal, 1, sizeof(int), dbFilePtr[DB_FILE_BASE_ARTES] );
}
file_s( dbFilePtr[DB_FILE_BASE_ARTES], 0 );
file_r( &ArtefactTotal, 1, sizeof(int), dbFilePtr[DB_FILE_BASE_ARTES] );

for( a = 0; a < ArtefactTotal; a++ ) {
	if( NULL == ( ArtefactRead = malloc( sizeof(dbArtefactDef) ) ) ) {
		critical( "memory allocation failed" );
		return NO;
	}
	file_r( ArtefactRead, 1, sizeof(dbArtefactDef), dbFilePtr[DB_FILE_BASE_ARTES] );
	ArtefactRead->next = ArtefactList;
	ArtefactList = ArtefactRead;
	if( ArtefactRead->active == true ) {
		if( ARTEFACT_NUMUSED >= SERVER_ARTIFACT_MAX ) {
			error( "Artefact overflow: #%d %s will not be enabled", ArtefactRead->id, ArtefactRead->name );
		}
		ARTEFACT_NUMUSED++;
		ArtefactTable = realloc( ArtefactTable, ARTEFACT_NUMUSED * sizeof(dbArtefactDef) );
		ArtefactTable[ARTEFACT_NUMUSED-1] = ArtefactRead;
	}
}


ARTEFACT_NUMUSED = fmin( SERVER_ARTIFACT_MAX, ARTEFACT_NUMUSED );

return YES;
}


void UnLoadArtefacts( ) {
	dbArtefactPtr FreeList;

for( FreeList = ArtefactList; FreeList; FreeList = FreeList->next ) {
	free( FreeList );
}

if( ArtefactTable ) {
	free( ArtefactTable );
	ArtefactTable = NULL;
}

return;
}
*/


char *RoundSpecName[ROUNDSPEC_NUMUSED] =
{
"Exploration costs are thirded!",
"All races have observe planet operation!",
"Race can be changed every 24h!",
"Galactic congress forbids destroying ancient heritage!"
};

char *RoundSpecImage[ROUNDSPEC_NUMUSED] =
{
"artimg19.gif",
"artimg15.gif",
"artimg16.gif",
"artimg17.gif"
};

int roundspec_define[ROUNDSPEC_NUMUSED] =
{ 
0, 
0,
0,
1
};
/*
rokeil's peace manual - you cant declare war vs this empire!
wilbo's big bones - your nw affects your research by 20% less!
blame's esurge fetish - u get esurge inca and its difficulty reduced by 3!
*/
char *artefactName[ARTEFACT_NUMUSED] =
{
"Rokeil's peace treaty", //ARTEFACT_1_BIT 1 //rokeil's whine manual 
"Purification Subprocessor", //ARTEFACT_2_BIT 2
"Wilbo's big bones", //ARTEFACT_4_BIT 4 //wilbo's big bones
"Defenders of the faith", // ARTEFACT_8_BIT 8 

"Effisiont working guide", //ARTEFACT_16_BIT 16
"Phantom Stranger", //ARTEFACT_32_BIT 32
"Evolution Obelisk", //ARTEFACT_64_BIT 64
"Furtifons shield", //ARTEFACT_128_BIT 128 //furtifons shield

"Ether Gardens", //ARTEFACT_ETHER_BIT 256 
"Blame's esurge fetish", //ARTEFACT_512_BIT 512 //blame's esurge fetish
};

char *artefactDescription[ARTEFACT_NUMUSED] =
{
"The <b>Rokeil's peace treaty</b> You cant declare war vs this empire!", 
"The <b>Purification Subprocessor</b> doubles your planets bonusses!", 
"The <b>Wilbo's big bones</b> your nw is decreased by 20%!",
"The <b>Defenders of the faith</b> always provide you with a defensive fleet of 100k cruisers!", 

//"The <b>Universal Exchanger</b> gives you an unexplored planet, when you lose a planet in a battle!",
"The <b>Effisiont working guide</b> removes fissions nw and each fission gives 1 Research point a tick!",
"The <b>Phantom Stranger</b> allows you to build phantoms, even cheaper this round!",
"The <b>Evolution Obelisk</b> raises the maximum of all research fields by 20%!",
//"The <b>Ancient Research Center</b> increases the production of your research centers by 20%!",
"The <b>Furtifons shield</b> increases the fr cost of attacking you depending on your culture research!", 

"The <b>Ether Gardens</b> increase your energy production by 10%!",
"The <b>Blame's esurge fetish</b> makes an e-surge 2 times easier and grants you the this incantation!",
};

char *artefactImage[ARTEFACT_NUMUSED] =
{
"artimg4.gif",
"artimg12.gif",
"ether3.gif",
"ether2.gif",

"artimg3.gif",
"ether4.gif",
"artimg2.gif",
"artimg14.gif",

"artimg0.gif",
"artimg6.gif",
};


