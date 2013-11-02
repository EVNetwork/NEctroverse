
void iohttpFunc_account( svConnectionPtr cnt )
{
  int a, id;
  dbUserMainDef maind;
  dbUserDescDef descd;
  char *faction, *race, *desc;
  char description[4096];

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

  if( !( ticks.status | ticks.number ) )
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

  if( !( ticks.status | ticks.number ) )
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
  char oldpass[128];
  char *newpass[3];


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
    else if( !( checkencrypt( newpass[0], oldpass ) ) )
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

  svSendString( cnt, "<form action=\"changepass\" method=\"POST\">Current Password:<br><input type=\"password\" name=\"password\" size=\"16\"><br><br>" );
  svSendString( cnt, "New Password:<br><input type=\"password\" name=\"newpass1\" size=\"16\"><br><br>" );
  svSendString( cnt, "Repeat New Password:<br><input type=\"password\" name=\"newpass2\" size=\"16\"><br><br>" );
  svSendString( cnt, "<input type=\"submit\" name=\"Change\"></center>" );

  iohttpBodyEnd( cnt );
  return;
}


void iohttpFunc_delete( svConnectionPtr cnt )
{
  int a, b, c, id;
  dbUserMainDef maind;
  char *deletestring, *deathstring;

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
  if( !( ticks.status | ticks.number ) )
    c = 120;
  a = time( 0 );
  if( maind.createtime+c > a )
  {
    b = maind.createtime+c - a;
    svSendString( cnt, "You must wait 48 hours after the creation of an account to delete it, or 2 minutes if time has not started yet.<br>" );
    svSendPrintf( cnt, "<b>There are %d hours and %d minutes left!</b><br><br>", b/3600, (b/60)%60 );
    iohttpBodyEnd( cnt );
    return;
  }
  

  //cant delete for 24hrs after round start
 // if(((ticks.number < 144)&&(ticks.number > 0))||(ticks.number))
 // {
 // 	svSendPrintf(cnt, "You must wait 24 hours after round start to delete your account. There are %d hours and %d minutes left.<br><br>", 23 - ticks.number/6, 59-(ticks.number%6)*10);
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

void iohttpFunc_logout( svConnectionPtr cnt ) {


svSendPrintf( cnt, "Set-Cookie: USRID=%04x%04x%04x%04x%04x; path=/\n", 0, 0, 0, 0, 0 );

iohttpFunc_front( cnt, "You have safley loged out!" );
  

return;
}


