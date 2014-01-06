#ifndef GLOBALINCLUDED
#include "config/global.h"
#endif


void mapCalcFactors( MapStoragePtr mapstore ) {
	int a, b, c, x, y, index;
	float fx, fy, dist, mindist;
	float fpos[2];
	float angle, anglevar;
	float fdir[2];
	float pts[ARRAY_MAX][2];
	int ptsnum;

ptsnum = 0;

for( a = 0 ; a < mapcfg.linknum ; a++ ) {
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
			if( dist >= mapcfg.linkradius )
			continue;
			mindist += mapcfg.linkradius - dist;
		}
		mapstore->factor[index] += (int)floor( mindist );
	}
}




return;
}


int spawn_map() {
	int a, b, c, d, e, i, p, x, y, x2, y2;
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

mapstore = calloc( 1, sizeof(MapStorageDef) );

mapstore->factor = calloc( mapcfg.sizex*mapcfg.sizey, sizeof(int) );
mapstore->data = calloc( mapcfg.sizex*mapcfg.sizey, sizeof(int) );
mapstore->posx = calloc( mapcfg.bonusnum, sizeof(int) );
mapstore->posy = calloc( mapcfg.bonusnum, sizeof(int) );
mapstore->type = calloc( mapcfg.bonusnum, sizeof(int) );
mapstore->pos = calloc( mapcfg.systems, sizeof(int) );
mapstore->planets = calloc( mapcfg.systems, sizeof(int) );
mapstore->pbase = calloc( mapcfg.systems, sizeof(int) );
mapstore->home = calloc( mapcfg.systems, sizeof(int) );
mapstore->system = calloc( mapcfg.families, sizeof(int) );
mapstore->arti = calloc( ARTEFACT_NUMUSED, sizeof(int) );

mapCalcFactors( mapstore );


sprintf( fname, "%s/data", sysconfig.directory );
dirstructurecheck(fname);

RANDOMIZE_SEED;

mapimage.width = mapcfg.sizex;
mapimage.height = mapcfg.sizey;
mapimage.format = IMG_IMAGE_FORMAT_GRAYSCALE;
mapimage.bytesperpixel = 1;
mapimage.bytesperline = mapimage.width;


for( a = 0 ; a < mapcfg.families ; a++ ) {
	mainL1:
	mapstore->system[a] = rand() % mapcfg.systems;
	if( mapstore->home[ mapstore->system[a] ] )
		goto mainL1;
	mapstore->home[ mapstore->system[a] ] = a+1;
	mapstore->planets[ mapstore->system[a] ] = mapcfg.fmembers;
}

for( a = b = c = 0 ; a < mapcfg.bonusnum ; a++, b++ ) {
	mapstore->posx[a] = rand() % mapcfg.sizex;
	mapstore->posy[a] = rand() % mapcfg.sizey;
	if( b >= mapcfg.bonusvar[c] ) {
		b -= mapcfg.bonusvar[c];
		c++;
	}
	mapstore->type[a] = c;
}

p = 0;
for( a = 0 ; a < mapcfg.systems ; a++ ) {
	mainL0:
	for( ; ; ) {
		x = rand() % mapcfg.sizex;
		y = rand() % mapcfg.sizey;
		i = ( y * mapcfg.sizex ) + x;
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
	b = rand() % mapcfg.systems;
	if( mapstore->home[b] )
		goto mainL2;
	mapstore->arti[a] = mapstore->pbase[b] + ( rand() % mapstore->planets[b] );
	sprintf(logString,  "( %d,%d ) ID:%d Holds: %s", mapstore->pos[b] & 0xFFFF, mapstore->pos[b] >> 16, mapstore->arti[a], artefactName[a] );
	info( logString );
}

if( NULL == ( mapbase = calloc( 1, sizeof(dbMainMapDef) ) ) ) {
	critical( "Malloc Failed" );
	return 0;
}
// OK, a new headers write.
sprintf( fname, "%s/data/map", sysconfig.directory );
file = fopen( fname, "wb" );
mapbase->sizex = mapcfg.sizex;
mapbase->sizey = mapcfg.sizey;
mapbase->systems = mapcfg.systems;
mapbase->planets = p;
mapbase->families = mapcfg.families;
mapbase->fmembers = mapcfg.fmembers;
mapbase->capacity = mapcfg.families * mapcfg.fmembers;
mapbase->artitimer = -1;
mapbase->timempire = -1;
file_w( mapbase, 1, sizeof(dbMainMapDef), file ); 
free( mapbase );
// New system generation, based on defaults.
p = 0;
for( a = 0 ; a < mapcfg.systems ; a++ ) {
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
		for( d = 0 ; d < mapcfg.bonusnum ; d++ ) {
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
for( a = 0 ; a < mapcfg.families ; a++ ) {
	memset( &empired, 0, sizeof(dbMainEmpireDef) );
	memset( empired.player, -1, mapcfg.fmembers*sizeof(int) );
	memset( empired.vote, -1, mapcfg.fmembers*sizeof(int) );
	empired.leader = empired.rank = -1;
	sprintf(empired.message[0],"<i>Welcome to Empire #%d!<i>",a);
	if( ( admincfg.empire == a ) ) {
		strcpy( empired.name, admincfg.ename);
		strcpy( empired.password, hashencrypt(admincfg.epassword) );
		sprintf(logString, "Empire %d Claimed for Administration.", a);
		info( logString );
	}
	empired.homeid = mapstore->system[a];
	empired.homepos = mapstore->pos[ mapstore->system[a] ];
	file_w( &empired, 1, sizeof(dbMainEmpireDef), file );
// FIXME: Well duh... we got all the other stuff nice and flexible... what about this!
	sprintf( fname, "%s/data/fam%dnews", sysconfig.directory, a );
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
//End family generation

//Arightyz, we've got all the data... now lets put it to some use and spalt a image out.
pixels = calloc( mapcfg.sizex*mapcfg.sizey, sizeof(int) );

for( y = 0 ; y < mapcfg.sizey ; y++ ) { 
	for( x = 0 ; x < mapcfg.sizex ; x++ )  {
		pixels[(y*mapcfg.sizex)+x] = (mapstore->factor[(y*mapcfg.sizex)+x] >> 3); //here the milk gets set
		if( mapstore->data[(y*mapcfg.sizex)+x] )
			pixels[(y*mapcfg.sizex)+x] = mapstore->data[(y*mapcfg.sizex)+x]; //here the dots get pointed
	}  
}


bigpixies = NULL;
/*
mapimage.width = MAP_DEFINE_SIZEX *3;
mapimage.height = MAP_DEFINE_SIZEY *3;
bigpixies = calloc( mapimage.height*mapimage.width, sizeof(int) );

for( y = 0 ; y < mapimage.height ; y++ ) { 
	for( x = 0 ; x < mapimage.width ; x++ )  {
		if ( pixels[((y*mapimage.width)+x)] )
			bigpixies[(y*mapimage.width)+x] = pixels[((y*mapimage.width)+x)];

		if ( pixels[(((y/3)*(mapimage.width/3))+(x/3))] )
			bigpixies[(y*mapimage.width)+x] = pixels[(((y/3)*(mapimage.width/3))+(x/3))];
		if ( pixels[(((y/3)*(mapimage.width))+(x/3))] )
			bigpixies[(y*mapimage.width)+x] = pixels[(((y/3)*(mapimage.width))+(x/3))];

	}  
}

*/


if( mapimage.width > mapcfg.sizex ) {
	mapimage.data = bigpixies;
	free( pixels );
} else {
	mapimage.data = pixels;
}

sprintf( fname, "%s/galaxies", sysconfig.httpimages );
dirstructurecheck(fname);

sprintf( fname, "%s/galaxies/galaxyr%d.png", sysconfig.httpimages, sysconfig.round );
//imgConvertGrayscale(&mapimage,IMG_IMAGE_FORMAT_RGB24);
imgWritePngFile( fname, &mapimage );
imgFree( &mapimage );



//FIXME: Such a dirty fix, but well... it works. =/
if( mapimage.width == mapcfg.sizex ) {
	sprintf(imgsizer, "convert \"%s\" -resize 300%% \"%s\"", fname, fname );
	if( system(imgsizer) ) {
		error( "unable to resize map" );
	}

}


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


return 1;
}


