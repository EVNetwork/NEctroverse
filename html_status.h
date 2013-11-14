#define IOSINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

typedef struct {
	long seconds;
	long minutes;
	long hours;
	long days;
} timeDef, *timePtr;

typedef struct {
	float runtime;
	float userload;
	float kernelload;
	long unsigned int stutime;
	long unsigned int ststime;
	long int stpriority;
	long long unsigned int ststarttime;
	long unsigned int stvsize;
	long int strss;
	int stcmajflt;
	int stmajflt;
	int stcminflt;
	int stminflt;
	float loadavg[0];
} proginfoDef, *proginfoPtr;

char *TimeToString( long eltime );

void iohttpFunc_status( svConnectionPtr cnt );

void iohtmlFunc_status( ReplyDataPtr cnt );

void connverttime_todef( timeDef *statime, long stime );

void getsys_infos( proginfoDef *proginfo );

struct tm *gettime( int t_in, bool gmt );

int timediff( struct tm t_in  );
