

void converttime_todef( timeDef *statime, int stime ) {

statime->days = fmax( 0.0, ( stime / day ) );
statime->hours = (stime-(statime->days*day))/hour;
statime->minutes = (stime-(statime->days*day)-(statime->hours*hour))/minute;
statime->seconds = (stime-(statime->days*day)-(statime->hours*hour)-(statime->minutes*minute));

return;
}

void iohttpFuncConvertTime( char *buffer, int eltime ) {
	bool bdays, bhours, bmins;
	char timeline[32];
	timeDef deftime;

converttime_todef(&deftime, eltime);

bdays = bhours = bmins = false;

strcpy(buffer,"");
if( deftime.days ) {
	sprintf( timeline, "%d %s", deftime.days, ( ( deftime.days == 1 ) ? "day" : "days" ) );
	strcat(buffer, timeline);
	bdays = true;
} 

if ( deftime.hours ) {
	if( bdays )
		strcat(buffer, " ");
	sprintf( timeline, "%d %s", deftime.hours, ( ( deftime.hours == 1 ) ? "hour" :"hours" ) );
	strcat(buffer, timeline);
	bhours = true;
} 

if ( deftime.minutes ) {
	if( ( bdays ) || ( bhours ) )
		strcat(buffer, " ");
	sprintf( timeline, "%d %s", deftime.minutes, ( ( deftime.minutes == 1 ) ? "minute" : "minutes" ) );
	strcat(buffer, timeline);
	bmins = true;
} 

if ( deftime.seconds ) {
	if( ( bdays ) || ( bhours ) || ( bmins ) )
		strcat(buffer, " ");
	sprintf( timeline, "%d %s", deftime.seconds, ( ( deftime.seconds == 1 ) ? "second" : "seconds" ) );
	strcat(buffer, timeline);
}

if( !( strlen( buffer ) ) ) {
	strcpy(buffer, "Never, or bad time input!" );
}


return;
}


void getsys_infos( proginfoDef *proginfo, struct sysinfo sinfo ) {
	int pid;
	FILE *file;
	char fname[256];
	float shiftfloat;

pid = getpid();
sprintf( fname, "/proc/%d/stat", pid );
if( ( file = fopen( fname, "r" ) ) ) {
	if( fscanf( file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %u %u %u %u %d %d %*d %*d %d %*d %*u %*u %u %u %u", &proginfo->stcminflt, &proginfo->stminflt, &proginfo->stmajflt, &proginfo->stcmajflt, &proginfo->stutime, &proginfo->ststime, &proginfo->stpriority, &proginfo->ststarttime, &proginfo->stvsize, &proginfo->strss ) != 10 ) {
		 if( options.verbose )
	printf("Error getting process stat Info...\n");
	syslog(LOG_ERR, "Error getting process stat Info...\n");
	}
	fclose( file );
}

proginfo->runtime = sinfo.uptime - CT_TO_SECS( ( (float)proginfo->ststarttime ) );
proginfo->userload = 100.0 * ( CT_TO_SECS( ( (float)proginfo->stutime ) ) / proginfo->runtime );
proginfo->kernelload = 100.0 * ( CT_TO_SECS( ( (float)proginfo->ststime ) ) / proginfo->runtime );

shiftfloat=(float)(1<<SI_LOAD_SHIFT);
proginfo->loadavg[0]=((float)sinfo.loads[0])/shiftfloat;
proginfo->loadavg[1]=((float)sinfo.loads[1])/shiftfloat;
proginfo->loadavg[2]=((float)sinfo.loads[2])/shiftfloat;

return;
}

void iohttpFunc_status( svConnectionPtr cnt ) {
	char addstring[32];
	char stringuptime[128];
	struct sysinfo sysinfod;
	struct utsname stustname;
	proginfoDef pinfod;
	cpuInfo cpuinfo;

cpuGetInfo( &cpuinfo );

if( sysinfo(&sysinfod) != 0 ) {
 	if( options.verbose )
		printf("Failure getting system infomation... Critical failure.\n");
	syslog(LOG_ERR, "Failure getting system infomation... Critical failure." );
	sysconfig.shutdown = true; return;
}

getsys_infos( &pinfod, sysinfod );


iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, FMENU_SERVER );
svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td width=\"7%\">&nbsp;</td>" );


svSendString( cnt, "<table width=\"100%\" border=\"0\"><tr><td width=\"50%\" align=\"left\" valign=\"top\">" );
sprintf(addstring, "%s status", sysconfig.servername );
iohttpFunc_boxstart( cnt, addstring);
svSendString( cnt, "<table border=\"0\"><tr><td>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>General status</td><td>&nbsp;:&nbsp;</td><td>No problems detected</td></tr>" ); // Should we partially keep running through signals?
if( irccfg.bot ) {
	svSendPrintf( cnt, "<tr><td>IRC Bot status</td><td>&nbsp;:&nbsp;</td><td>Enabled (Host:%s, Channel:%s)</td></tr>", irccfg.host, irccfg.channel );
} else {
	svSendString( cnt, "<tr><td>IRC Bot status</td><td>&nbsp;:&nbsp;</td><td>Disabled</td></tr>" );
}

iohttpFuncConvertTime((char *)&stringuptime,pinfod.runtime);
svSendPrintf( cnt, "<tr><td>Game Uptime</td><td>&nbsp;:&nbsp;</td><td id=\"gameuptime\">%s</td></tr>", stringuptime );
svSendPrintf( cnt, "<tr><td>Current date</td><td>&nbsp;:&nbsp;</td><td>Week <span id=\"sstatweeks\">%d</span>, year <span id=\"sstatyears\">%d</span></td></tr>", ticks.number % 52, ticks.number / 52 );
svSendPrintf( cnt, "<tr><td>Tick time</td><td>&nbsp;:&nbsp;</td><td>%d seconds</td></tr>", sysconfig.ticktime );

if( ticks.status ) {
	svSendPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td id=\"sstatsTime\">%d seconds</td></tr>", (int)( ticks.next - time(0) ) );
} else {
	svSendPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td>Time Frozen!</td></tr>" );
}
svSendPrintf( cnt, "<tr><td>Process priority</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", pinfod.stpriority );
svSendString( cnt, "</table><br>" );

svSendString( cnt, "<b>Game Server usage ( average )</b><br>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
svSendPrintf( cnt, "<tr><td>Memory used</td><td>&nbsp;:&nbsp;</td><td id=\"memused\">%d bytes ( %d mb )</td></tr>", pinfod.stvsize, pinfod.stvsize >> 20 );
svSendPrintf( cnt, "<tr><td>Resident Size</td><td>&nbsp;:&nbsp;</td><td id=\"strss\">%d pages</td></tr>", pinfod.strss );
svSendPrintf( cnt, "<tr><td>Total CPU usage</td><td>&nbsp;:&nbsp;</td><td id=\"cputotal\">%.3f %%</td></tr>", pinfod.userload + pinfod.kernelload );
svSendPrintf( cnt, "<tr><td>In kernel mode</td><td>&nbsp;:&nbsp;</td><td id=\"cpukernel\">%.3f %%</td></tr>", pinfod.kernelload ); 
svSendPrintf( cnt, "<tr><td>In user mode</td><td>&nbsp;:&nbsp;</td><td id=\"cpuuser\">%.3f %%</td></tr>", pinfod.userload );
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
svSendPrintf( cnt, "<tr><td>Uptime</td><td>&nbsp;:&nbsp;</td><td id=\"hostuptime\">%s</td></tr>", stringuptime );
svSendString( cnt, "</table><br>" );


svSendString( cnt, "<b>Server Processor Info</b><br>" );
svSendString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );

if( (pinfod.loadavg[0] > 0) || (pinfod.loadavg[1] > 0) || (pinfod.loadavg[2] > 0) )
	svSendPrintf( cnt, "<tr><td>Load Averages</td><td>&nbsp;:&nbsp;</td><td id=\"cpuloads\">%f (1 min) - %f (5 mins) - %f (15 mins)</td></tr>",pinfod.loadavg[0],pinfod.loadavg[1],pinfod.loadavg[2]);

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
svSendPrintf( cnt, "<tr><td>Avalible memory</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"memavbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"memavmeg\">( %ld mb )</td></tr>", sysinfod.freeram, (sysinfod.freeram >> 20) );
if( (sysinfod.totalswap) > 0 ) {
	svSendPrintf( cnt, "<tr><td>Total Swap</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"totalswapbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"totalswapmeg\">( %ld mb )</td></tr>", sysinfod.totalswap, (sysinfod.totalswap >> 20) );
	svSendPrintf( cnt, "<tr><td>Free Swap</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"freeswapbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"freeswapmeg\">( %ld mb )</td></tr>", sysinfod.freeswap, (sysinfod.freeswap >> 20) );
}
if( (sysinfod.bufferram) > 0 )
	svSendPrintf( cnt, "<tr><td>Buffer Usage</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"bufferbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"bufermeg\">( %ld mb )</td></tr>", sysinfod.bufferram, (sysinfod.bufferram >> 20));
if( (sysinfod.sharedram) > 0 )
	svSendPrintf( cnt, "<tr><td>Shared Ram</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"sharedbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"sharedmeg\">( %ld mb )</td></tr>", sysinfod.sharedram, (sysinfod.sharedram >> 20) );
svSendString( cnt, "</table>" );
svSendString( cnt, "</td></tr></table>" );
iohttpFunc_boxend( cnt );


iohttpFunc_endhtml( cnt );

  return;
}


