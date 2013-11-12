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

char *TimeToString( int eltime );

void iohttpFunc_status( svConnectionPtr cnt );

void iohtmlFunc_status( ReplyDataPtr cnt );

void connverttime_todef( timeDef *statime, int stime );

void getsys_infos( proginfoDef *proginfo, struct sysinfo sinfo );

struct tm *gettime( int t_in, bool gmt );

int timediff( struct tm t_in  );
