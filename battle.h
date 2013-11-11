#ifndef BATTLEINCLUDES
#define BATTLEINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.


float battlePortalCalc( int tx, int ty, int *portals, int num, int research );

float battlePortalCoverage( int id, int tx, int ty, int research );

int battleReadinessLoss( dbUserMainPtr maind, dbUserMainPtr main2d );

void battlePhaseUpdate( int *unit, int *results );

int battle( int id, int fltid, int *results );


#endif
