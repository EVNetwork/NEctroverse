#ifndef CONFIGINCLUDED
#include "config.h"
#endif
#ifndef GLOBALINCLUDED
#include "global.h"
#endif

#include "imgpng.c"

/*
#define MAP_SIZEX (480)
#define MAP_SIZEY (100)
#define MAP_SYSTEMS (1100)
#define MAP_FAMILIES (100)
#define MAP_FAMMEMBERS (7)

#define MAP_GENBORDER (20)
#define MAP_GENANGLEVAR (1024.0)

#define MAP_GEN_LNKNUM (80)
#define MAP_GEN_LNKRADIUS (8.0)
#define MAP_GEN_LNKLENGHTBASE (2)
#define MAP_GEN_LNKLENGHTVAR (48)
*/


// These can move into config.h soonish.
#define MAP_SIZEX (105)
#define MAP_SIZEY (105)
#define MAP_SYSTEMS (709)

#define MAP_FAMILIES (61)
#define MAP_FAMMEMBERS (7)

#define MAP_ARTEFACTS (9)

#define MAP_GENBORDER (20)
#define MAP_GENANGLEVAR (1024.0)

#define MAP_GEN_LNKNUM (60)
#define MAP_GEN_LNKRADIUS (8.0)
#define MAP_GEN_LNKLENGHTBASE (2)
#define MAP_GEN_LNKLENGHTVAR (64)



#define MAP_RESOURCES (25+20+10+8)

// solar, mineral, crystal, ectrolium
int map_resources_gen[4] = { 24, 23, 17, 10 };

int map_resources_posx[MAP_RESOURCES];
int map_resources_posy[MAP_RESOURCES];
int map_resources_type[MAP_RESOURCES];
int mapdata[MAP_SIZEX*MAP_SIZEY];
int system_pos[MAP_SYSTEMS];
int system_planets[MAP_SYSTEMS];
int system_pbase[MAP_SYSTEMS];
int system_home[MAP_SYSTEMS];
int empire_system[MAP_FAMILIES];
int artefact_planet[MAP_ARTEFACTS];

#define CMD_PLANET_FLAGS_HOME 4

int mapfactor[MAP_SIZEX*MAP_SIZEY];


void mapGetRandnorm( float *fvars )
{
  float norm;
  for( ; ; )
  {
    fvars[0] = (float)( ( rand() & 0xFF ) - 0x80 );
    fvars[1] = (float)( ( rand() & 0xFF ) - 0x80 );
    norm = sqrt( fvars[0]*fvars[0] + fvars[1]*fvars[1] );
    if( norm < 10.0 )
      continue;
    fvars[0] /= norm;
    fvars[1] /= norm;
    break;
  }
  return;
}


#define DEF_PI 3.14159265358979323846
#define ANG_SIN(x) sin((x*2*DEF_PI)/360.0)
#define ANG_COS(x) cos((x*2*DEF_PI)/360.0)

uint8_t *mapCalcFactors()
{
  int a, b, c, x, y, index;
  float fx, fy, dist, mindist;
  float fpos[2];
  float angle, anglevar;
  float fdir[2];
	char fname[256];
  float pts[65536][2];
  int ptsnum;
  FILE *file;

  ptsnum = 0;
  memset( mapfactor, 0, sizeof(int)*MAP_SIZEX*MAP_SIZEY );
  for( a = 0 ; a < MAP_GEN_LNKNUM ; a++ )
  {
    fpos[0] = (float)( MAP_GENBORDER + ( rand() % ( MAP_SIZEX-2*MAP_GENBORDER ) ) );
    fpos[1] = (float)( MAP_GENBORDER + ( rand() % ( MAP_SIZEY-2*MAP_GENBORDER ) ) );
    angle = rand() % 360;
    anglevar = (float)( ( rand() & 0xFFF ) - 0x800 ) / MAP_GENANGLEVAR;

    c = MAP_GEN_LNKLENGHTBASE + ( rand() % MAP_GEN_LNKLENGHTVAR );
    for( b = 0 ; b < c ; b++ )
    {
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

for( y = index = 0 ; y < MAP_SIZEY ; y++ ) {
	for( x = 0 ; x < MAP_SIZEY ; x++, index++ ) {
		mindist = 0;
		for( a = 0 ; a < ptsnum ; a++ ) {
			fx = (float)x - pts[a][0];
			fy = (float)y - pts[a][1];
			dist = sqrt( fx*fx + fy*fy );
			if( dist >= MAP_GEN_LNKRADIUS )
			continue;
			mindist += MAP_GEN_LNKRADIUS - dist;
		}
		mapfactor[index] += (int)floor( mindist );
	}
}


uint8_t *pixies;   
pixies = malloc( MAP_SIZEX * MAP_SIZEY );
for( y = 0 ; y < MAP_SIZEY ; y++ ) { 
	for( x = 0 ; x < MAP_SIZEX ; x++ )  {
		pixies[(y*MAP_SIZEX)+x] = 0;
	    if( mapfactor[(y*MAP_SIZEX)+x] > 0xFF ) {
		pixies[(y*MAP_SIZEX)+x] = 126;
		}

	}  
}

  return pixies;
}


int mapgen() {
	int a, b, c, d, e, i, p, x, y, x2, y2;
	long long int j;
	float dist, distmax;
	char fname[256];
	char imgsizer[2048];
	uint8_t *pixels, *bigpixies; 
	FILE *file;
	FILE *file2;
	imgImage mapgen;
	dbMainMapDef mapd;
	dbMainSystemDef systemd;
	dbMainPlanetDef planetd;
	dbMainEmpireDef empired;



memset( &mapd, 0, sizeof(dbMainMapDef) );
memset( &systemd, 0, sizeof(dbMainSystemDef) );
memset( &planetd, 0, sizeof(dbMainPlanetDef) );
memset( &empired, 0, sizeof(dbMainEmpireDef) );

openlog(LOGTAG, LOG_PID | LOG_NDELAY, LOGFAC);
sprintf( fname, "%s/data", COREDIRECTORY );
dirstructurecheck(fname);

srand( time( 0 ) );

mapgen.width = MAP_SIZEX;
mapgen.height = MAP_SIZEY;
mapgen.format = IMG_IMAGE_FORMAT_GRAYSCALE;
mapgen.bytesperpixel = 1;
mapgen.bytesperline = mapgen.width;

pixels = mapCalcFactors();

for( a = 0 ; a < MAP_FAMILIES ; a++ ) {
	mainL1:
	empire_system[a] = rand() % MAP_SYSTEMS;
	if( system_home[ empire_system[a] ] )
		goto mainL1;
	system_home[ empire_system[a] ] = a+1;
	system_planets[ empire_system[a] ] = MAP_FAMMEMBERS;
}

for( a = b = c = 0 ; a < MAP_RESOURCES ; a++, b++ ) {
	map_resources_posx[a] = rand() % MAP_SIZEX;
	map_resources_posy[a] = rand() % MAP_SIZEY;
	if( b >= map_resources_gen[c] ) {
		b -= map_resources_gen[c];
		c++;
	}
	map_resources_type[a] = c;
}

p = 0;
for( a = 0 ; a < MAP_SYSTEMS ; a++ ) {
	mainL0:
	for( ; ; ) {
		x = rand() % MAP_SIZEX;
		y = rand() % MAP_SIZEY;
		i = ( y * MAP_SIZEX ) + x;
		if( ( rand() & 0xFF ) >= mapfactor[i] )
			continue;
		break;
	}
	if( mapdata[i] )
		goto mainL0;
	system_pos[a] = ( y << 16 ) + x;
	if( !( system_planets[a] ) ) {
		system_planets[a] = 4 + rand() % 4;
		if( !( rand() & 7 ) )
			system_planets[a] += rand() % 12;
		if( !( rand() & 7 ) )
			system_planets[a] += rand() % 8;

	}
	system_pbase[a] = p;
	p += system_planets[a];
	mapdata[i] = 1;
}


for( a = 0 ; a < MAP_ARTEFACTS ; a++ ) {
	mainL2:
	b = rand() % MAP_SYSTEMS;
	if( system_home[b] )
		goto mainL2;
	artefact_planet[a] = system_pbase[b] + ( rand() % system_planets[b] );
	#if FORKING == 0
	printf("( %d,%d ) ID:%d Holds: %s\n", system_pos[b] & 0xFFFF, system_pos[b] >> 16, artefact_planet[a], artefactName[a] );
	#endif
	syslog(LOG_INFO,  "( %d,%d ) ID:%d Holds: %s\n", system_pos[b] & 0xFFFF, system_pos[b] >> 16, artefact_planet[a], artefactName[a] );
}


// OK, a new headers write.
file = fopen( COREDIRECTORY "/data/map", "wb" );
mapd.sizex = MAP_SIZEX;
mapd.sizey = MAP_SIZEY;
mapd.systems = MAP_SYSTEMS;
mapd.planets = p;
mapd.families = MAP_FAMILIES;
mapd.fmembers = MAP_FAMMEMBERS;
mapd.capacity = MAP_FAMILIES * MAP_FAMMEMBERS;
mapd.artitimer = -1;
mapd.timempire = -1;
fwrite( &mapd, 1, sizeof(dbMainMapDef), file ); 

// New system generation, based on defaults.
p = 0;
for( a = 0 ; a < MAP_SYSTEMS ; a++ ) {
	systemd = dbSystemDefault;
	systemd.position = system_pos[a];
	systemd.indexplanet = p;
	p += system_planets[a];
	systemd.numplanets = system_planets[a];

	if( system_home[a] ) {
		systemd.empire = system_home[a] - 1;
	} else {
		systemd.unexplored = system_planets[a];
	}
	fwrite( &systemd, 1, sizeof(dbMainSystemDef), file );
}
//End system generation

// New planet generation, based on defaults.
for( a = b = c = 0 ; a < p ; a++, b++ ) {
	planetd = dbPlanetDefault;
	if( b >= system_planets[c] ) {
		b -= system_planets[c];
		c++;
	}
	planetd.system = c;
	x = system_pos[c] & 0xFFFF;
	y = system_pos[c] >> 16;
	i = ( y << 20 ) + ( x << 8 ) + b;
	planetd.position = i;
	if( !( system_home[c] ) ) {
		planetd.size = 128 + ( rand() % 192 );
		if( !( rand() & 7 ) )
			planetd.size += rand() & 255;
		if( !( rand() & 31 ) )
			planetd.size += rand() & 511;
	}
	if( system_home[c] )
		planetd.flags = CMD_PLANET_FLAGS_HOME;

	e = i = 0;
	if( !( system_home[c] ) ) {
		distmax = (float)0xFFFF;
		for( d = 0 ; d < MAP_RESOURCES ; d++ ) {
			x2 = x - map_resources_posx[d];
			y2 = y - map_resources_posy[d];
			dist = sqrt( x2*x2 + y2*y2 );
			if( dist < distmax ) {
				distmax = dist;
				e = map_resources_type[d];
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
	for( d = 0 ; d < MAP_ARTEFACTS ; d++ ) {
		if( a != artefact_planet[d] )
			continue;
			i = d + 1;
			break;
		}
	planetd.special[2] = i;
	fwrite( &planetd, 1, sizeof(dbMainPlanetDef), file );

}
//End planet generation


// New families generation, based on defaults.
for( a = 0 ; a < MAP_FAMILIES ; a++ ) {
	empired = dbEmpireDefault;
	
	if( ( cmdAdminEmpire == a ) && ( strlen(cmdAdminEmpirePass) ) ) {
		strcpy( empired.name, "Administration");
		#if HASHENCRYPTION == 1
		if( strlen(cmdAdminEmpirePass) )
			sprintf(cmdAdminEmpirePass, "%s", hashencrypt(cmdAdminEmpirePass) );
		#endif
		strcpy( empired.password, cmdAdminEmpirePass );
		#if FORKING == 0
		printf("Empire %d Claimed for Administration.\n", cmdAdminEmpire);
		#endif
		syslog(LOG_INFO, "Empire %d Claimed for Administration.\n", cmdAdminEmpire);
	}

	empired.homeid = empire_system[a];
	empired.homepos = system_pos[ empire_system[a] ];
	fwrite( &empired, 1, sizeof(dbMainEmpireDef), file );
// <<WORKNEEDED>>
	sprintf( fname, COREDIRECTORY "/data/fam%dnews", a );
	file2 = fopen( fname, "wb" );
	j = 0;
	fwrite( &j, 1, sizeof(long long int), file2 );
	j = -1;
	fwrite( &j, 1, sizeof(long long int), file2 );
	fwrite( &j, 1, sizeof(long long int), file2 );
	j = 0;
	fwrite( &j, 1, sizeof(long long int), file2 );
	fwrite( &j, 1, sizeof(long long int), file2 );
	fclose( file2 );
}
fclose( file );
//End family generation

for( y = 0 ; y < MAP_SIZEX ; y++ ) { 
	for( x = 0 ; x < MAP_SIZEY ; x++ )  {
		if ( mapdata[(y*MAP_SIZEY)+x] )
			pixels[(y*MAP_SIZEY)+x] = 255;
	}  
}

/*
mapgen.width = MAP_SIZEX *3;
mapgen.height = MAP_SIZEY *3;
bigpixies = malloc( mapgen.height * mapgen.width );

for( y = 0 ; y < mapgen.height ; y++ ) { 
	for( x = 0 ; x < mapgen.width ; x++ )  {
		if ( pixels[((y*mapgen.width)+x)] )
			bigpixies[(y*mapgen.width)+x] = pixels[((y*mapgen.width)+x)];

		if ( pixels[(((y/3)*(mapgen.width/3))+(x/3))] )
			bigpixies[(y*mapgen.width)+x] = pixels[(((y/3)*(mapgen.width/3))+(x/3))];
		if ( pixels[(((y/3)*(mapgen.width))+(x/3))] )
			bigpixies[(y*mapgen.width)+x] = pixels[(((y/3)*(mapgen.width))+(x/3))];

	}  
}

*/


if(mapgen.width > MAP_SIZEX) {
	mapgen.data = bigpixies;
} else {
	mapgen.data = pixels;
}

sprintf( fname, IOHTTP_FILES_DIRECTORY "/galaxyr%d.png", ROUND_ID );
imgWritePngFile( fname, &mapgen );
free(pixels);
free(bigpixies);


//<<WORKNEEDED>> Such a dirty fix, but well... it works. =/
if(mapgen.width == MAP_SIZEX) {
	sprintf(imgsizer, "convert \"%s\" -resize 300% \"%s\"", fname, fname );
	system(imgsizer);
}

return 1;
}


