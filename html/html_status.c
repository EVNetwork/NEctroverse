

void converttime_todef( timeDef *statime, long stime ) {

statime->weeks = ( stime / week );
statime->days = ( ( stime % week ) / day );
statime->hours = ( ( stime % day ) / hour );
statime->minutes = ( ( stime % hour ) / minute );
statime->seconds = ( stime % minute );

return;
}

#define MAX_TIME_STRING 1024
char *TimeToString( long eltime ) {
	int offset = 0;
	bool bweeks, bdays, bhours, bmins;
	char buffer[MAX_TIME_STRING];
	char *ret;
	timeDef deftime;

converttime_todef(&deftime, eltime);

bweeks = bdays = bhours = bmins = false;

memset( &buffer, 0, MAX_TIME_STRING );

if( deftime.weeks ) {
	offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%ld %s", deftime.weeks, ( ( deftime.weeks == 1 ) ? "week" : "weeks" ) );
	bweeks = true;
}

if( deftime.days ) {
	if( bweeks )
		offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%s", " " );
	offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%ld %s", deftime.days, ( ( deftime.days == 1 ) ? "day" : "days" ) );
	bdays = true;
}

if ( deftime.hours ) {
	if( bdays )
		offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%s", " " );
	offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%ld %s", deftime.hours, ( ( deftime.hours == 1 ) ? "hour" :"hours" ) );
	bhours = true;
}

if ( deftime.minutes ) {
	if( ( bdays ) || ( bhours ) )
		offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%s", " " );
	offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%ld %s", deftime.minutes, ( ( deftime.minutes == 1 ) ? "minute" : "minutes" ) );
	bmins = true;
}

if ( deftime.seconds ) {
	if( ( bdays ) || ( bhours ) || ( bmins ) )
		offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%s", " " );
	offset += snprintf( &buffer[offset], (MAX_TIME_STRING - offset), "%ld %s", deftime.seconds, ( ( deftime.seconds == 1 ) ? "second" : "seconds" ) );
}

if( !( strlen( buffer ) ) ) {
	strcpy(buffer, "Never, or bad time input!" );
}

ret = buffer;

return ret;
}

void getsys_infos( proginfoDef *proginfo ) {
	int pid;
	FILE *file;
	char fname[PATH_MAX];
	float shiftfloat;
	struct sysinfo sinfo;

if( sysinfo(&sinfo) != 0 ) {
	critical( "Failure getting system infomation... Critical failure." );
	sysconfig.shutdown = true; return;
}

pid = getpid();
sprintf( fname, "/proc/%d/stat", pid );
if( ( file = fopen( fname, "r" ) ) ) {
	if( fscanf( file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %ld %*d %ld %*u %llu %lu %ld", &proginfo->stutime, &proginfo->ststime, &proginfo->stpriority, &proginfo->threads, &proginfo->ststarttime, &proginfo->stvsize, &proginfo->strss ) != 7 ) {
	error( "Error getting process stat Info..." );
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
void iohtmlFunc_status( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings[2];
	char timebuf[512];
	char addstring[32];
	struct sysinfo sysinfod;
	struct utsname stustname;
	proginfoDef pinfod;
	cpuInfo cpuinfo;
	time_t tint;

cpuGetInfo( &cpuinfo );
getsys_infos( &pinfod );

if( sysinfo(&sysinfod) != 0 ) {
	critical( "Failure getting system infomation... Critical failure." );
	sysconfig.shutdown = true; return;
}



iohtmlBase( cnt, 8 | 16 );
iohtmlFunc_frontmenu( cnt, FMENU_SERVER );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );

httpString( cnt, "<table width=\"100%\" border=\"0\"><tr><td width=\"50%\" align=\"left\" valign=\"top\">" );
settings[0] = GetSetting( "Server Name" );
sprintf(addstring, "%s status", settings[0]->string_value );
iohtmlFunc_boxstart( cnt, addstring);
httpString( cnt, "<table border=\"0\"><tr><td>" );
httpString( cnt, "<b>Overall Game Stats</b><br>" );
httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
httpString( cnt, "<tr><td>General status</td><td>&nbsp;:&nbsp;</td><td>No problems detected</td></tr>" ); // Should we partially keep running through signals?

#if IRCBOT_SUPPORT
if( sysconfig.irc_enabled ) {
	if( irc_is_connected(sysconfig.irc_session) ) {
		settings[0] = GetSetting( "IRC Host" );
		settings[1] = GetSetting( "IRC Channel" );
		httpPrintf( cnt, "<tr><td>IRC Bot status</td><td>&nbsp;:&nbsp;</td><td id=\"botstatus\">Enabled (Host:%s, Channel:%s)</td></tr>", settings[0]->string_value, settings[1]->string_value );
	} else {
		httpString( cnt, "<tr><td>IRC Bot status</td><td>&nbsp;:&nbsp;</td><td id=\"botstatus\">Enabled but not connected</td></tr>" );
	}
} else {
	httpString( cnt, "<tr><td>IRC Bot status</td><td>&nbsp;:&nbsp;</td><td id=\"botstatus\">Disabled</td></tr>" );
}
#endif

httpPrintf( cnt, "<tr><td>Game Uptime</td><td>&nbsp;:&nbsp;</td><td id=\"gameuptime\">%s</td></tr>", TimeToString( pinfod.runtime ) );
httpPrintf( cnt, "<tr><td>Current date</td><td>&nbsp;:&nbsp;</td><td>Week <span id=\"sstatweeks\">%d</span>, year <span id=\"sstatyears\">%d</span></td></tr>", ticks.number % 52, ticks.number / 52 );
settings[0] = GetSetting( "Tick Speed" );
httpPrintf( cnt, "<tr><td>Tick time</td><td>&nbsp;:&nbsp;</td><td>%.0f seconds</td></tr>", settings[0]->num_value );

if( ticks.status ) {
	httpPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td id=\"sstatsTime\">%s</td></tr>", ((ticks.next - time(0)) > 0) ? TimeToString( ticks.next - time(0) ) : "Now..." );
} else {
	httpPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td>Time Frozen!</td></tr>" );
}
httpPrintf( cnt, "<tr><td>Process priority</td><td>&nbsp;:&nbsp;</td><td id=\"servpriority\">%ld</td></tr>", pinfod.stpriority );
httpPrintf( cnt, "<tr><td>Threads Active</td><td>&nbsp;:&nbsp;</td><td id=\"servthreads\">%ld</td></tr>", pinfod.threads );
httpString( cnt, "</table><br>" );

httpString( cnt, "<b>Game Server usage ( average )</b><br>" );
httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
httpPrintf( cnt, "<tr><td>Memory used</td><td>&nbsp;:&nbsp;</td><td id=\"memused\">%lu bytes ( %5.1f mb )</td></tr>", pinfod.stvsize, pinfod.stvsize  / megabyte  );
httpPrintf( cnt, "<tr><td>Resident Size</td><td>&nbsp;:&nbsp;</td><td id=\"strss\">%ld pages</td></tr>", pinfod.strss );
httpPrintf( cnt, "<tr><td>Total CPU usage</td><td>&nbsp;:&nbsp;</td><td id=\"cputotal\">%.3f %%</td></tr>", pinfod.userload + pinfod.kernelload );
httpPrintf( cnt, "<tr><td>In kernel mode</td><td>&nbsp;:&nbsp;</td><td id=\"cpukernel\">%.3f %%</td></tr>", pinfod.kernelload );
httpPrintf( cnt, "<tr><td>In user mode</td><td>&nbsp;:&nbsp;</td><td id=\"cpuuser\">%.3f %%</td></tr>", pinfod.userload );
httpString( cnt, "</table>" );

httpString( cnt, "</td></tr></table>" );
iohtmlFunc_boxend( cnt );
httpString( cnt, "</td><td width=\"50%\" align=\"left\" valign=\"top\">" );
iohtmlFunc_boxstart( cnt, "Server Info" );
httpString( cnt, "<table border=\"0\"><tr><td>" );
httpString( cnt, "<b>System OS</b><br>" );
uname( &stustname );
httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );

time( &tint );
strftime(timebuf,512,"%a, %d %b %G %T %Z", localtime( &tint ) );
httpPrintf( cnt, "<tr><td>Server Time</td><td>&nbsp;:&nbsp;</td><td id=\"timeserver\">%s</td></tr>", timebuf );
strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
httpPrintf( cnt, "<tr><td>GMT/UTC Time</td><td>&nbsp;:&nbsp;</td><td id=\"timegmt\">%s</td></tr>", timebuf );

httpPrintf( cnt, "<tr><td>Sysname</td><td>&nbsp;:&nbsp;</td><td>%s %s</td></tr>", stustname.sysname, stustname.release );
httpPrintf( cnt, "<tr><td>Release</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", stustname.version );
httpPrintf( cnt, "<tr><td>Uptime</td><td>&nbsp;:&nbsp;</td><td id=\"hostuptime\">%s</td></tr>", TimeToString( sysinfod.uptime ) );
httpPrintf( cnt, "<tr><td>Processes</td><td>&nbsp;:&nbsp;</td><td id=\"hostprocs\">%d</td></tr>", sysinfod.procs );
httpString( cnt, "</table><br>" );


httpString( cnt, "<b>Server Processor Info</b><br>" );
httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );

if( (pinfod.loadavg[0] > 0) || (pinfod.loadavg[1] > 0) || (pinfod.loadavg[2] > 0) )
	httpPrintf( cnt, "<tr><td>Load Averages</td><td>&nbsp;:&nbsp;</td><td id=\"cpuloads\">%f (1 min) - %f (5 mins) - %f (15 mins)</td></tr>",pinfod.loadavg[0],pinfod.loadavg[1],pinfod.loadavg[2]);

httpPrintf( cnt, "<tr><td>CPU Type</td><td>&nbsp;:&nbsp;</td><td>%s %s</td></tr>", cpuinfo.identifier, cpuArchName[cpuinfo.arch] );
httpPrintf( cnt, "<tr><td>%s</td><td>&nbsp;:&nbsp;</td><td>%s</td></tr>", cpuinfo.vendorstring, cpuClassName[cpuinfo.class] );

if( cpuinfo.cachesizeL3 > 0 )
	httpPrintf( cnt, "<tr><td>Level 3 Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cachesizeL3 );

if( cpuinfo.cachesizeL2 > 0 )
	httpPrintf( cnt, "<tr><td>Level 2 Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cachesizeL2 );

if( cpuinfo.cachesizeL1data > 0 )
	httpPrintf( cnt, "<tr><td>Level 1 Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cachesizeL1data );

if( cpuinfo.cacheunifiedL1 > 0 )
	httpPrintf( cnt, "<tr><td>Unified Cache</td><td>&nbsp;:&nbsp;</td><td>%d k</td></tr>", cpuinfo.cacheunifiedL1 );

httpPrintf( cnt, "<tr><td>Physical Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketphysicalcores );
httpPrintf( cnt, "<tr><td>Logical Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketlogicalcores );

if( cpuinfo.socketcount ) {
	httpPrintf( cnt, "<tr><td>Sockets</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketcount );
	httpPrintf( cnt, "<tr><td>Total Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.totalcorecount );
}
httpString( cnt, "</table><br>" );
httpString( cnt, "<b>System RAM infomation</b><br>" );
httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" );
httpPrintf( cnt, "<tr><td>Total memory</td><td>&nbsp;-&nbsp;</td><td align=\"right\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\">( %5.1f mb )</td></tr>", sysinfod.totalram, (sysinfod.totalram  / megabyte ) );
httpPrintf( cnt, "<tr><td>Avalible memory</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"memavbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"memavmeg\">( %5.1f mb )</td></tr>", sysinfod.freeram, (sysinfod.freeram  / megabyte ) );
if( (sysinfod.totalswap) > 0 ) {
	httpPrintf( cnt, "<tr><td>Total Swap</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"totalswapbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"totalswapmeg\">( %5.1f mb )</td></tr>", sysinfod.totalswap, (sysinfod.totalswap  / megabyte ) );
	httpPrintf( cnt, "<tr><td>Free Swap</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"freeswapbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"freeswapmeg\">( %5.1f mb )</td></tr>", sysinfod.freeswap, (sysinfod.freeswap  / megabyte ) );
}
if( (sysinfod.bufferram) > 0 )
	httpPrintf( cnt, "<tr><td>Buffer Usage</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"bufferbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"bufermeg\">( %5.1f mb )</td></tr>", sysinfod.bufferram, (sysinfod.bufferram  / megabyte ));
if( (sysinfod.sharedram) > 0 )
	httpPrintf( cnt, "<tr><td>Shared Ram</td><td>&nbsp;-&nbsp;</td><td align=\"right\" id=\"sharedbytes\">%ld bytes</td><td>&nbsp;&nbsp;</td><td align=\"right\" id=\"sharedmeg\">( %5.1f mb )</td></tr>", sysinfod.sharedram, (sysinfod.sharedram  / megabyte ) );
httpString( cnt, "</table>" );
httpString( cnt, "</td></tr></table>" );
iohtmlFunc_boxend( cnt );
httpString( cnt, "<br><br>" );

iohtmlFunc_endhtml( cnt );

  return;
}


struct tm *gettime( time_t t_in, bool gmt ) {
	time_t result;

result = t_in;

if( gmt ) {
	return gmtime( &result );
} else {
	return localtime( &result );
}

}


int timediff( struct tm t_in  ) {
	int seconds;
	struct tm variable;

time(&now);

variable = *localtime(&now);

if( t_in.tm_sec != -1 ) {
	variable.tm_sec = t_in.tm_sec;
}
if( t_in.tm_min != -1 ) {
	variable.tm_min = t_in.tm_min;
}
if( t_in.tm_hour != -1 ) {
	variable.tm_hour = t_in.tm_hour;
}
if( t_in.tm_mday != -1 ) {
	variable.tm_mday = t_in.tm_mday;
}
if( t_in.tm_mon != -1 ) {
	variable.tm_mon = t_in.tm_mon;
}
if( t_in.tm_year != -1 ) {
	variable.tm_year = t_in.tm_year;
}

seconds = (int)difftime(mktime(&variable),now);

//printf("dif: %d\n",seconds);

return seconds;
}


