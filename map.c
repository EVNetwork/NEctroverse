#ifndef GLOBALINCLUDED
#include "global.h"
#endif

#include "imgpng.c"


void mapCalcFactors(mapstoreDef *mapstore) {
	int a, b, c, x, y, index;
	float fx, fy, dist, mindist;
	float fpos[2];
	float angle, anglevar;
	float fdir[2];
	float pts[65536][2];
	int ptsnum;

ptsnum = 0;

for( a = 0 ; a < mapcfg.num ; a++ ) {
	fpos[0] = (float)( mapcfg.border + ( rand() % ( mapcfg.sizex-2*mapcfg.border ) ) );
	fpos[1] = (float)( mapcfg.border + ( rand() % ( mapcfg.sizey-2*mapcfg.border ) ) );
	angle = rand() % 360;
	anglevar = (float)( ( rand() & 0xFFF ) - 0x800 ) / mapcfg.anglevar;

	c = mapcfg.lenghtbase + ( rand() % mapcfg.lenghtvar );
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

for( y = index = 0 ; y < mapcfg.sizey ; y++ ) {
	for( x = 0 ; x < mapcfg.sizey ; x++, index++ ) {
		mindist = 0;
		for( a = 0 ; a < ptsnum ; a++ ) {
			fx = (float)x - pts[a][0];
			fy = (float)y - pts[a][1];
			dist = sqrt( fx*fx + fy*fy );
			if( dist >= mapcfg.radius )
			continue;
			mindist += mapcfg.radius - dist;
		}
		mapstore->factor[index] += (int)floor( mindist );
	}
}




return;
}


int spawn_map() {
	int a, b, c, d, e, i, p, x, y, x2, y2;
	long long int j;
	float dist, distmax;
	char fname[256];
	char imgsizer[2048];
	uint8_t *pixels, *bigpixies; 
	FILE *file;
	FILE *file2;
	imgImage mapgen;
	mapstoreDef mapstore;
	dbMainMapDef mapd;
	dbMainSystemDef systemd;
	dbMainPlanetDef planetd;
	dbMainEmpireDef empired;

memset( &mapstore, 0, sizeof(mapstoreDef) ); // ahaha... holy cow this is gonna be hectic =P

mapstore.factor = malloc( sizeof(int)*mapcfg.sizex*mapcfg.sizey );
memset( mapstore.factor, 0, sizeof(int)*mapcfg.sizex*mapcfg.sizey );
mapstore.data = malloc( sizeof(int)*mapcfg.sizex*mapcfg.sizey );
memset( mapstore.data, 0, sizeof(int)*mapcfg.sizex*mapcfg.sizey );

mapstore.posx = malloc( sizeof(int)*mapcfg.bonusnum );
memset( mapstore.posx, 0, sizeof(int)*mapcfg.bonusnum );
mapstore.posy = malloc( sizeof(int)*mapcfg.bonusnum );
memset( mapstore.posy, 0, sizeof(int)*mapcfg.bonusnum );
mapstore.type = malloc( sizeof(int)*mapcfg.bonusnum );
memset( mapstore.type, 0, sizeof(int)*mapcfg.bonusnum );

mapstore.pos = malloc( sizeof(int)*mapcfg.systems );
memset( mapstore.pos, 0, sizeof(int)*mapcfg.systems );
mapstore.planets = malloc( sizeof(int)*mapcfg.systems );
memset( mapstore.planets, 0, sizeof(int)*mapcfg.systems );
mapstore.pbase = malloc( sizeof(int)*mapcfg.systems );
memset( mapstore.pbase, 0, sizeof(int)*mapcfg.systems );
mapstore.home = malloc( sizeof(int)*mapcfg.systems );
memset( mapstore.home, 0, sizeof(int)*mapcfg.systems );
mapstore.system = malloc( sizeof(int)*mapcfg.families );
memset( mapstore.system, 0, sizeof(int)*mapcfg.families );

mapstore.arti = malloc( sizeof(int)*ARTEFACT_NUMUSED );
memset( mapstore.arti, 0, sizeof(int)*ARTEFACT_NUMUSED );


memset( &mapd, 0, sizeof(dbMainMapDef) );
memset( &systemd, 0, sizeof(dbMainSystemDef) );
memset( &planetd, 0, sizeof(dbMainPlanetDef) );
memset( &empired, 0, sizeof(dbMainEmpireDef) );

sprintf( fname, "%s/data", sysconfig.directory );
dirstructurecheck(fname);

srand( time( 0 ) );

mapgen.width = mapcfg.sizex;
mapgen.height = mapcfg.sizey;
mapgen.format = IMG_IMAGE_FORMAT_GRAYSCALE;
mapgen.bytesperpixel = 1;
mapgen.bytesperline = mapgen.width;

mapCalcFactors(&mapstore);

for( a = 0 ; a < mapcfg.families ; a++ ) {
	mainL1:
	mapstore.system[a] = rand() % mapcfg.systems;
	if( mapstore.home[ mapstore.system[a] ] )
		goto mainL1;
	mapstore.home[ mapstore.system[a] ] = a+1;
	mapstore.planets[ mapstore.system[a] ] = mapcfg.fmembers;
}

for( a = b = c = 0 ; a < mapcfg.bonusnum ; a++, b++ ) {
	mapstore.posx[a] = rand() % mapcfg.sizex;
	mapstore.posy[a] = rand() % mapcfg.sizey;
	if( b >= mapcfg.bonusvar[c] ) {
		b -= mapcfg.bonusvar[c];
		c++;
	}
	mapstore.type[a] = c;
}

p = 0;
for( a = 0 ; a < mapcfg.systems ; a++ ) {
	mainL0:
	for( ; ; ) {
		x = rand() % mapcfg.sizex;
		y = rand() % mapcfg.sizey;
		i = ( y * mapcfg.sizex ) + x;
		if( ( rand() & 0xFF ) >= mapstore.factor[i] )
			continue;
		break;
	}
	if( mapstore.data[i] )
		goto mainL0;
	mapstore.pos[a] = ( y << 16 ) + x;
	if( !( mapstore.planets[a] ) ) {
		mapstore.planets[a] = 4 + rand() % 4;
		if( !( rand() & 7 ) )
			mapstore.planets[a] += rand() % 12;
		if( !( rand() & 7 ) )
			mapstore.planets[a] += rand() % 8;

	}
	mapstore.pbase[a] = p;
	p += mapstore.planets[a];
	mapstore.data[i] = 0xFF;
}


for( a = 0 ; a < ARTEFACT_NUMUSED ; a++ ) {
	mainL2:
	b = rand() % mapcfg.systems;
	if( mapstore.home[b] )
		goto mainL2;
	mapstore.arti[a] = mapstore.pbase[b] + ( rand() % mapstore.planets[b] );
	if( options.verbose )
	printf("( %d,%d ) ID:%d Holds: %s\n", mapstore.pos[b] & 0xFFFF, mapstore.pos[b] >> 16, mapstore.arti[a], artefactName[a] );
	syslog(LOG_INFO,  "( %d,%d ) ID:%d Holds: %s\n", mapstore.pos[b] & 0xFFFF, mapstore.pos[b] >> 16, mapstore.arti[a], artefactName[a] );
}


// OK, a new headers write.
sprintf( fname, "%s/data/map", sysconfig.directory );
file = fopen( fname, "wb" );
mapd.sizex = mapcfg.sizex;
mapd.sizey = mapcfg.sizey;
mapd.systems = mapcfg.systems;
mapd.planets = p;
mapd.families = mapcfg.families;
mapd.fmembers = mapcfg.fmembers;
mapd.capacity = mapcfg.families * mapcfg.fmembers;
mapd.artitimer = -1;
mapd.timempire = -1;
fwrite( &mapd, 1, sizeof(dbMainMapDef), file ); 

// New system generation, based on defaults.
p = 0;
for( a = 0 ; a < mapcfg.systems ; a++ ) {
	systemd = dbSystemDefault;
	systemd.position = mapstore.pos[a];
	systemd.indexplanet = p;
	p += mapstore.planets[a];
	systemd.numplanets = mapstore.planets[a];

	if( mapstore.home[a] ) {
		systemd.empire = mapstore.home[a] - 1;
	} else {
		systemd.unexplored = mapstore.planets[a];
	}
	fwrite( &systemd, 1, sizeof(dbMainSystemDef), file );
}
//End system generation

// New planet generation, based on defaults.
for( a = b = c = 0 ; a < p ; a++, b++ ) {
	dist = 0;
	planetd = dbPlanetDefault;
	if( b >= mapstore.planets[c] ) {
		b -= mapstore.planets[c];
		c++;
	}
	planetd.system = c;
	x = mapstore.pos[c] & 0xFFFF;
	y = mapstore.pos[c] >> 16;
	i = ( y << 20 ) + ( x << 8 ) + b;
	planetd.position = i;
	if( !( mapstore.home[c] ) ) {
		planetd.size = 128 + ( rand() % 192 );
		if( !( rand() & 7 ) )
			planetd.size += rand() & 255;
		if( !( rand() & 31 ) )
			planetd.size += rand() & 511;
	}
	if( mapstore.home[c] )
		planetd.flags = CMD_PLANET_FLAGS_HOME;

	e = i = 0;
	if( !( mapstore.home[c] ) ) {
		distmax = (float)0xFFFF;
		for( d = 0 ; d < mapcfg.bonusnum ; d++ ) {
			x2 = x - mapstore.posx[d];
			y2 = y - mapstore.posy[d];
			dist = sqrt( x2*x2 + y2*y2 );
			if( dist < distmax ) {
				distmax = dist;
				e = mapstore.type[d];
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
		if( a != mapstore.arti[d] )
			continue;
			i = d + 1;
			break;
		}
	planetd.special[2] = i;
	fwrite( &planetd, 1, sizeof(dbMainPlanetDef), file );

}
//End planet generation


// New families generation, based on defaults.
for( a = 0 ; a < mapcfg.families ; a++ ) {
	empired = dbEmpireDefault;
	if( ( admincfg.empire_number == a ) && ( strlen(admincfg.empire_password) ) ) {
		if( strlen(admincfg.empire_name) )
		strcpy( empired.name, admincfg.empire_name);
		else
		strcpy( empired.name, "Administration");
		if( strlen(admincfg.empire_password) )
			strcpy(empired.password, hashencrypt(admincfg.empire_password) );
		if( options.verbose )
		printf("Empire %d Claimed for Administration.\n", a);
		syslog(LOG_INFO, "Empire %d Claimed for Administration.\n", a);
	}
	empired.homeid = mapstore.system[a];
	empired.homepos = mapstore.pos[ mapstore.system[a] ];
	fwrite( &empired, 1, sizeof(dbMainEmpireDef), file );
// <<WORKNEEDED>>
	sprintf( fname, "%s/data/fam%dnews", sysconfig.directory, a );
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

//Arightyz, we've got all the data... now lets put it to some use and spalt a image out.
pixels = malloc( sizeof(int)*mapcfg.sizex*mapcfg.sizey );
memset( pixels, 0, sizeof(int)*mapcfg.sizex*mapcfg.sizey );

for( y = 0 ; y < mapcfg.sizey ; y++ ) { 
	for( x = 0 ; x < mapcfg.sizex ; x++ )  {
		pixels[(y*mapcfg.sizex)+x] = (mapstore.factor[(y*mapcfg.sizex)+x] >> 3); //here the milk gets set
		if( mapstore.data[(y*mapcfg.sizex)+x] )
			pixels[(y*mapcfg.sizex)+x] = mapstore.data[(y*mapcfg.sizex)+x]; //here the dots get pointed
	}  
}


/*
mapgen.width = MAP_DEFINE_SIZEX *3;
mapgen.height = MAP_DEFINE_SIZEY *3;
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


if(mapgen.width > mapcfg.sizex) {
	mapgen.data = bigpixies;
	free(bigpixies);
} else {
	mapgen.data = pixels;
}

sprintf( fname, "%s/galaxyr%d.png", sysconfig.httpimages, sysconfig.round );
//imgConvertGrayscale(&mapgen,IMG_IMAGE_FORMAT_RGB24);
imgWritePngFile( fname, &mapgen );
free(pixels);



//<<WORKNEEDED>> Such a dirty fix, but well... it works. =/
if(mapgen.width == mapcfg.sizex) {
	sprintf(imgsizer, "convert \"%s\" -resize 300% \"%s\"", fname, fname );
	if( system(imgsizer) ) {
		if( options.verbose )
			printf( "Map Error: unable to convert map size\n" );
		syslog(LOG_ERR, "Map Error: unable to convert map size\n" );
	}

}


//Ahh yes, I'll free all those malloc's back up now... better safe eh.
free(mapstore.data);
free(mapstore.factor);

free(mapstore.posx);
free(mapstore.posy);
free(mapstore.type);

free(mapstore.pos);
free(mapstore.planets);
free(mapstore.pbase);
free(mapstore.home);
free(mapstore.system);

free(mapstore.arti);


return 1;
}


