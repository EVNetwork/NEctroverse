#include "config/global.h"

int artefactPrecense( dbMainPlanetPtr planetd )
{
  if( !( planetd->special[2] ) )
    return -1;
  return ( planetd->special[2] - 1 );
}


int ArtefactNum = 0;
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
		if( ArtefactNum >= SERVER_ARTIFACT_MAX ) {
			error( "Artefact overflow: #%d %s will not be enabled", ArtefactRead->id, ArtefactRead->name );
		}
		ArtefactNum++;
		ArtefactTable = realloc( ArtefactTable, ArtefactNum * sizeof(dbArtefactDef) );
		ArtefactTable[ArtefactNum-1] = ArtefactRead;
	}
}

ArtefactNum = fmin( SERVER_ARTIFACT_MAX, ArtefactNum );

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


