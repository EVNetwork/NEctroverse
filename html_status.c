

void iohttpFuncConvertTime( char *buffer, int eltime ) {
	int up_days, up_hrs, up_mins, up_secs;
	bool bdays, bhours, bmins;
	char timeline[32];

bdays = bhours = bmins = false;
up_days = eltime / day;
up_hrs = (eltime-(up_days*day))/hour;
up_mins = (eltime-(up_days*day)-(up_hrs*hour))/minute;
up_secs = (eltime-(up_days*day)-(up_hrs*hour)-(up_mins*minute));

strcpy(buffer,"");
if( up_days ) {
	sprintf( timeline, "%d %s", up_days, ( ( up_days == 1 ) ? "day" : "days" ) );
	strcat(buffer, timeline);
	bdays = true;
} 

if ( up_hrs ) {
	if( bdays )
		strcat(buffer, " ");
	sprintf( timeline, "%d %s", up_hrs, ( ( up_hrs == 1 ) ? "hour" :"hours" ) );
	strcat(buffer, timeline);
	bhours = true;
} 

if ( up_mins ) {
	if( ( bdays ) || ( bhours ) )
		strcat(buffer, " ");
	sprintf( timeline, "%02d %s", up_mins, ( ( up_mins == 1 ) ? "minute" : "minutes" ) );
	strcat(buffer, timeline);
	bmins = true;
} 

if ( up_secs ) {
	if( ( bdays ) || ( bhours ) || ( bmins ) )
		strcat(buffer, " ");
	sprintf( timeline, "%02d %s", up_secs, ( ( up_secs == 1 ) ? "second" : "seconds" ) );
	strcat(buffer, timeline);
}

if( !( strlen( buffer ) ) ) {
	strcpy(buffer, "Never, or bad time input!" );
}


return;
}


void iohttpFunc_status( svConnectionPtr cnt ) {
	int pid;
	FILE *file;
	char fname[256], addstring[32];
	int stutime, ststime, stpriority, ststarttime, stvsize, strss, stcmajflt, stmajflt, stcminflt, stminflt;
	float runtime, userload, kernelload;
	float loadavg[3];
	float shiftfloat;
	char stringuptime[128];
	struct sysinfo sysinfod;
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
	if( fscanf( file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %u %u %u %u %d %d %*d %*d %d %*d %*u %*u %u %u %u", &stcminflt, &stminflt, &stmajflt, &stcmajflt, &stutime, &ststime, &stpriority, &ststarttime, &stvsize, &strss ) != 10 ) {
		 if( options.verbose )
	printf("Error getting process stat Info...\n");
	syslog(LOG_ERR, "Error getting process stat Info...\n");
	}
	fclose( file );
}

if( sysinfo(&sysinfod) != 0 ) {
 	if( options.verbose )
		printf("Failure getting system infomation... Critical failure.\n");
	syslog(LOG_ERR, "Failure getting system infomation... Critical failure." );
	cleanUp(-1,-1);
	exit(EXIT_FAILURE);
}

runtime = sysinfod.uptime - CT_TO_SECS( ( (float)ststarttime ) );

userload = 100.0 * ( CT_TO_SECS( ( (float)stutime ) ) / runtime );
kernelload = 100.0 * ( CT_TO_SECS( ( (float)ststime ) ) / runtime );

svSendString( cnt, "<table width=\"100%\" border=\"0\"><tr><td width=\"50%\" align=\"left\" valign=\"top\">" );
sprintf(addstring, "%s status", sysconfig.servername );
iohttpFunc_boxstart( cnt, addstring);
svSendString( cnt, "<table border=\"0\"><tr><td>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>General status</td><td>&nbsp;:&nbsp;</td><td>No problems detected</td></tr>" ); // Should we partially keep running through signals?
iohttpFuncConvertTime((char *)&stringuptime,runtime);
svSendPrintf( cnt, "<tr><td>Game Uptime</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", stringuptime );
svSendPrintf( cnt, "<tr><td>Current date</td><td>&nbsp;:&nbsp;</td><td>Week <span id=\"sstatweeks\">%d</span>, year <span id=\"sstatyears\">%d</span></td></tr>", ticks.number % 52, ticks.number / 52 );
svSendPrintf( cnt, "<tr><td>Tick time</td><td>&nbsp;:&nbsp;</td><td>%d seconds</td></tr>", sysconfig.ticktime );

if( ticks.status ) {
	svSendPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td id=\"sstatsTime\">%d seconds</td></tr>", (int)( ticks.next - time(0) ) );
} else {
	svSendPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td>Time Frozen!</td></tr>" );
}
svSendPrintf( cnt, "<tr><td>Process priority</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", stpriority );
svSendString( cnt, "</table><br>" );

svSendString( cnt, "<b>Game Server usage ( average )</b><br>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>Memory used</td><td>&nbsp;:&nbsp;</td><td>%d bytes ( %d mb )</td></tr>", stvsize, stvsize >> 20 );
svSendPrintf( cnt, "<tr><td>Resident Size</td><td>&nbsp;:&nbsp;</td><td>%d pages</td></tr>", strss );
svSendPrintf( cnt, "<tr><td>Total CPU usage</td><td>&nbsp;:&nbsp;</td><td>%.3f %%</td></tr>", userload + kernelload );
svSendPrintf( cnt, "<tr><td>In kernel mode</td><td>&nbsp;:&nbsp;</td><td>%.3f %%</td></tr>", kernelload ); 
svSendPrintf( cnt, "<tr><td>In user mode</td><td>&nbsp;:&nbsp;</td><td>%.3f %%</td></tr>", userload );
svSendString( cnt, "</table>" );

svSendString( cnt, "</td></tr></table>" );
iohttpFunc_boxend( cnt );
svSendString( cnt, "</td><td width=\"50%\" align=\"left\" valign=\"top\">" );
iohttpFunc_boxstart( cnt, "Server Info" );
svSendString( cnt, "<table border=\"0\"><tr><td>" );
svSendString( cnt, "<b>System OS</b><br>" );
uname( &stustname );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>Sysname</td><td>&nbsp;:&nbsp;</td><td>%s %s</td></tr>", stustname.sysname, stustname.release );
svSendPrintf( cnt, "<tr><td>Release</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", stustname.version );
iohttpFuncConvertTime((char *)&stringuptime,sysinfod.uptime);
svSendPrintf( cnt, "<tr><td>Uptime</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", stringuptime );
svSendString( cnt, "</table><br>" );

shiftfloat=(float)(1<<SI_LOAD_SHIFT);
loadavg[0]=((float)sysinfod.loads[0])/shiftfloat;
loadavg[1]=((float)sysinfod.loads[1])/shiftfloat;
loadavg[2]=((float)sysinfod.loads[2])/shiftfloat;
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
svSendString( cnt, "</table><br>" );
svSendString( cnt, "<b>System RAM infomation</b><br>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>Total memory</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %ld mb )</td></tr>", sysinfod.totalram, (sysinfod.totalram >> 20) );
svSendPrintf( cnt, "<tr><td>Avalible memory</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %ld mb )</td></tr>", sysinfod.freeram, (sysinfod.freeram >> 20) );
if( (sysinfod.totalswap) > 0 ) {
	svSendPrintf( cnt, "<tr><td>Total Swap</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %ld mb )</td></tr>", sysinfod.totalswap, (sysinfod.totalswap >> 20) );
	svSendPrintf( cnt, "<tr><td>Free Swap</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %ld mb )</td></tr>", sysinfod.freeswap, (sysinfod.freeswap >> 20) );
}
if( (sysinfod.bufferram) > 0 )
	svSendPrintf( cnt, "<tr><td>Buffer Usage</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %ld mb )</td></tr>", sysinfod.bufferram, (sysinfod.bufferram >> 20));
if( (sysinfod.sharedram) > 0 )
	svSendPrintf( cnt, "<tr><td>Shared Ram</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %ld mb )</td></tr>", sysinfod.sharedram, (sysinfod.sharedram >> 20) );
svSendString( cnt, "</table>" );
svSendString( cnt, "</td></tr></table>" );
iohttpFunc_boxend( cnt );


iohttpFunc_endhtml( cnt );

  return;
}


