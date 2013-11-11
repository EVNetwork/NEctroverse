#ifndef MAPINCLUDES
#define MAPINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct {
	//new map struct
	int sizex;
	int sizey;
	int systems;

	int families;
	int fmembers;

	int border;
	double anglevar;

	int linknum;
	double linkradius;
	int lenghtbase;
	int lenghtvar;
	
	int bonusnum;
	int bonusvar[CMD_BONUS_NUMUSED];
} mapcfgDef, *mapcfgPtr;

extern mapcfgDef mapcfg;

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
} mapstoreDef, *mapstorePtr;

extern mapstoreDef mapstg;


int spawn_map();

#endif
