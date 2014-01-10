


void iohtmlFunc_account( ReplyDataPtr cnt ) {
	int a, id;
	char *faction, *race, *desc;
	char description[USER_DESC_SIZE];
	dbUserMainDef maind;
	dbUserInfoDef infod;

faction = iohtmlVarsFind( cnt, "newname" );
race = iohtmlVarsFind( cnt, "race" );
desc = iohtmlVarsFind( cnt, "desc" );

iohtmlBase( cnt, 1 );

if( ( id = iohtmlIdentify( cnt, 1 ) ) < 0 )
	return;
if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

if( !( dbUserInfoRetrieve( id, &infod ) ) ) {
	error( "Error in user html info, getting real info" );
	return;
}

iohtmlBodyInit( cnt, "Account Options" );

if( !( ticks.status | ticks.number ) ) {
	if( faction ) {
		if( cmdExecChangeName( id, faction ) < 0 ) {
			if( cmdErrorString )
				httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
			else
				httpString( cnt, "<i>Error encountered while changing faction name</i><br><br>" );
		} else {
			httpString( cnt, "<i>Faction name has been changed</i><br><br>" );
			dbUserMainRetrieve( id, &maind );
		}
	}
	if( race ) {
		if( ( sscanf( race, "%d", &a ) == 1 ) && ( (unsigned int)a < CMD_RACE_NUMUSED ) ) {
			maind.raceid = a;
			httpString( cnt, "<i>Race changed</i><br><br>" );
			dbUserMainSet( id, &maind );
		}
	}
}

if( desc ) {
	iohttpForumFilter( description, desc, USER_DESC_SIZE, 0 );
	iohttpForumFilter2( infod.desc, description, USER_DESC_SIZE );
	if( dbUserInfoSet( id, &infod ) )
      		httpString( cnt, "<i>Description Updated</i><br><br>" );
	else
		httpString( cnt, "<i>Error encountered while changing Description</i><br><br>" );
}

httpString( cnt, "<table border=\"0\"><tr><td>" );
httpPrintf( cnt, "Account ID : <b>%d</b><br>", id );
httpPrintf( cnt, "User name : <b>%s</b><br>", ((cnt->session)->dbuser)->name );
httpPrintf( cnt, "Faction name : <b>%s</b><br>", maind.faction );
httpPrintf( cnt, "Faction race : <b>%s</b><br>", cmdRaceName[maind.raceid] );
httpPrintf( cnt, "Tag points : <b>%d</b><br>", infod.tagpoints );

httpPrintf( cnt, "Forum tag : <b>%s</b><br>", infod.forumtag );

if( !( ticks.status | ticks.number ) ) {
	httpString( cnt, "<br>Change Faction name<br><i>Only available before time starts.</i><br>" );
	httpPrintf( cnt, "<form action=\"account\"><input type=\"text\" name=\"newname\" size=\"32\" value=\"%s\"><input type=\"submit\" value=\"Change\"></form>", maind.faction );
	httpString( cnt, "<form action=\"account\">Faction race - <a href=\"races\">See races</a><br><i>Only available before time starts.</i><br><select name=\"race\">" );

	for( a = 0 ; a < CMD_RACE_NUMUSED-1 ; a++ ) {
		httpPrintf( cnt, "<option value=\"%d\"", a );
		if( a == maind.raceid )
			httpString( cnt, " selected" );
		httpPrintf( cnt, ">%s", cmdRaceName[a] );
	}
	httpString( cnt, "</select><input type=\"submit\" value=\"Change\"></form>" );
}

iohttpForumFilter3( description, infod.desc, USER_DESC_SIZE );
httpString( cnt, "<br>" );
httpString( cnt, "<form action=\"account\" method=\"POST\"><i>Faction description</i><br>" );
httpString( cnt, "<textarea name=\"desc\" wrap=\"soft\" rows=\"4\" cols=\"64\">" );
httpString( cnt, description );
httpString( cnt, "</textarea><br>" );
httpString( cnt, "<input type=\"submit\" value=\"Change\"></form><br>" );
#if FACEBOOK_SUPPORT
if( -timediff( *localtime( &infod.fbinfo.updated ) ) >= day ) {
	facebook_update_user( (cnt->session)->dbuser );
}
iohtmlFBConnect( cnt );
if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBLINK ) ) {
httpString( cnt, "<table><tr><td>" );
httpPrintf( cnt, "<img src=\"%s\" alt=\"Facebook Icon\">", infod.fbinfo.pic );
httpString( cnt, "</td><td>" );
	httpPrintf( cnt, "Facebook ID : <b>%s</b><br>", infod.fbinfo.id );
	httpPrintf( cnt, "Facebook Name : <b>%s</b><br>", infod.fbinfo.full_name );
httpString( cnt, "</td></tr></table>" );
}
#endif

httpString( cnt, "<br><a href=\"changepass\">Change your password</a>" );
httpString( cnt, "<br><br><a href=\"delete\">Delete your Faction</a>" );
httpString( cnt, "</td></tr></table>" );


iohtmlBodyEnd( cnt );
return;
}


void iohtmlFunc_changepass( ReplyDataPtr cnt )
{
  int a, b, id;
  dbUserMainDef maind;
  char oldpass[USER_PASS_MAX];
  char *newpass[3];


  newpass[0] = iohtmlVarsFind( cnt, "password" );
  newpass[1] = iohtmlVarsFind( cnt, "newpass1" );
  newpass[2] = iohtmlVarsFind( cnt, "newpass2" );

  iohtmlBase( cnt, 1 );
  if( ( id = iohtmlIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohtmlHeader( cnt, id, &maind ) ) )
    return;

  iohtmlBodyInit( cnt, "Change password" );

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
      httpString( cnt, "<i>Error encountered when retrieving password.</i><br><br>" );
    else if( !( checkencrypt( newpass[0], oldpass ) ) )
      httpString( cnt, "<i>Wrong old password</i><br><br>" );
    else if( !( ioCompareExact( newpass[1], newpass[2] ) ) )
      httpString( cnt, "<i>Different new passwords? Check your typing.</i><br><br>" );
    else if( cmdExecChangePassword( id, newpass[1] ) < 0 )
    {
      if( cmdErrorString )
        httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
      else
        httpString( cnt, "<i>Error encountered when changing password.</i><br><br>" );
    }
    else
      httpString( cnt, "<i>Password has been changed.</i><br><br>" );
  }

  httpString( cnt, "<form action=\"changepass\" method=\"POST\">Current Password:<br><input type=\"password\" name=\"password\" size=\"16\"><br><br>" );
  httpString( cnt, "New Password:<br><input type=\"password\" name=\"newpass1\" size=\"16\"><br><br>" );
  httpString( cnt, "Repeat New Password:<br><input type=\"password\" name=\"newpass2\" size=\"16\"><br><br>" );
  httpString( cnt, "<input type=\"submit\" name=\"Change\"></center>" );

  iohtmlBodyEnd( cnt );
  return;
}



void iohtmlFunc_delete( ReplyDataPtr cnt )
{
  int a, b, c, id;
  dbUserMainDef maind;
  dbUserInfoDef infod;
  char *deletestring/*, *deathstring*/;

  deletestring = iohtmlVarsFind( cnt, "delete" );
  //deathstring = iohtmlVarsFind( cnt, "death" );

  iohtmlBase( cnt, 1 );
  if( ( id = iohtmlIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohtmlHeader( cnt, id, &maind ) ) )
    return;
if( !( dbUserInfoRetrieve( id, &infod ) ) ) {
	error( "Error in user html delete, getting real info" );
	return;
}
  iohtmlBodyInit( cnt, "Delete faction" );

  c = 3600*48;
  if( !( ticks.status | ticks.number ) )
    c = 120;
  a = time( 0 );
  if( infod.createtime+c > a )
  {
    b = infod.createtime+c - a;
    httpString( cnt, "You must wait 48 hours after the creation of an account to delete it, or 2 minutes if time has not started yet.<br>" );
    httpPrintf( cnt, "<b>There are %d hours and %d minutes left!</b><br><br>", b/3600, (b/60)%60 );
    iohtmlBodyEnd( cnt );
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
      httpPrintf( cnt, "Faction and account deleted!<br>" );
    else if( ( a == 1 ) && ( cmdExecUserDeactivate( id, 0 ) >= 0 ) )
      httpPrintf( cnt, "Account deactivated!<br>" );
    else
      httpPrintf( cnt, "Error encountered while deleting faction<br>" );
    iohtmlBodyEnd( cnt );
    return;
  }

  httpString( cnt, "<form action=\"delete\" method=\"POST\"><table><tr><td>Please specify the kind of deletion<br><select name=\"delete\">" );
  httpString( cnt, "<option value=\"1\">Keep the account to join a different empire" );
  httpString( cnt, "<option value=\"0\">Delete the faction and the account" );
  httpString( cnt, "</select><br><br>How should your emperor career end?<br><select name=\"death\">" );
  httpString( cnt, "<option value=\"0\">I retired and lived happily ever after." );
  httpString( cnt, "<option value=\"1\">I slipped on a soap bar." );
  httpString( cnt, "<option value=\"2\">I mixed the television and the auto-destruction device remotes." );
  httpString( cnt, "<option value=\"3\">I got into an argument with my pet Lynx." );
  httpString( cnt, "<option value=\"4\">I caught my tie in the blender." );
  httpString( cnt, "<option value=\"5\">A chicken bone got stuck in my throat." );
  httpString( cnt, "</select><br><br><input type=\"submit\" value=\"Delete\"></td></tr></table></form>" );

  iohtmlBodyEnd( cnt );
  return;
}


void iohtmlFunc_logout( ReplyDataPtr cnt ) {

if( remove_session( (cnt->session)->sid ) == 0 ) {
	critical( "Unable to remove user session, this really shoulden't be able to happen." );
	if( ( cnt->session ) )
		cnt->session = NULL;
	iohtmlFunc_front( cnt, "%s", "An error, has occured with the logout. To be safe please close your browser.!" );
} else {
	cnt->session = get_session( SESSION_HTTP, NULL );
	iohtmlFunc_front( cnt, "%s", "You have safley loged out!" );
}

return;
}


