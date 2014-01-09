#ifndef GLOBALINCLUDED
#include "config/global.h"
#endif

typedef struct {
	//rescource bonus/artefacts
	int *arti;
	int *posx;
	int *posy;
	int *type;
	//Systems
	int *pos;
	int *planets;
	int *pbase;
	int *home;
	int *system;
	//data for map image.
	int *data;
	int *factor;
} MapStorageDef, *MapStoragePtr;

static void mapCalcFactors( ConfigArrayPtr map_set, MapStoragePtr mapstore ) {
	int a, b, c, x, y, index;
	float fx, fy, dist, mindist;
	float fpos[2];
	float angle, anglevar;
	float fdir[2];
	float pts[ARRAY_MAX][2];
	int ptsnum;

ptsnum = 0;

for( a = 0 ; a < map_set[8].num_value ; a++ ) {
	fpos[0] = (float)( map_set[5].num_value + ( rand() % (int)( map_set[0].num_value-2*map_set[5].num_value ) ) );
	fpos[1] = (float)( map_set[5].num_value + ( rand() % (int)( map_set[1].num_value-2*map_set[5].num_value ) ) );
	angle = rand() % 360;
	anglevar = (float)( ( rand() & 0xFFF ) - 0x800 ) / map_set[10].num_value;

	c = map_set[6].num_value + ( rand() % (int)map_set[7].num_value );
	for( b = 0 ; b < c ; b++ ) {
		pts[ptsnum+b][0] = fpos[0];
		pts[ptsnum+b][1] = fpos[1];
		fdir[0] = ANG_COS( angle );
		fdir[1] = ANG_SIN( angle );
		fpos[0] += fdir[0];
		fpos[1] += fdir[1];
		angle += anglevar;
	}
	ptsnum += c;
}

for( y = index = 0 ; y < map_set[1].num_value ; y++ ) {
	for( x = 0 ; x < map_set[1].num_value ; x++, index++ ) {
		mindist = 0;
		for( a = 0 ; a < ptsnum ; a++ ) {
			fx = (float)x - pts[a][0];
			fy = (float)y - pts[a][1];
			dist = sqrt( fx*fx + fy*fy );
			if( dist >= map_set[9].num_value )
			continue;
			mindist += map_set[9].num_value - dist;
		}
		mapstore->factor[index] += (int)floor( mindist );
	}
}




return;
}


int spawn_map() {
	ConfigArrayPtr settings[4];
	ConfigArrayPtr map_set;
	int a, b, c, d, e, i, p, x, y, x2, y2;
	int map_bonus[CMD_BONUS_NUMUSED+1];
	int64_t j;
	float dist, distmax;
	char fname[PATH_MAX];
	char imgsizer[2048];
	uint8_t *pixels, *bigpixies; 
	FILE *file;
	FILE *file2;
	imgImage mapimage;
	dbMainMapPtr mapbase;
	MapStoragePtr mapstore;
	dbMainSystemDef systemd;
	dbMainPlanetDef planetd;
	dbMainEmpireDef empired;

//Yer... I'm so not calling these one-by-one, list calling time!! *sigh* lol =P
static char *list[] = { "Map Size", "Map Size", "Map Systems", "Map Families", "Map Family Members", "Map Border", "Map Lenght", "Map Lenght Var", "Map Link", "Map Link Radius", "Map Angle Var" };
map_set = ListSettings( list );
if( map_set == NULL )
	goto DIE;
/*
for( a = 0; map_set[a].num_value; a++ ) {
	printf( "%d - ( %d ) %s: %f\n", a, map_set[a].id, map_set[a].name, map_set[a].num_value );
}
*/
if( ( mapstore = calloc( 1, sizeof(MapStorageDef) ) ) == NULL ) {
	critical( "No memory for map storage" );
	goto DIE;
}

map_bonus[CMD_BONUS_NUMUSED+1] = 0;
for(a = 0; a < CMD_BONUS_NUMUSED-1; a++) {
	sprintf(fname,"Map %s Bonus",cmdBonusName[a]);
	if( ( settings[0] = GetSetting( fname ) ) == NULL )
		goto DIE;
	map_bonus[a] = settings[0]->num_value;
	map_bonus[CMD_BONUS_NUMUSED+1] += map_bonus[a];
}

//haha, naa... we won't check these... in all my tests they've never failed anyways (and if the above works, theres almost no reason this should fail)
mapstore->factor = calloc( map_set[0].num_value*map_set[1].num_value, sizeof(int) );
mapstore->data = calloc( map_set[0].num_value*map_set[1].num_value, sizeof(int) );
mapstore->posx = calloc( map_bonus[CMD_BONUS_NUMUSED+1], sizeof(int) );
mapstore->posy = calloc( map_bonus[CMD_BONUS_NUMUSED+1], sizeof(int) );
mapstore->type = calloc( map_bonus[CMD_BONUS_NUMUSED+1], sizeof(int) );
mapstore->pos = calloc( map_set[2].num_value, sizeof(int) );
mapstore->planets = calloc( map_set[2].num_value, sizeof(int) );
mapstore->pbase = calloc( map_set[2].num_value, sizeof(int) );
mapstore->home = calloc( map_set[2].num_value, sizeof(int) );
mapstore->system = calloc( map_set[3].num_value, sizeof(int) );
mapstore->arti = calloc( ARTEFACT_NUMUSED, sizeof(int) );

mapCalcFactors( map_set, mapstore );

settings[0] = GetSetting( "Directory" );
sprintf( fname, "%s/data", settings[0]->string_value );
dirstructurecheck(fname);

RANDOMIZE_SEED;

mapimage.width = map_set[0].num_value;
mapimage.height = map_set[1].num_value;
mapimage.format = IMG_IMAGE_FORMAT_GRAYSCALE;
mapimage.bytesperpixel = 1;
mapimage.bytesperline = mapimage.width;


for( a = 0 ; a < map_set[3].num_value ; a++ ) {
	mainL1:
	mapstore->system[a] = rand() % (int)map_set[2].num_value;
	if( mapstore->home[ mapstore->system[a] ] )
		goto mainL1;
	mapstore->home[ mapstore->system[a] ] = a+1;
	mapstore->planets[ mapstore->system[a] ] = map_set[4].num_value;
}

for( a = b = c = 0 ; a < map_bonus[CMD_BONUS_NUMUSED+1] ; a++, b++ ) {
	mapstore->posx[a] = rand() % (int)map_set[0].num_value;
	mapstore->posy[a] = rand() % (int)map_set[1].num_value;
	if( b >= map_bonus[c] ) {
		b -= map_bonus[c];
		c++;
	}
	mapstore->type[a] = c;
}

p = 0;
for( a = 0 ; a < map_set[2].num_value ; a++ ) {
	mainL0:
	for( ; ; ) {
		x = rand() % (int)map_set[0].num_value;
		y = rand() % (int)map_set[1].num_value;
		i = ( y * map_set[0].num_value ) + x;
		if( ( rand() & 0xFF ) >= mapstore->factor[i] )
			continue;
		break;
	}
	if( mapstore->data[i] )
		goto mainL0;
	mapstore->pos[a] = ( y << 16 ) + x;
	if( !( mapstore->planets[a] ) ) {
		mapstore->planets[a] = 4 + rand() % 4;
		if( !( rand() & 7 ) )
			mapstore->planets[a] += rand() % 12;
		if( !( rand() & 7 ) )
			mapstore->planets[a] += rand() % 8;

	}
	mapstore->pbase[a] = p;
	p += mapstore->planets[a];
	mapstore->data[i] = 0xFF;
}


for( a = 0 ; a < ARTEFACT_NUMUSED ; a++ ) {
	mainL2:
	b = rand() % (int)map_set[2].num_value;
	if( mapstore->home[b] )
		goto mainL2;
	mapstore->arti[a] = mapstore->pbase[b] + ( rand() % mapstore->planets[b] );
	info( "( %d,%d ) ID:%d Holds: %s", mapstore->pos[b] & 0xFFFF, mapstore->pos[b] >> 16, mapstore->arti[a], artefactName[a] );
}

if( NULL == ( mapbase = calloc( 1, sizeof(dbMainMapDef) ) ) ) {
	critical( "Map Base Malloc Failed" );
	goto DIE;
}
// OK, a new headers write.
sprintf( fname, "%s/data/map", settings[0]->string_value );
file = fopen( fname, "wb" );
mapbase->sizex = map_set[0].num_value;
mapbase->sizey = map_set[1].num_value;
mapbase->systems = map_set[2].num_value;
mapbase->planets = p;
mapbase->families = map_set[3].num_value;
mapbase->fmembers = map_set[4].num_value;
mapbase->capacity = map_set[3].num_value * map_set[4].num_value;
mapbase->artitimer = -1;
mapbase->timempire = -1;
file_w( mapbase, 1, sizeof(dbMainMapDef), file ); 
free( mapbase );
// New system generation, based on defaults.
p = 0;
for( a = 0 ; a < map_set[2].num_value ; a++ ) {
	memset( &systemd, -1, sizeof(dbMainSystemDef) );
	systemd.position = mapstore->pos[a];
	systemd.indexplanet = p;
	p += mapstore->planets[a];
	systemd.numplanets = mapstore->planets[a];
	if( mapstore->home[a] ) {
		systemd.empire = mapstore->home[a] - 1;
	} else {
		systemd.unexplored = mapstore->planets[a];
	}
	file_w( &systemd, 1, sizeof(dbMainSystemDef), file );
}
//End system generation

// New planet generation, based on defaults.
for( a = b = c = 0 ; a < p ; a++, b++ ) {
	memset( &planetd, -1, sizeof(dbMainPlanetDef) );
	memset( planetd.building, 0, CMD_BLDG_NUMUSED*sizeof(int) );
	memset( planetd.unit, 0, CMD_UNIT_NUMUSED*sizeof(int) );
	planetd.construction = planetd.protection = 0;
	dist = 0;
	if( b >= mapstore->planets[c] ) {
		b -= mapstore->planets[c];
		c++;
	}
	planetd.system = c;
	x = mapstore->pos[c] & 0xFFFF;
	y = mapstore->pos[c] >> 16;
	i = ( y << 20 ) + ( x << 8 ) + b;
	planetd.position = i;
	if( !( mapstore->home[c] ) ) {
		planetd.flags = 0;
		planetd.size = 128 + ( rand() % 192 );
		if( !( rand() & 7 ) )
			planetd.size += rand() & 255;
		if( !( rand() & 31 ) )
			planetd.size += rand() & 511;
	} else {
		planetd.size = 450;
		planetd.flags = CMD_PLANET_FLAGS_HOME;
	}

	e = i = 0;
	if( !( mapstore->home[c] ) ) {
		distmax = (float)0xFFFF;
		for( d = 0 ; d < map_bonus[CMD_BONUS_NUMUSED+1] ; d++ ) {
			x2 = x - mapstore->posx[d];
			y2 = y - mapstore->posy[d];
			dist = sqrt( x2*x2 + y2*y2 );
			if( dist < distmax ) {
				distmax = dist;
				e = mapstore->type[d];
			}
		}
		d = 160.0 * dist;
		if( !( d ) || ( ( rand() % d ) < 1024 ) ) {
			i = 25 + rand() % 25;
			if( !( rand() & 7 ) )
				i += rand() % 100;
			if( !( rand() & 15 ) )
				i += rand() % 100;
		}
	}
	planetd.special[0] = e;
	planetd.special[1] = i;
	// artefacts
	i = 0;
	for( d = 0 ; d < ARTEFACT_NUMUSED ; d++ ) {
		if( a != mapstore->arti[d] )
			continue;
		i = d + 1;
		break;
	}
	planetd.special[2] = i;
	file_w( &planetd, 1, sizeof(dbMainPlanetDef), file );
}
//End planet generation


// New families generation, based on defaults.
settings[1] = GetSetting( "Admin Empire Number" );
settings[2] = GetSetting( "Admin Empire Name" );
settings[3] = GetSetting( "Admin Empire Password" );
for( a = 0 ; a < map_set[3].num_value ; a++ ) {
	memset( &empired, 0, sizeof(dbMainEmpireDef) );
	memset( empired.player, -1, map_set[4].num_value*sizeof(int) );
	memset( empired.vote, -1, map_set[4].num_value*sizeof(int) );
	empired.leader = empired.rank = -1;
	sprintf(empired.message[0],"<i>Welcome to Empire #%d!<i>",a);
	if( ( (int)settings[1]->num_value == a ) ) {
		strcpy( empired.name, settings[2]->string_value );
		strcpy( empired.password, hashencrypt(settings[3]->string_value) );
		info( "Empire %d Claimed for Administration.", a);
	}
	empired.homeid = mapstore->system[a];
	empired.homepos = mapstore->pos[ mapstore->system[a] ];
	file_w( &empired, 1, sizeof(dbMainEmpireDef), file );
// FIXME: Well duh... we got all the other stuff nice and flexible... what about this!
	sprintf( fname, "%s/data/fam%dnews", settings[0]->string_value, a );
	file2 = fopen( fname, "wb" );
	j = 0;
	file_w( &j, 1, sizeof(int64_t), file2 );
	j = -1;
	file_w( &j, 1, sizeof(int64_t), file2 );
	file_w( &j, 1, sizeof(int64_t), file2 );
	j = 0;
	file_w( &j, 1, sizeof(int64_t), file2 );
	file_w( &j, 1, sizeof(int64_t), file2 );
	fclose( file2 );
}
fclose( file );
UnloadSetting( "Admin Empire Password" );
UnloadSetting( "Admin Empire Name" );
//End family generation

//Arightyz, we've got all the data... now lets put it to some use and spalt a image out.
pixels = calloc( map_set[0].num_value*map_set[1].num_value, sizeof(int) );

for( y = 0 ; y < map_set[1].num_value ; y++ ) { 
	for( x = 0 ; x < map_set[0].num_value ; x++ )  {
		pixels[(y*((int)map_set[0].num_value))+x] = (mapstore->factor[(y*((int)map_set[0].num_value))+x] >> 1 ); //here the milk gets set
		if( mapstore->data[(y*((int)map_set[0].num_value))+x] )
			pixels[(y*((int)map_set[0].num_value))+x] = mapstore->data[(y*((int)map_set[0].num_value))+x]; //here the dots get pointed
	}  
}


bigpixies = NULL;
/*
mapimage.width = (int)map_set[0].num_value *3;
mapimage.height = (int)map_set[1].num_value *3;
bigpixies = calloc( mapimage.height*mapimage.width, sizeof(int) );


for( x = 0; x < mapimage.width; x++) {
	for( y = 0; y < mapimage.height; y++) {
		bigpixies[(y* mapimage.width)+x] = pixels[((y*((int)map_set[0].num_value))+x)];
	}
}

for( y = 0 ; y < mapimage.height ; y++ ) { 
	for( x = 0 ; x < mapimage.width ; x++ )  {
		bigpixies[x+y] = pixels[( x * (int)map_set[0].num_value ) + ( y * mapimage.width )];
	}  
}
*/



if( mapimage.width > map_set[0].num_value ) {
	mapimage.data = bigpixies;
	free( pixels );
} else {
	mapimage.data = pixels;
}
settings[0] = GetSetting( "HTTP Images" );
sprintf( fname, "%s/galaxies", settings[0]->string_value );
dirstructurecheck(fname);

settings[1] = GetSetting( "Round Number" );
sprintf( fname, "%s/galaxies/galaxyr%.0f.png", settings[0]->string_value, settings[1]->num_value );
imgConvertGrayscale(&mapimage,IMG_IMAGE_FORMAT_RGB24);
imgWritePngFile( fname, &mapimage );
imgFree( &mapimage );

/*

//FIXME: Such a dirty fix, but well... it works. =/
if( mapimage.width == map_set[0].num_value ) {
	sprintf(imgsizer, "convert \"%s\" -resize 300%% \"%s\"", fname, fname );
	if( system(imgsizer) ) {
		error( "unable to resize map" );
	}

}
*/

//Ahh yes, I'll free all those malloc's back up now... better safe eh.

free(mapstore->data);
free(mapstore->factor);

free(mapstore->posx);
free(mapstore->posy);
free(mapstore->type);

free(mapstore->pos);
free(mapstore->planets);
free(mapstore->pbase);
free(mapstore->home);
free(mapstore->system);

free(mapstore->arti);

free(mapstore);

free( map_set );

return YES;
DIE:
return NO;
}


