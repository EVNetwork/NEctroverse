

void iohttpFuncConvertTime( char *buffer, int eltime )
{
  int up_days, up_hrs, up_mins, up_secs;
  up_days = eltime/86400;
  up_hrs = (eltime-(up_days*86400))/3600;
  up_mins = (eltime-(up_days*86400)-(up_hrs*3600))/60;
  up_secs = (eltime-(up_days*86400)-(up_hrs*3600)-(up_mins*60));
  if( up_days )
    sprintf( buffer, "%d %s %02d %s %02d %s %02d %s", up_days, ( up_days==1?"day":"days" ), up_hrs, ( up_hrs==1?"hour":"hours" ), up_mins, ( up_mins==1?"minute":"minutes" ), up_secs, ( up_secs==1?"second":"seconds" ) );
  else
    sprintf( buffer, "%d %s %02d %s %02d %s", up_hrs, ( up_hrs==1?"hour":"hours" ), up_mins, ( up_mins==1?"minute":"minutes" ), up_secs, ( up_secs==1?"second":"seconds" ) );
  return;
}

//Now caped at one processor, we don't need to display the same infomation multiple times for the cores/siblings.
int linux_cpuinfo( CPUInfoDef *cpuinfos ) {
	int a;
	FILE *file;
	char temp[1024];
	char* match;
	
file = fopen( "/proc/cpuinfo", "r" );
if( file ) {
	a = fread( temp, 1, sizeof(temp), file );
	if( !(a) ){
		return 0; 
	}
}

match = strstr(temp, "cpu cores");
if( sscanf(match, "cpu cores  :  %d", &cpuinfos->cores) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Core Info...\n");
	syslog(LOG_INFO, "Error getting CPU Core Info...\n");
	return 0;
}

match = strstr(temp, "siblings");
if( sscanf(match, "siblings  :  %d", &cpuinfos->siblings) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Siblings Info...\n");
	syslog(LOG_INFO, "Error getting CPU Siblings Info...\n");
	return 0;
}

match = strstr(temp, "cache size");
if( sscanf(match, "cache size  :  %d", &cpuinfos->cache) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Cache Info...\n");
	syslog(LOG_INFO, "Error getting CPU Cache Info...\n");
	return 0;
}

match = strstr(temp, "model");
if( sscanf(match, "model  :  %d", &cpuinfos->model) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Model Info...\n");
	syslog(LOG_INFO, "Error getting CPU Model Info...\n");
	return 0;
}

match = strstr(temp, "bogomips");
if( sscanf(match, "bogomips  :  %f", &cpuinfos->bogomips) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Bogomips Info...\n");
	syslog(LOG_INFO, "Error getting CPU Bogomips Info...\n");
	return 0;
}

match = strstr(temp, "cpu MHz");
if( sscanf(match, "cpu MHz  :  %f", &cpuinfos->speed) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU MHz Info...\n");
	syslog(LOG_INFO, "Error getting CPU Mhz Info...\n");
	return 0;
}

match = strstr(temp, "vendor_id");
if( sscanf(match, "vendor_id  :  %[^\n]", (char *)&cpuinfos->vendor_id) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Vendor Info...\n");
	syslog(LOG_INFO, "Error getting CPU Vendor Info...\n");
	return 0;
}

match = strstr(temp, "model name");
if( sscanf(match, "model name  :  %[^\n]", (char *)&cpuinfos->name) != 1 ) {
	if( options.verbose )
	printf("Error getting CPU Name Info...\n");
	syslog(LOG_INFO, "Error getting CPU Name Info...\n");
	return 0;
}



return 1;
}


void iohttpFunc_status( svConnectionPtr cnt ) {
	int pid;
	FILE *file;
	char fname[256], addstring[32];
	int stutime, ststime, stpriority, ststarttime, stvsize, strss;
	float boottime, runtime, userload, kernelload;
	float fav[3];
	float shiftfloat;
	char stringuptime[128];
	struct sysinfo  si;
	struct utsname stustname;


CPUInfoDef cpuinfo = { -1, -1, -1, -1, -1, -1, -1, "", "" };


iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, FMENU_SERVER );
svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td width=\"7%\">&nbsp;</td>" );

pid = getpid();
sprintf( fname, "/proc/%d/stat", pid );
if( ( file = fopen( fname, "r" ) ) ) {
	fscanf( file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %d %d %*d %*d %d %*d %*u %*u %u %u %u", &stutime, &ststime, &stpriority, &ststarttime, &stvsize, &strss );
	fclose( file );
}

if( sysinfo(&si) != 0 ) {
 	if( options.verbose )
	printf("Failure getting system infomation... Critical failure.");
	syslog(LOG_INFO, "Failure getting system infomation... Critical failure." );
	cleanUp(-1,-1);
	exit(EXIT_FAILURE);
}

iohttpFuncConvertTime((char *)&stringuptime,si.uptime);
runtime = si.uptime - CT_TO_SECS( ( (float)ststarttime ) );

userload = 100.0 * ( CT_TO_SECS( ( (float)stutime ) ) / runtime );
kernelload = 100.0 * ( CT_TO_SECS( ( (float)ststime ) ) / runtime );

svSendString( cnt, "<table width=\"100%\" border=\"0\"><tr><td width=\"50%\" align=\"left\" valign=\"top\">" );
sprintf(addstring, "%s status", sysconfig.servername );
iohttpFunc_boxstart( cnt, addstring);
svSendString( cnt, "<table border=\"0\"><tr><td>" );
svSendPrintf( cnt, "General status : No problems detected<br>" ); // Should we partially keep running through signals?
svSendPrintf( cnt, "Current date : Week %d, year %d<br>", ticks.number % 52, ticks.number / 52 );
svSendPrintf( cnt, "Tick time : %d seconds<br>", sysconfig.ticktime );
if( ticks.status ) {
	svSendPrintf( cnt, "Next tick : %d seconds<br>", (int)( ticks.next - time(0) ) );
} else {
	svSendPrintf( cnt, "Next tick : Time Frozen!<br>" );
}
svSendPrintf( cnt, "Process priority : %d<br><br>", stpriority );

if( linux_cpuinfo( &cpuinfo ) ) {
	shiftfloat=(float)(1<<SI_LOAD_SHIFT);
	fav[0]=((float)si.loads[0])/shiftfloat;
	fav[1]=((float)si.loads[1])/shiftfloat;
	fav[2]=((float)si.loads[2])/shiftfloat;
	svSendPrintf( cnt, "<b>Server Processor%s</b><br>", (cpuinfo.cores > 1) ? "s" : "" );
	svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
	svSendPrintf( cnt, "<tr><td>Load Averages</td><td>&nbsp;:&nbsp;</td><td>%f (1 min) - %f (5 mins) - %f (15 mins)</td></tr>",fav[0],fav[1],fav[2]);
	svSendPrintf( cnt, "<tr><td>%s</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", cpuinfo.vendor_id, cpuinfo.name );
	if( (cpuinfo.siblings > 0) && (cpuinfo.cores > 0) ) {
		svSendString( cnt, "<tr><td>CPU Info</td><td>&nbsp;:&nbsp;</td><td>" );
		svSendPrintf( cnt, "Processor has %d sibling%s with %d core%s</td></tr>", cpuinfo.siblings, ( cpuinfo.siblings > 1) ? "s" : "", cpuinfo.cores, ( cpuinfo.cores > 1) ? "s" : "" );
	}
	svSendPrintf( cnt, "<tr><td>Current Speed</td><td>&nbsp;:&nbsp;</td><td>%.02f MHz</td></tr>",cpuinfo.speed);
	svSendPrintf( cnt, "<tr><td>Bogomips</td><td>&nbsp;:&nbsp;</td><td>%.02f</td></tr>",cpuinfo.bogomips);
	svSendPrintf( cnt, "<tr><td>Cache Size</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>",cpuinfo.cache);
	svSendString( cnt, "</table>" );
}

svSendString( cnt, "</td></tr></table>" );
iohttpFunc_boxend( cnt );
svSendString( cnt, "</td><td width=\"50%\" align=\"left\" valign=\"top\">" );
iohttpFunc_boxstart( cnt, "Server status" );
svSendString( cnt, "<table border=\"0\"><tr><td>" );
svSendString( cnt, "<b>System info</b><br>" );
uname( &stustname );
svSendPrintf( cnt, "Sysname : %s %s<br>", stustname.sysname, stustname.release );
svSendPrintf( cnt, "Release : %s<br>", stustname.version );
svSendPrintf( cnt, "Uptime : %s<br>", stringuptime );

svSendString( cnt, "<br><b>System RAM infomation</b><br>" );
svSendPrintf( cnt, "Total system memory : %ld bytes ( %ld mb )<br>", si.totalram, (si.totalram >> 20) );
svSendPrintf( cnt, "Avalible memory now : %ld bytes ( %ld mb )<br>", si.freeram, (si.freeram >> 20) );
if( (si.totalswap) > 0 )
	svSendPrintf( cnt, "Total Swap: %ld bytes ( %ld mb )<br>Free Swap: %ld bytes ( %ld mb )<br>", si.totalswap, (si.totalswap >> 20), si.freeswap, (si.freeswap >> 20));
svSendPrintf( cnt, "Buffer Usage: %ld bytes ( %ld mb )<br>", si.bufferram, (si.bufferram >> 20));
if( (si.sharedram) > 0 )
svSendPrintf( cnt, "Shared Ram: %ld bytes ( %ld mb )<br>", si.sharedram, (si.sharedram >> 20) );

svSendString( cnt, "<br><b>Game Server usage ( average )</b><br>" );
svSendPrintf( cnt, "Memory used : %d bytes ( %d mb )<br>", stvsize, stvsize >> 20 );
svSendPrintf( cnt, "Resident Size : %d pages<br>", strss );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>Total CPU usage</td><td>&nbsp;:&nbsp;</td><td>%.3f %%</td></tr>", userload + kernelload );
svSendPrintf( cnt, "<tr><td>In kernel mode</td><td>&nbsp;:&nbsp;</td><td>%.3f %%</td></tr>", kernelload ); 
svSendPrintf( cnt, "<tr><td>In user mode</td><td>&nbsp;:&nbsp;</td><td>%.3f %%</td></tr>", userload );
svSendString( cnt, "</table>" );
svSendString( cnt, "</td></tr></table>" );
iohttpFunc_boxend( cnt );


iohttpFunc_endhtml( cnt );

  return;
}


