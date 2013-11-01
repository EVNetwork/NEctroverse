#define IOSINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.


typedef struct
{
  int cores;
  int siblings;
  int family;
  int cache;
  int model;
  float bogomips;
  float speed;
  char vendor_id[32];
  char name[128];
} CPUInfoDef, *CPUInfoPtr;

//extern CPUInfoDef cpuinfo;

void iohttpFuncConvertTime( char *buffer, int eltime );

int linux_cpuinfo( CPUInfoDef *cpuinfos );

void iohttpFunc_status( svConnectionPtr cnt );
