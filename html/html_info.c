

void converttime_todef( timeDef *statime, long stime ) {

statime->weeks = ( stime / week );
statime->days = ( ( stime % week ) / day );
statime->hours = ( ( stime % day ) / hour );
statime->minutes = ( ( stime % hour ) / minute );
statime->seconds = ( stime % minute );

return;
}

static StringBufferPtr time_string_buffer;

char *TimeToString( long eltime ) {
	bool bweeks, bdays, bhours, bmins;
	timeDef deftime;

if( time_string_buffer ) {
	time_string_buffer->off = 0;
} else if( NULL == ( time_string_buffer = calloc( 1, sizeof(StringBufferDef) ) ) ) {
	critical( "URL allocation error!" );
	return NULL;
}

converttime_todef(&deftime, eltime);

bweeks = bdays = bhours = bmins = false;


if( deftime.weeks ) {
	AddBufferPrintf( time_string_buffer, "%ld %s", deftime.weeks, ( ( deftime.weeks == 1 ) ? "week" : "weeks" ) );
	bweeks = true;
}

if( deftime.days ) {
	if( bweeks ) {
		AddBufferString( time_string_buffer, " " );
	}
	AddBufferPrintf( time_string_buffer, "%ld %s", deftime.days, ( ( deftime.days == 1 ) ? "day" : "days" ) );
	bdays = true;
}

if ( deftime.hours ) {
	if( bdays ) {
		AddBufferString( time_string_buffer, " " );
	}
	AddBufferPrintf( time_string_buffer, "%ld %s", deftime.hours, ( ( deftime.hours == 1 ) ? "hour" :"hours" ) );
	bhours = true;
}

if ( deftime.minutes ) {
	if( ( bdays ) || ( bhours ) ) {
		AddBufferString( time_string_buffer, " " );
	}
	AddBufferPrintf( time_string_buffer, "%ld %s", deftime.minutes, ( ( deftime.minutes == 1 ) ? "minute" : "minutes" ) );
	bmins = true;
}

if ( deftime.seconds ) {
	if( ( bdays ) || ( bhours ) || ( bmins ) ) {
		AddBufferString( time_string_buffer, " " );
	}
	AddBufferPrintf( time_string_buffer, "%ld %s", deftime.seconds, ( ( deftime.seconds == 1 ) ? "second" : "seconds" ) );
}

if( time_string_buffer->off == 0 ) {
	AddBufferString( time_string_buffer, "Never, or bad time input!" );
}

return time_string_buffer->buf;
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
html_boxstart( cnt, true, addstring);
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
	if( ( ticks.locked == false ) && ( sysconfig.autostart ) && ( timediff(sysconfig.start) >= 1 ) ) {
		httpPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td id=\"sstatsStartTime\">%s</td></tr>", TimeToString( timediff(sysconfig.start) ) );
	} else {
		httpPrintf( cnt, "<tr><td>Next tick</td><td>&nbsp;:&nbsp;</td><td>Time Frozen!</td></tr>" );
	}
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
html_boxend( cnt );
httpString( cnt, "</td><td width=\"50%\" align=\"left\" valign=\"top\">" );
html_boxstart( cnt, true, "Server Info" );
httpString( cnt, "<table border=\"0\"><tr><td>" );
httpString( cnt, "<b>System Core</b><br>" );
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


httpString( cnt, "<b>System Processor</b><br>" );
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
if( cpuinfo.socketlogicalcores != cpuinfo.socketphysicalcores ) {
	httpPrintf( cnt, "<tr><td>Logical Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketlogicalcores );
}
if( cpuinfo.socketcount > 1 ) {
	httpPrintf( cnt, "<tr><td>Sockets</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.socketcount );
	if( cpuinfo.socketphysicalcores != cpuinfo.totalcorecount ) {
		httpPrintf( cnt, "<tr><td>Total Cores</td><td>&nbsp;:&nbsp;</td><td>%d</td></tr>", cpuinfo.totalcorecount );
	}
}
httpString( cnt, "</table><br>" );
httpString( cnt, "<b>System Memory</b><br>" );
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
html_boxend( cnt );
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


void iohtmlFunc_info( ReplyDataPtr cnt ) {
	int a, b, id, table;
	char *type;
	bool c;
	dbUserMainDef maind;
	ConfigArrayPtr settings;

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	if( iohtmlVarsFind( cnt, "request" ) ) {
		iohtmlBase( cnt, 1|32 );
	} else {
		iohtmlBase( cnt, 1 );
		if( !( iohtmlHeader( cnt, id, &maind ) ) ) {
			return;
		}
	}
} else {
	if( iohtmlVarsFind( cnt, "request" ) ) {
		iohtmlBase( cnt, 8|32 );
	} else {
		iohtmlBase( cnt, 8 );
		iohtmlFunc_frontmenu( cnt, FMENU_NONE );
	}
}

settings = GetSetting( "Server Name" );

type = iohtmlVarsFind( cnt, "type" );

if( type == NULL ) {
	goto INVALID;
} else if( strcmp( type, "races" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Races", settings->string_value );
	goto RACES;
} else if( strcmp( type, "units" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Units", settings->string_value );
	goto UNITS;
} else if( strcmp( type, "buildings" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Buildings", settings->string_value );
	goto BUILDINGS;
} else if( strcmp( type, "smileys" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Forum Smileys", settings->string_value );
	goto SMILEYS;
} else if( strcmp( type, "artefacts" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Artefacts Info", settings->string_value );
	goto ARTEFACTS;
} else {
	INVALID:
	iohtmlBodyInit( cnt, "%s: Info", settings->string_value );
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s&type=races\">Races Info</a><br>", URLAppend( cnt, "info" ) );
	httpPrintf( cnt, "<a href=\"%s&type=units\">Units Info</a><br>", URLAppend( cnt, "info" ) );
	httpPrintf( cnt, "<a href=\"%s&type=buildings\">Building Info</a><br>", URLAppend( cnt, "info" ) );
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s&type=smileys\">Forum Smileys</a><br>", URLAppend( cnt, "info" ) );
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s&type=artefacts\">Artefacts Info</a><br>", URLAppend( cnt, "info" ) );
	goto RETURN;
}


RACES:
for( a = 0; a < CMD_RACE_NUMUSED ; a++) {
	httpPrintf( cnt, "<div class=\"genlarge\">%s</div><br>", cmdRaceName[a] );
	if( cmdRace[a].special )
	httpPrintf( cnt, "<i><b>Special bonus.</b></i><br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_POPRESEARCH )
		httpString( cnt, "Each 6000 population produces 1 research point every week!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_SOLARP15 )
		httpString( cnt, "Solar Production increased by 15%!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_CULPROTECT )
		httpString( cnt, "Culture Research production provides a psychic shield for planets!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_IDW )
		httpString( cnt, "Imune to Dark Web Effects!<br>" );

	httpString( cnt, "<table width=\"*\" border=\"0\"><tr>" );
	httpString( cnt, "<td valign=\"top\" width=\"250\"><i><b>Main bonuses</b></i><br>" );
	if( (cmdRace[a].attack - 1) )
	httpPrintf( cnt, "%+.0f%% Attack.<br>", ( cmdRace[a].attack - 1 ) * 100 );
	if( cmdRace[a].defense - 1 )
	httpPrintf( cnt, "%+.0f%% Defence.<br>", ( cmdRace[a].defense - 1 ) * 100 );
	if( ( ( cmdRace[a].speed / 2 ) - 1 ) )
		httpPrintf( cnt, "%+.0f%% Travel Speed<br>", ( ( cmdRace[a].speed / 2 ) - 1 ) *100 );
	for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ ) {
		if( (cmdRace[a].unit[b] - 1) )
			httpPrintf( cnt, " %+.0f%% %s strength.<br>", ( cmdRace[a].unit[b] - 1 ) * 100, cmdUnitName[b] );
	}
	c = false;
	if( (int)((((cmdRace[a].growth)/0.02) - 1 ) * 100 ) != 0 ) {
		httpPrintf( cnt, "<br><i><b>Ressource bonuses</b></i><br>" );
		httpPrintf( cnt, "%+.0f%% Population Growth<br>",  (((cmdRace[a].growth)/0.02) - 1 ) * 100 );
		c = true;
	}
	for( b = 0; b < CMD_RESSOURCE_NUMUSED ; b++) {
		if( (cmdRace[a].resource[b] - 1) ) {
			if( c == false ) {
				httpPrintf( cnt, "<br><i><b>Ressource bonuses</b></i><br>" );
				c = true;
			}
			httpPrintf( cnt, "%+.0f%% %s production<br>", ( cmdRace[a].resource[b] - 1 ) * 100, cmdRessourceName[b] );
		}
	}
	httpString( cnt, "</td><td valign=\"top\" width=\"225\">" );
	httpPrintf( cnt, "<i><b>Research bonuses</b></i><br>" );
	for( b = 0 ; b < CMD_RESEARCH_NUMUSED ; b++ ) {
		if( cmdRace[a].researchmax[b] != 200 )
			httpPrintf( cnt, "%.0f%% %s limit.<br>", cmdRace[a].researchmax[b], cmdResearchName[b] );
	}
	for( b = 0 ; b < CMD_RESEARCH_NUMUSED ; b++ ) {
		if( ( cmdRace[a].researchpoints[b] - 1 ) )
			httpPrintf( cnt, "%+.0f%% %s<br>", ( cmdRace[a].researchpoints[b] - 1 ) * 100, cmdResearchName[b] );
	}
	httpString( cnt, "</td><td valign=\"top\" width=\"140\"><i><b>Operations</b></i>" );
	for( b = 0 ; b < CMD_AGENTOP_NUMUSED ; b++ ) {
		if( specopAgentsAllowed( b, a ) )
			httpPrintf( cnt, "<br>%s", cmdAgentopName[b] );
	}
	httpString( cnt, "<br><br><i><b>Spells</b></i>" );
	for( b = 0 ; b < CMD_PSYCHICOP_NUMUSED ; b++ ) {
		if( specopPsychicsAllowed( b, a ) )
			httpPrintf( cnt, "<br>%s", cmdPsychicopName[b] );
	}
	httpString( cnt, "</td>" );
	httpString( cnt, "<td valign=\"top\" width=\"140\"><i><b>Incantations</b></i>" );
	for( b = 0 ; b < CMD_GHOSTOP_NUMUSED ; b++ ) {
		if( specopGhostsAllowed( b, a ) )
			httpPrintf( cnt, "<br>%s", cmdGhostopName[b] );
	}
	httpString( cnt, "</td></tr></table>" );
	httpString( cnt, "<br><br>" );
}
goto RETURN;
UNITS:


httpString( cnt, "<table width=\"100%\">" );

for( a = table = 0; a < CMD_UNIT_NUMUSED; a++, table++ ) {
	if( table == 3 ) {
		httpString( cnt, "</tr><tr><td>&nbsp;</td></tr>" );
		table = 0;
	} else {
		httpString( cnt, "<td>&nbsp;&nbsp;&nbsp;</td>" );
	}
	if( table == 0 ) {
		httpString( cnt, "<tr>" );
	}
	httpString( cnt, "<td valign=\"top\" align=\"center\">" );
	httpPrintf( cnt, "<span class=\"genlarge\">%s</span><br>", cmdUnitName[a] );
	httpString( cnt, "<table>" );
	if( a != CMD_UNIT_PHANTOM ) {
		httpString( cnt, "<tr><td><span class=\"genblue\">Base Cost:</span></td><td>" );
		httpString( cnt, "<table>" );
		for( b = 0; b < CMD_RESSOURCE_NUMUSED; b++) {
			if( cmdUnitCost[a][b] > 0 ) {
				httpPrintf( cnt, "<tr><td align=\"right\">%lld</td><td align=\"left\">%s</td></tr>", (long long)cmdUnitCost[a][b], cmdRessourceName[b] );
			}
		}
		httpString( cnt, "</table></td>" );
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Build Time:</span></td><td>%lld Ticks</td></tr>", (long long)cmdUnitCost[a][CMD_RESSOURCE_TIME] );
	} else {
		httpString( cnt, "<tr><td><span class=\"genblue\">Special Unit:</span></td><td>Can not be built.</td></tr>" );
	}
	if( cmdUnitUpkeep[a] > 0 ) {
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>%.2f %s per Tick</td></tr>", cmdUnitUpkeep[a], cmdRessourceName[CMD_RESSOURCE_ENERGY] );
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Travel Upkeep:</span></td><td>%.2f %s per Tick</td></tr>", (cmdUnitUpkeep[a] * cmdUnitPenalty[0]), cmdRessourceName[CMD_RESSOURCE_ENERGY] );
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Station Upkeep:</span></td><td>%.2f %s per Tick</td></tr>", (cmdUnitUpkeep[a] * cmdUnitPenalty[1]), cmdRessourceName[CMD_RESSOURCE_ENERGY] );
	} else {
		httpString( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>No Upkeep</td></tr>" );
	}
	if( cmdUnitTech[a] > 0 ) {
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Tech:</span></td><td>%d%%</td></tr>", cmdUnitTech[a] );
	}
	for( b = 0; b < CMD_UNIT_STATS_NUMUSED; b++ ) {
		if( cmdUnitStats[a][b] > 0 ) {
			httpPrintf( cnt, "<tr><td><span class=\"genblue\">%s:</span></td><td>%d</td></tr>", cmdUnitStatsNames[b], cmdUnitStats[a][b] );
		}
	}
	
	httpString( cnt, "</table>" );
	httpString( cnt, "</td>" );
}

httpString( cnt, "</table>" );


goto RETURN;
BUILDINGS:

httpString( cnt, "<table width=\"100%\">" );

for( a = table = 0; a < CMD_BLDG_NUMUSED+1; a++, table++ ) {
	if( table == 3 ) {
		httpString( cnt, "</tr><tr><td>&nbsp;</td></tr>" );
		table = 0;
	} else {
		httpString( cnt, "<td>&nbsp;&nbsp;&nbsp;</td>" );
	}
	if( table == 0 ) {
		httpString( cnt, "<tr>" );
	}
	httpString( cnt, "<td valign=\"top\" align=\"center\">" );
	httpPrintf( cnt, "<span class=\"genlarge\">%s</span><br>", cmdBuildingName[a] );
	httpString( cnt, "<table>" );
	httpString( cnt, "<tr><td><span class=\"genblue\">Base Cost:</span></td><td>" );
	httpString( cnt, "<table>" );
	for( b = 0; b < CMD_RESSOURCE_NUMUSED; b++) {
		if( cmdBuildingCost[a][b] > 0 ) {
			httpPrintf( cnt, "<tr><td align=\"right\">%lld</td><td align=\"left\">%s</td></tr>", (long long)cmdBuildingCost[a][b], cmdRessourceName[b] );
		}
	}
	httpString( cnt, "</table></td>" );
	httpPrintf( cnt, "<tr><td><span class=\"genblue\">Build Time:</span></td><td>%lld Ticks</td></tr>", (long long)cmdBuildingCost[a][CMD_RESSOURCE_TIME] );
	if( a < CMD_BLDG_NUMUSED ) {
		if( cmdBuildingUpkeep[a] > 0 ) {
			httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>%.2f per Tick</td></tr>", cmdBuildingUpkeep[a] );
		} else {
			httpString( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>No Upkeep</td></tr>" );
		}
		if( cmdBuildingProduction[a] > 0 ) {
			if( a == CMD_BUILDING_CITIES ) {
				httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Capacity:</span></td><td>%.0f Population</td></tr>", cmdBuildingProduction[a] );
			} else {
				httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Output:</span></td><td>%.2f per Tick</td></tr>", cmdBuildingProduction[a] );
			}
		}
		if( cmdBuildingTech[a] > 0 ) {
			httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Tech:</span></td><td>%d%%</td></tr>", cmdBuildingTech[a] );
		}
	}
	httpPrintf( cnt, "<tr><td><span class=\"genblue\">Networth:</span></td><td>%d</td></tr>", cmdBuildingNetworth[a] );
	httpString( cnt, "</table>" );
	httpString( cnt, "</td>" );
}

httpString( cnt, "</table>" );
goto RETURN;

SMILEYS:
httpPrintf( cnt, "There are %d Smileys in total.<br>", IOHTTP_FORUM_SMILETOTAL );
httpString( cnt, "<table><tr>" );
for( a = table = 0; a < IOHTTP_FORUM_SMILETOTAL; a++, table++ ) {
	if( table == 5 ) {
		httpString( cnt, "</tr><tr>" );
		table = 0;
	}
	httpPrintf( cnt, "<td align=\"right\"><img src=\"files?type=image&name=smilies/%s\"></td><td align=\"left\">%s</td>", SmileTable[a]->name, SmileTable[a]->string );

}
httpString( cnt, "</tr></table>" );
goto RETURN;

ARTEFACTS:

httpPrintf( cnt, "There are %d Artefacts in total.<br>", ArtefactTotal );
httpString( cnt, "<table class=\"left\">" );
for( a = b = 0; a < ArtefactTotal; a++ ) {
	httpString( cnt, "<tr>" );
	httpPrintf( cnt, "<tr><img src=\"files?type=image&name=artefact/%s\"></td>", ArtefactTable[a]->image );
	httpPrintf( cnt, "<td align=\"left\">%s</td>", ArtefactTable[a]->name );
	httpPrintf( cnt, "<td>%s</td>", ( ArtefactTable[a]->active ? "Activated" : "&nbsp;" )  );
	httpString( cnt, "</tr>" );
	b += ArtefactTable[a]->active;
}
httpString( cnt, "</table>" );
if( b ) {
	httpPrintf( cnt, "%d Artefacts are currently Active.", b );
}


RETURN:
iohtmlBodyEnd( cnt );

return;
}


void iohtmlFunc_rules( ReplyDataPtr cnt ) {
	struct stat stdata;
	char *data;
	char DIRCHECKER[PATH_MAX];
	ConfigArrayPtr settings[2];
	FILE *file;

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_NONE );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );

settings[0] = GetSetting( "Server Name" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpPrintf( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>General Rules for %s.</b></font></td></tr>", settings[0]->string_value );
httpString( cnt, "<tr><td><font size=\"2\">" );

settings[1] = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/rules.txt", settings[1]->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
			httpString( cnt, "<ul>" );
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( data[0] == '#' ) {
						continue;
					}
					if( strlen(data) > 1 ) {
						httpPrintf( cnt, "<li>%s</li>", trimwhitespace(data) );
					}
				}
			httpString( cnt, "</ul>" );
			}
			fclose( file );
		}
		free( data );
		httpString( cnt, "<br>" );
		httpPrintf( cnt, "Rules are subject to change at any time and applicable to every instance of %s.<br>", settings[0]->string_value );
		httpString( cnt, "<br>" );
		httpString( cnt, "Administration is open to discussion regarding these rules. In all cases the Administration's decision is final.<br>" );
		httpString( cnt, "While we are open to discuss these rules, they always apply unless specificly waived/altered by Administration. In which case you will be notified.<br>" );
		httpString( cnt, "<br>" );
		httpString( cnt, "Players breaking the rules will get a warning, an account reset/deletion or a permanent ban.<br>" );
		httpString( cnt, "When a player gets warned, his player tag will be changed to “Warned” for a minimum of 4 days.<br>" );
		httpString( cnt, "You do not get 2 warnings. A second violation is an account reset (your records will be kept).<br>" );
		httpString( cnt, "A third violation is an account deletion and a fourth violation will require me to go all out, and find a way to ban you.<br>" );
	}
}

httpString( cnt, "</td></tr></table><br><br>" );

iohtmlFunc_endhtml( cnt );

return;
}


