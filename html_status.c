

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


void iohttpFunc_status( svConnectionPtr cnt ) {
	int pid;
	FILE *file;
	char fname[256], addstring[32];
	int stutime, ststime, stpriority, ststarttime, stvsize, strss;
	float boottime, runtime, userload, kernelload;
	float loadavg[3];
	float shiftfloat;
	char stringuptime[128];
	struct sysinfo  si;
	struct utsname stustname;
	cpuInfo cpuinfo;

cpuGetInfo( &cpuinfo );


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

shiftfloat=(float)(1<<SI_LOAD_SHIFT);
loadavg[0]=((float)si.loads[0])/shiftfloat;
loadavg[1]=((float)si.loads[1])/shiftfloat;
loadavg[2]=((float)si.loads[2])/shiftfloat;
svSendString( cnt, "<b>Server Processor Info</b><br>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
if( (loadavg[0] > 0) || (loadavg[1] > 0) || (loadavg[2] > 0) )
	svSendPrintf( cnt, "<tr><td>Load Averages</td><td>&nbsp;:&nbsp;</td><td>%f (1 min) - %f (5 mins) - %f (15 mins)</td></tr>",loadavg[0],loadavg[1],loadavg[2]);
svSendPrintf( cnt, "<tr><td>CPU Type</td><td>&nbsp;:&nbsp;</td><td>%s %s</td></tr>", cpuinfo.identifier, cpuArchName[cpuinfo.arch] );
svSendPrintf( cnt, "<tr><td>%s</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", cpuinfo.vendorstring, cpuClassName[cpuinfo.class] );
if( cpuinfo.cachesizeL3 > 0 )
	svSendPrintf( cnt, "<tr><td>Level 3 Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cachesizeL3 );

if( cpuinfo.cachesizeL2 > 0 )
	svSendPrintf( cnt, "<tr><td>Level 2 Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cachesizeL2 );

if( cpuinfo.cachesizeL1data > 0 )
	svSendPrintf( cnt, "<tr><td>Level 1 Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cachesizeL1data );

if( cpuinfo.cacheunifiedL1 > 0 )
	svSendPrintf( cnt, "<tr><td>Unified Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cacheunifiedL1 );

svSendPrintf( cnt, "<tr><td>Physical Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketphysicalcores );
svSendPrintf( cnt, "<tr><td>Logical Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketlogicalcores );

if( cpuinfo.socketcount ) {
	svSendPrintf( cnt, "<tr><td>Sockets</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketcount );
	svSendPrintf( cnt, "<tr><td>Total Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.totalcorecount );
}
svSendString( cnt, "</table>" );


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
if( (si.bufferram) > 0 )
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


