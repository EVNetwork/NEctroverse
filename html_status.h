#define IOSINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

void iohttpFuncConvertTime( char *buffer, int eltime );

int linux_get_proc_uptime( char *buffer, float *uptime );

int linux_get_proc_loadavg( char *buffer );

int linux_cpuinfo( char *buffer );

void iohttpFunc_status( svConnectionPtr cnt );
