#ifndef ARTEFACT_H
#define ARTEFACT_H
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.
//Artifact ENUMS
enum
{
ARTEFACT_CITI,
ARTEFACT_GEM,
ARTEFACT_CRAZ,
ARTEFACT_CRUI,

ARTEFACT_FAST,
ARTEFACT_LARG,
ARTEFACT_RICH,
ARTEFACT_PORT,

ARTEFACT_ETHER,

ARTEFACT_NUMUSED
};


#define ARTEFACT_1_BIT 1
#define ARTEFACT_2_BIT 2
#define ARTEFACT_4_BIT 4
#define ARTEFACT_8_BIT 8

#define ARTEFACT_16_BIT 16
#define ARTEFACT_32_BIT 32
#define ARTEFACT_64_BIT 64
#define ARTEFACT_128_BIT 128

#define ARTEFACT_ETHER_BIT 256

//#define ARTEFACT_TERO_BIT 512  //When 10 arti

typedef struct ArtefactArray
{
  bool active;
  int id;
  int side_effect;
  int effect_flags;
  int effect_var[32];
  float variable[64];
  char name[128];
  char image[64];
  char description[1024];
  struct ArtefactArray *next;
} dbArtefactDef, *dbArtefactPtr;

extern dbArtefactPtr ArtefactList;
extern dbArtefactPtr *ArtefactTable;
extern int ArtefactNum;
extern int ArtefactTotal;

int artefactPrecense( dbMainPlanetPtr planetd );

int LoadArtefacts( );
void UnLoadArtefacts( );

#endif
