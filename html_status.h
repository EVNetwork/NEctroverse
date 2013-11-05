#define IOSINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

typedef struct {
	int seconds;
	int minutes;
	int hours;
	int days;
} timeDef, *timePtr;

typedef struct {
	float runtime;
	float userload;
	float kernelload;
	int stutime;
	int ststime;
	int stpriority;
	int ststarttime;
	int stvsize;
	int strss;
	int stcmajflt;
	int stmajflt;
	int stcminflt;
	int stminflt;
	float loadavg[0];
} proginfoDef, *proginfoPtr;

void iohttpFuncConvertTime( char *buffer, int eltime );

void iohttpFunc_status( svConnectionPtr cnt );


void connverttime_todef( timeDef *statime, int stime );

void getsys_infos( proginfoDef *proginfo, struct sysinfo sinfo );
