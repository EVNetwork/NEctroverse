

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

int linux_get_proc_uptime( char *buffer, float *uptime )
{
  FILE *file;
  float uptime_seconds;
  file = fopen( "/proc/uptime", "r" );
  if( file )
  {
    fscanf( file, "%f", &uptime_seconds );
    fclose( file );
    if( uptime )
      *uptime = uptime_seconds;
    iohttpFuncConvertTime( buffer, (int)uptime_seconds );
    return 1;
  }
  return 0;
}

int linux_get_proc_loadavg( char *buffer )
{
  FILE *file;
  float load_1;
  float load_5;
  float load_15;
  file = fopen( "/proc/loadavg", "r" );
  if( file )
  {
    fscanf( file, "%f" "%f" "%f", &load_1, &load_5, &load_15 );
    fclose( file );
    sprintf( buffer, "%4.2f, %4.2f, %4.2f", load_1, load_5, load_15 );
    return 1;
  }
  return 0;
}

int linux_cpuinfo( char *buffer )
{
  int a;
  FILE *file;
  char temp[896];
  file = fopen( "/proc/cpuinfo", "r" );
  if( file )
  {
    a = fread( temp, 1, 896, file );
    temp[a] = 0;
    fclose( file );
    for( a = 0 ; temp[a] ; a++ )
    {
      if( temp[a] == '\n' )
      {
        memcpy( buffer, "<br>", 4 );
        buffer += 4;
        continue;
      }
      *buffer++ = temp[a];
    }
    *buffer = 0;
    return 1;
  }
  return 0;
}


void iohttpFunc_status( svConnectionPtr cnt )
{
  int pid;
  FILE *file;
  char fname[256], addstring[32];
  int stutime, ststime, stpriority, ststarttime, stvsize, strss;
  char buffer[896];
  float boottime, runtime, userload, kernelload;
  char stringuptime[128];
	struct sysinfo  si;
	struct utsname stustname;

iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, FMENU_SERVER );
svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td width=\"7%\">&nbsp;</td>" );

  pid = getpid();
  sprintf( fname, "/proc/%d/stat", pid );
  if( ( file = fopen( fname, "r" ) ) )
  {
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
  sprintf(buffer, "%s status", sysconfig.servername );
  iohttpFunc_boxstart( cnt, buffer);
  svSendString( cnt, "<table border=\"0\"><tr><td>" );
  svSendPrintf( cnt, "General status : No problems detected<br>" ); // Should we partially keep running through signals?
  svSendPrintf( cnt, "Current date : Week %d, year %d<br>", ticks.number % 52, ticks.number / 52 );
    svSendPrintf( cnt, "Tick time : %d seconds<br>", sysconfig.ticktime );
 if( ticks.status )
    svSendPrintf( cnt, "Next tick : %d seconds<br>", (int)( ticks.next - time(0) ) );
  else
    svSendPrintf( cnt, "Next tick : Time Frozen!<br>" );
  svSendPrintf( cnt, "Process priority : %d<br><br>", stpriority );
  svSendString( cnt, "<b>Server Processor(s)</b><br>" );
  linux_cpuinfo( buffer );
  svSendString( cnt, buffer );

  svSendString( cnt, "</td></tr></table>" );
iohttpFunc_boxend( cnt );
  svSendString( cnt, "</td><td width=\"50%\" align=\"left\" valign=\"top\">" );
  iohttpFunc_boxstart( cnt, "Server status" );
  svSendString( cnt, "<table border=\"0\"><tr><td>" );
  svSendString( cnt, "<b>System info</b><br>" );
  uname( &stustname );
  svSendPrintf( cnt, "Sysname : %s %s<br>", stustname.sysname, stustname.release );
  svSendPrintf( cnt, "Release : %s<br>", stustname.version );
  svSendPrintf( cnt, "Uptime : %s<br><br>", stringuptime );


  svSendString( cnt, "<b>Server program CPU usage ( average )</b><br>" );
  svSendPrintf( cnt, "Total usage : %.3f %%<br>", userload + kernelload );
  svSendPrintf( cnt, "In user mode : %.3f %%<br>", userload );
  svSendPrintf( cnt, "In kernel mode : %.3f %%<br><br>", kernelload );

  svSendString( cnt, "<b>System RAM infomation</b><br>" );
  svSendPrintf( cnt, "Total system memory : %ld bytes ( %ld mb )<br>", si.totalram, (si.totalram >> 20) );
  svSendPrintf( cnt, "Avalible memory now : %ld bytes ( %ld mb )<br>", si.freeram, (si.freeram >> 20) );
  svSendPrintf( cnt, "Server has used : %d bytes ( %d mb )<br>", stvsize, stvsize >> 20 );
  svSendPrintf( cnt, "Resident Size : %d pages<br><br>", strss );
  svSendString( cnt, "</td></tr></table>" );
  iohttpFunc_boxend( cnt );


iohttpFunc_endhtml( cnt );

  return;
}


