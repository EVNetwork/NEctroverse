
#include <sys/utsname.h>

void iohttpFuncConvertTime( unsigned char *buffer, int eltime )
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


int linux_get_proc_uptime( unsigned char *buffer, float *uptime )
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

int linux_get_proc_loadavg( unsigned char *buffer )
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

int linux_cpuinfo( unsigned char *buffer )
{
  int a;
  FILE *file;
  unsigned char temp[4096];
  file = fopen( "/proc/cpuinfo", "r" );
  if( file )
  {
    a = fread( temp, 1, 1024, file );
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





#include <asm/param.h>
#ifndef CT_TO_SECS
#define CT_TO_SECS(x) ((x)/HZ)
#endif


void iohttpFunc_status( svConnectionPtr cnt )
{
  int pid;
  FILE *file;
  unsigned char fname[256];
  int stutime, ststime, stpriority, ststarttime, stvsize, strss;
  unsigned char buffer[4096];
  float boottime, runtime, userload, kernelload;
  unsigned char stringuptime[1024];
  struct utsname stustname;
  struct stat stdata;

  pid = getpid();
  sprintf( fname, "/proc/%d/stat", pid );
  if( ( file = fopen( fname, "r" ) ) )
  {
    fscanf( file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %d %d %*d %*d %d %*d %*u %*u %u %u %u", &stutime, &ststime, &stpriority, &ststarttime, &stvsize, &strss );
    fclose( file );
  }


  stringuptime[0] = 0;
  linux_get_proc_uptime( stringuptime, &boottime );
  runtime = boottime - CT_TO_SECS( ( (float)ststarttime ) );

  userload = 100.0 * ( CT_TO_SECS( ( (float)stutime ) ) / runtime );
  kernelload = 100.0 * ( CT_TO_SECS( ( (float)ststime ) ) / runtime );


  iohttpBase( cnt, 0 );


  svSendString( cnt, "<table width=\"100%\" border=\"0\"><tr><td width=\"50%\" align=\"left\" valign=\"top\">" );

  svSendString( cnt, "<table border=\"0\"><tr><td>" );
  svSendPrintf( cnt, "<b>%s status</b><br>", SERVERNAME );
  svSendPrintf( cnt, "General status : No problems detected<br>" ); // Should we partially keep running through signals?
  svSendPrintf( cnt, "Current date : Week %d, year %d<br>", svTickNum % 52, svTickNum / 52 );
  if( svTickStatus )
    svSendPrintf( cnt, "Tick time : %d seconds left<br>", (int)( svTickTime - time(0) ) );
  else
    svSendPrintf( cnt, "Tick time : time frozen<br>" );
  svSendPrintf( cnt, "Process priority : %d<br><br>", stpriority );
  svSendString( cnt, "<b>Server Processor(s)</b><br>" );
  linux_cpuinfo( buffer );
  svSendString( cnt, buffer );
  if( stat( "/proc/kcore", &stdata ) != -1 )
  {
    svSendString( cnt, "<b>Random Access Memory</b><br>" );
    svSendPrintf( cnt, "User-space memory : %d bytes ( %d mb )<br>", (int)stdata.st_size, (int)(stdata.st_size >> 20) );
  }
  svSendString( cnt, "</td></tr></table>" );

  svSendString( cnt, "</td><td width=\"50%\" align=\"left\" valign=\"top\">" );

  svSendString( cnt, "<table border=\"0\"><tr><td>" );
  svSendString( cnt, "<b>Server status</b><br>" );
  uname( &stustname );
  svSendPrintf( cnt, "Sysname : %s %s<br>", stustname.sysname, stustname.release );
  svSendPrintf( cnt, "Release : %s<br>", stustname.version );
  svSendPrintf( cnt, "Uptime : %s<br><br>", stringuptime );
  svSendString( cnt, "<b>Server program CPU usage ( average )</b><br>" );
  svSendPrintf( cnt, "Total usage : %.3f %%<br>", userload + kernelload );
  svSendPrintf( cnt, "In user mode : %.3f %%<br>", userload );
  svSendPrintf( cnt, "In kernel mode : %.3f %%<br><br>", kernelload );
  svSendString( cnt, "<b>Server program RAM usage ( current )</b><br>" );
  svSendPrintf( cnt, "Memory used : %d bytes ( %d mb )<br>", stvsize, stvsize >> 20 );
  svSendPrintf( cnt, "Resident set size : %d pages<br><br>", strss );
  svSendString( cnt, "</td></tr></table>" );

  svSendString( cnt, "</td></tr></table>" );


  svSendString( cnt, "</center></body></html>" );

  return;
}


void iohttpFunc_account( svConnectionPtr cnt )
{
  int a, id;
  dbUserMainDef maind;
  dbUserDescDef descd;
  unsigned char *faction, *race, *desc;
  unsigned char description[4096];

  iohttpVarsInit( cnt );
  faction = iohttpVarsFind( "newname" );
  race = iohttpVarsFind( "race" );
  desc = iohttpVarsFind( "desc" );
  iohttpVarsCut();

  iohttpBase( cnt, 1 );
  if( ( id = iohttpIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohttpHeader( cnt, id, &maind ) ) )
    return;

  iohttpBodyInit( cnt, "Account Options" );

  if( !( svTickStatus | svTickNum ) )
  {
    if( faction )
    {
      if( cmdExecChangeName( id, faction ) < 0 )
      {
        if( cmdErrorString )
          svSendPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
        else
          svSendString( cnt, "<i>Error encountered while changing faction name/i><br><br>" );
      }
      else
      {
        svSendString( cnt, "<i>Faction name has been changed</i><br><br>" );
        dbUserMainRetrieve( id, &maind );
      }
    }
    if( race )
    {
      if( ( sscanf( race, "%d", &a ) == 1 ) && ( (unsigned int)a < CMD_RACE_NUMUSED ) )
      {
      	if(((cnt->dbuser)->level >= LEVEL_MODERATOR)||(a != CMD_RACE_ULTI))
      		maind.raceid = a;
        svSendString( cnt, "<i>Race changed</i><br><br>" );
        dbUserMainSet( id, &maind );
      }
    }
  }

  if( desc )
  {
    if( dbUserDescRetrieve( id, &descd ) >= 0 )
    {
      iohttpForumFilter( description, desc, 4096, 0 );
      iohttpForumFilter2( descd.desc, description, 4096 );
      dbUserDescSet( id, &descd );
    }
  }

  svSendString( cnt, "<table border=\"0\"><tr><td>" );
  svSendPrintf( cnt, "User name : <b>%s</b><br>", (cnt->dbuser)->name );
  svSendPrintf( cnt, "Faction name : <b>%s</b><br>", maind.faction );
  svSendPrintf( cnt, "Faction race : <b>%s</b><br>", cmdRaceName[maind.raceid] );
  svSendPrintf( cnt, "Forum tag : <b>%s</b><br>", maind.forumtag );
  svSendPrintf( cnt, "Tag points : <b>%d</b><br>", maind.tagpoints );
  svSendPrintf( cnt, "Account ID : <b>%d</b><br>", id );

  if( !( svTickStatus | svTickNum ) )
  {
    svSendString( cnt, "<br>Change Faction name<br><i>Only available before time starts.</i><br>" );
    svSendPrintf( cnt, "<form action=\"account\"><input type=\"text\" name=\"newname\" size=\"32\" value=\"%s\"><input type=\"submit\" value=\"Change\"></form>", maind.faction );
    svSendString( cnt, "<form action=\"account\">Faction race - <a href=\"races\">See races</a><br><i>Only available before time starts.</i><br><select name=\"race\">" );
    for( a = 0 ; a < CMD_RACE_NUMUSED-1 ; a++ )
    {
      svSendPrintf( cnt, "<option value=\"%d\"", a );
      if( a == maind.raceid )
        svSendString( cnt, " selected" );
      svSendPrintf( cnt, ">%s", cmdRaceName[a] );
    }
    svSendString( cnt, "</select><input type=\"submit\" value=\"Change\"></form>" );
  }

  description[0] = 0;
  if( dbUserDescRetrieve( id, &descd ) >= 0 )
    iohttpForumFilter3( description, descd.desc, 4096 );

  svSendString( cnt, "<form action=\"account\" method=\"POST\"><i>Faction description</i><br>" );
  svSendString( cnt, "<textarea name=\"desc\" wrap=\"soft\" rows=\"4\" cols=\"64\">" );
  svSendString( cnt, description );
  svSendString( cnt, "</textarea><br>" );
  svSendString( cnt, "<input type=\"submit\" value=\"Change\"></form><br>" );

  svSendString( cnt, "<br><a href=\"changepass\">Change your password</a>" );
  svSendString( cnt, "<br><br><a href=\"delete\">Delete your Faction</a>" );
  svSendString( cnt, "</td></tr></table>" );

  iohttpBodyEnd( cnt );
  return;
}


void iohttpFunc_changepass( svConnectionPtr cnt )
{
  int a, b, id;
  dbUserMainDef maind;
  unsigned char oldpass[32];
  unsigned char *newpass[3];

  iohttpVarsInit( cnt );
  newpass[0] = iohttpVarsFind( "password" );
  newpass[1] = iohttpVarsFind( "newpass1" );
  newpass[2] = iohttpVarsFind( "newpass2" );
  iohttpVarsCut();

  iohttpBase( cnt, 1 );
  if( ( id = iohttpIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohttpHeader( cnt, id, &maind ) ) )
    return;

  iohttpBodyInit( cnt, "Change password" );

  if( ( newpass[0] ) && ( newpass[1] ) && ( newpass[2] ) )
  {
    for( b = 0 ; b < 3 ; b++ )
    {
      for( a = 0 ; newpass[b][a] ; a++ )
      {
        if( newpass[b][a] == '+' )
          newpass[b][a] = ' ';
        else if( ( newpass[b][a] == 10 ) || ( newpass[b][a] == 13 ) )
          newpass[b][a] = 0;
      }
    }
    if( dbUserRetrievePassword( id, oldpass ) < 0 )
      svSendString( cnt, "<i>Error encountered when retrieving password.</i><br><br>" );
    else if( !( ioCompareExact( newpass[0], oldpass ) ) )
      svSendString( cnt, "<i>Wrong old password</i><br><br>" );
    else if( !( ioCompareExact( newpass[1], newpass[2] ) ) )
      svSendString( cnt, "<i>Different new passwords? Check your typing.</i><br><br>" );
    else if( cmdExecChangePassword( id, newpass[1] ) < 0 )
    {
      if( cmdErrorString )
        svSendPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
      else
        svSendString( cnt, "<i>Error encountered when changing password.</i><br><br>" );
    }
    else
      svSendString( cnt, "<i>Password has been changed.</i><br><br>" );
  }

  svSendString( cnt, "<form action=\"changepass\" method=\"POST\">Current Password:<br><input type=\"text\" name=\"password\" size=\"16\"><br><br>" );
  svSendString( cnt, "New Password:<br><input type=\"text\" name=\"newpass1\" size=\"16\"><br><br>" );
  svSendString( cnt, "Repeat New Password:<br><input type=\"text\" name=\"newpass2\" size=\"16\"><br><br>" );
  svSendString( cnt, "<input type=\"submit\" name=\"Change\"></center>" );

  iohttpBodyEnd( cnt );
  return;
}


void iohttpFunc_delete( svConnectionPtr cnt )
{
  int a, b, c, id;
  dbUserMainDef maind;
  unsigned char *deletestring, *deathstring;

  iohttpVarsInit( cnt );
  deletestring = iohttpVarsFind( "delete" );
  deathstring = iohttpVarsFind( "death" );
  iohttpVarsCut();

  iohttpBase( cnt, 1 );
  if( ( id = iohttpIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohttpHeader( cnt, id, &maind ) ) )
    return;

  iohttpBodyInit( cnt, "Delete faction" );

  c = 3600*48;
  if( !( svTickStatus | svTickNum ) )
    c = 120;
  a = time( 0 );
  if( maind.createtime+c > a )
  {
    b = maind.createtime+c - a;
    svSendPrintf( cnt, "You must wait 48 hours after the creation of an account to delete it, or 2 minutes if time has not started yet. There are %d hours and %d minutes left.<br><br>", b/3600, (b/60)%60 );
    iohttpBodyEnd( cnt );
    return;
  }
  
        // SK: you can't delete when your empire is at war with someone or if you are declaring war on someone.
//      printf("---starting the check for deleting when in war.\n");
     
     int counterRelations, maxRelations, curfam;
     dbMainEmpireDef empired;
     unsigned char *empirestring;
     int *rel;
     int warCounter = 0;
     
     iohttpVarsInit( cnt );
     empirestring = iohttpVarsFind( "id" );
     iohttpVarsCut();
     
//     printf("printing empirestring\n");
//     printf("%u\n",empirestring);
     
     if( !( empirestring ) || ( sscanf( empirestring, "%d", &curfam ) <= 0 ) )
      curfam = maind.empire;
     
     if( dbMapRetrieveEmpire( curfam, &empired ) < 0 )
     {
      svSendString( cnt, "This empire does not seem to exist!</body></html>" );
      return;
     }
     
     if( ( maxRelations = dbEmpireRelsList( curfam, &rel ) ) >= 0 )
     {
               maxRelations <<= 2;
      for( counterRelations = 0 ; counterRelations < maxRelations ; counterRelations += 4 )
      {
       if( rel[counterRelations+3] & 1 )
        continue;
       else if( rel[counterRelations+1] == CMD_RELATION_WAR )
        warCounter++;
      }
      for( counterRelations = 0 ; counterRelations < maxRelations ; counterRelations += 4 )
      {
       if( !( rel[counterRelations+3] & 1 ) )
        continue;
       else if( rel[counterRelations+1] == CMD_RELATION_WAR )
        warCounter++;
      }
     }
//     printf("printing warcounter\n");
//      printf("%d\n",warCounter);
      if( warCounter > 0 )
      {
             svSendPrintf( cnt, "You cannot delete when your empire is at war.<br><br>");
        iohttpBodyEnd( cnt );
            return;
      }
     
//     printf("---ending the check for deleting when in war.\n");

  
  //cant delete for 24hrs after round start
 // if(((svTickNum < 144)&&(svTickNum > 0))||(svTickNum))
 // {
 // 	svSendPrintf(cnt, "You must wait 24 hours after round start to delete your account. There are %d hours and %d minutes left.<br><br>", 23 - svTickNum/6, 59-(svTickNum%6)*10);
 // 	iohttpBodyEnd( cnt );
 //   return;
 // }

  if( deletestring )
  {
    a = 0;
    sscanf( deletestring, "%d", &a );
    if( ( a == 0 ) && ( cmdUserDelete( id ) >= 0 ) )
      svSendPrintf( cnt, "Faction and account deleted!<br>" );
    else if( ( a == 1 ) && ( cmdExecUserDeactivate( id, 0 ) >= 0 ) )
      svSendPrintf( cnt, "Account deactivated!<br>" );
    else
      svSendPrintf( cnt, "Error encountered while deleting faction<br>" );
    iohttpBodyEnd( cnt );
    return;
  }

  svSendString( cnt, "<form action=\"delete\" method=\"POST\"><table><tr><td>Please specify the kind of deletion<br><select name=\"delete\">" );
  svSendString( cnt, "<option value=\"1\">Keep the account to join a different empire" );
  svSendString( cnt, "<option value=\"0\">Delete the faction and the account" );
  svSendString( cnt, "</select><br><br>How should your emperor career end?<br><select name=\"death\">" );
  svSendString( cnt, "<option value=\"0\">I retired and lived happily ever after." );
  svSendString( cnt, "<option value=\"1\">I slipped on a soap bar." );
  svSendString( cnt, "<option value=\"2\">I mixed the television and the auto-destruction device remotes." );
  svSendString( cnt, "<option value=\"3\">I got into an argument with my pet Lynx." );
  svSendString( cnt, "<option value=\"4\">I caught my tie in the blender." );
  svSendString( cnt, "<option value=\"5\">A chicken bone got stuck in my throat." );
  svSendString( cnt, "</select><br><br><input type=\"submit\" value=\"Delete\"></td></tr></table></form>" );

  iohttpBodyEnd( cnt );
  return;
}

void iohttpFunc_logout( svConnectionPtr cnt )
{
  FILE *file;
  struct stat stdata;
  unsigned char *data;

  svSendPrintf( cnt, "Set-Cookie: USRID=%04x%04x%04x%04x%04x; path=/\n", 0, 0, 0, 0, 0 );
  svSendString( cnt, "Content-Type: text/html\n\n" );

  if( stat( IOHTTP_READ_DIRECTORY "/front.html", &stdata ) != -1 )
  {
    if( ( data = malloc( stdata.st_size + 1 ) ) )
    {
      data[stdata.st_size] = 0;
      if( ( file = fopen( IOHTTP_READ_DIRECTORY "/front.html", "rb" ) ) )
      {
        fread( data, 1, stdata.st_size, file );
        svSendString( cnt, data );
        fclose( file );
      }
      free( data );
    }
  }

  return;
}

#include "admin.c"
