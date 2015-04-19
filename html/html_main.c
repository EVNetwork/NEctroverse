

int iohtmlIdentify( ReplyDataPtr cnt, int action ) {
	char sess[SESSION_SIZE];
	dbUserPtr user;

if( ( NULL == (cnt->session)->dbuser ) || ( NULL == (cnt->session)->sid ) ) {
	goto FAIL;
} else {
	user = (cnt->session)->dbuser;
}

if( dbSessionRetrieve( user, sess ) < 0 ) {
	goto FAIL;
}

if( ( sess == NULL ) || ( user == NULL ) ) {
	goto FAIL;
}

if( ( user->id < 0 ) || ( user->id > dbUserNum ) ) {
	goto FAIL;
}

if( strcmp( (cnt->session)->sid, sess ) ) {
	goto FAIL;
}

if( !( action & 16 ) ) {
	user->lasttime = time(NULL);
}

if( dbUserSave( user->id, user ) < 0 ) {
	error( "Database UserSave" );
	goto NEGATIVE;
}
if( action & 8 ) {
	goto RETURN;
}

if( ( action & 2 ) ) {
	if( !( bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED ) ) && ( user->level < LEVEL_MODERATOR ) ) {
		if( action & 1 ) {
			iohtmlFunc_register( cnt );
		}
		goto NEGATIVE;
	}
}

if( action & 4 ) {
	if( ( bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
		if( action & 1 ) {
			iohtmlFunc_register( cnt );
		}
		goto NEGATIVE;
	}
}
RETURN:
purge_captcha( cnt->session );
return user->id;

FAIL:
if( action & 1 ) {
	iohtmlFunc_expired( cnt );
}

NEGATIVE:
return -1;
}

void iohtmlBase( ReplyDataPtr cnt, int flags ) {
	ConfigArrayPtr settings[2];

settings[0] = GetSetting( "Server Name" );

httpString( cnt, "<!DOCTYPE html><html dir=\"ltr\" lang=\"en\">");
httpString( cnt, "<head>");
httpString( cnt, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" );
httpPrintf( cnt, "<title>%s</title>", settings[0]->string_value );
httpString( cnt, "<link rel=\"icon\" href=\"files?type=image&amp;name=favicon.ico\">" );
if( !( flags & 32 ) ) {
	httpPrintf( cnt, "<link href=\"%s&amp;type=server&amp;name=style.css\" rel=\"stylesheet\" type=\"text/css\" media=\"screen\">", URLAppend( cnt, "files" ) );
	httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s&amp;type=server&amp;name=jquery-1.11.0.min.js\"></script>", URLAppend( cnt, "files" ) );
	httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s\"></script>", URLAppend( cnt, "ajax.js" ) );
	if( flags & 16 ) {
		httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s&amp;type=server&amp;name=status.min.js\"></script>", URLAppend( cnt, "files" ) );
	}
	httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s&amp;type=server&amp;name=javascript.js\"></script>", URLAppend( cnt, "files" ) );
}
if( flags & 4 ) {
	httpString( cnt, "<base target=\"_blank\">" );
}
if( ( flags & 1 ) && !( flags & 32 ) ) {
	httpString( cnt, "<style type=\"text/css\">" );
	httpString( cnt, "body{background-image:url(files?type=image&name=background.gif);" );
	if( !( flags & 2 ) ) {
		httpString( cnt, "background-attachment:fixed;" );
	}
	httpString( cnt, "}</style>" );
}

httpString( cnt, "</head>" );
httpString( cnt, "<script type=\"text/javascript\">" );
httpString( cnt, "  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){" );
httpString( cnt, "  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o)," );
httpString( cnt, "  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)" );
httpString( cnt, "  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');" );
settings[0] = GetSetting( "Google Analytics Domain" );
settings[1] = GetSetting( "Google Analytics ID" );
httpPrintf( cnt, "  ga('create', '%s', '%s');", settings[1]->string_value, settings[0]->string_value );
httpString( cnt, "  ga('send', 'pageview');" );
httpString( cnt, "  ga('send', 'pageview');" );
httpString( cnt, "</script>" );

if( flags & 1 ) {
	httpString( cnt, "<body style=\"margin:auto;\">" );
}

httpString( cnt, "<center>" );

return;
}


void iohtmlFunc_frontmenu( ReplyDataPtr cnt, int flags ) {
	urlinfoPtr urlp;
	int port;
	char *referal;

referal = iohtmlHeaderFind(cnt, "Referer");
urlp = parse_url( referal );


httpString( cnt, "<table class=\"center\" cellspacing=\"0\" cellpadding=\"10\" style=\"width:100%;border-width:0;\"><tbody><tr><td class=\"bodyline\">" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td align=\"center\"><img src=\"files?type=image&amp;name=n1.jpg\" alt=\"NEctroverse Logo\"></td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_12.jpg\" align=\"center\">" );
httpString( cnt, "<table width=\"90%\" height=\"75\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_12.jpg\" valign=\"middle\">" );
httpString( cnt, "<table style=\"width:100%;border-width:0;\"><tr>" );
httpPrintf( cnt, "<td align=\"center\"><span id=\"u_online\">%d</span> of <span id=\"u_activated\">%d</span> players online</td>", dbRegisteredInfo[DB_TOTALS_USERS_ONLINE], dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] );
httpString( cnt, "<td align=\"center\"><b>" );

if( !( flags == FMENU_MAIN ) ) {
	httpPrintf( cnt, "<a href=\"%s\">Main</a>", URLAppend( cnt, "index" ) );
}
if( !( flags == FMENU_REGISTER ) ) {
	if( !( flags == FMENU_MAIN ) ) {
		httpString( cnt, " | " );
	}
	httpPrintf( cnt, "<a href=\"%s\">%s</a>", URLAppend( cnt, ( ( (cnt->session)->dbuser ) ? ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ? "hq" : "register" ) : "register" ) ), ( ( (cnt->session)->dbuser ) ? ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ? "Game" : "Activate" ) : "Register" ) );
}
if( !( flags == FMENU_FORUM ) ) {
	httpString( cnt, " | " );
	httpPrintf( cnt, "<a href=\"%s\">Forums</a>", URLAppend( cnt, "forum" ) );
}
if( !( flags == FMENU_FAQ ) ) {
	httpString( cnt, " | " );
	httpPrintf( cnt, "<a href=\"%s\">FAQ</a>", URLAppend( cnt, "faq" ) );
}
if( !( flags == FMENU_GSTART ) ) {
	httpString( cnt, " | " );
	httpPrintf( cnt, "<a href=\"%s\">Getting Started</a>", URLAppend( cnt, "gettingstarted" ) );
}
if( !( flags == FMENU_RANKS ) ) {
	httpString( cnt, " | " );
	httpPrintf( cnt, "<a href=\"%s\">Hall of fame</a>", URLAppend( cnt, "halloffame" ) );
}
if( !( flags == FMENU_SERVER ) ) {
	httpString( cnt, " | " );
	httpPrintf( cnt, "<a href=\"%s\">Server Status</a>", URLAppend( cnt, "status" ) );
}
sscanf( urlp->port, "%d", &port );
if( port != options.port[PORT_HTTP] ) {
	httpString( cnt, " | " );
	httpPrintf( cnt, "<a href=\"%s\">Portal</a>", URLAppend( cnt, "/" ) );
}

httpString( cnt, "</b></td></tr></table></td></tr>" );

httpString( cnt, "</table>" );
httpString( cnt, "</td></tr></table>" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td>&nbsp;</td></tr>" );

urlinfo_free( urlp );
return;
}

void html_boxstart( ReplyDataPtr cnt, bool type, char *title, ... ) {
	char text[ARRAY_MAX];
	va_list ap;

va_start( ap, title );
vsnprintf( text, ARRAY_MAX, title, ap );
va_end( ap );

httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\" background=\"files?type=image&amp;name=i27.jpg\"><tr><td width=\"22\" height=\"24\" background=\"files?type=image&amp;name=i25.jpg\" ></td><td align=\"center\" nowrap><b><font face=\"verdana\" size=\"2\">" );
httpPrintf( cnt, "%s", text);
httpPrintf( cnt, "</font></b></td><td align=\"right\" background=\"files?type=image&amp;name=i30.jpg\" height=\"24\" width=\"62\"></td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"7\" background=\"files?type=image&amp;name=i38.jpg\">&nbsp;</td><td bgcolor=\"#0b1119\" align=\"%s\"><br>", ( type ? "left" : "center" ) );

return;
}

void html_boxend( ReplyDataPtr cnt ) {

httpString( cnt, "<br><br></td><td width=\"7\" background=\"files?type=image&amp;name=i43.jpg\">&nbsp;</td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"62\"><img height=\"12\" src=\"files?type=image&amp;name=i45.jpg\" width=\"62\"></td><td width=\"100%\" background=\"files?type=image&amp;name=i47.jpg\"><img height=\"12\" src=\"files?type=image&amp;name=i47.jpg\" width=\"1\"></td><td width=\"62\"><img height=\"12\" src=\"files?type=image&amp;name=i49.jpg\" width=\"62\"></td></tr></table>" );

return;
}


void iohtmlBodyEnd( ReplyDataPtr cnt ) {

html_boxend( cnt );
httpString( cnt, "</td></tr></table></center><br><br><br></body></html>" );

return;
}

void iohtmlFunc_endhtml( ReplyDataPtr cnt ) {

httpString( cnt, "</td></tr></tbody></table>" );
httpString( cnt, "</body></html>" );

return;
}


void iohtmlFunc_front( ReplyDataPtr cnt, char *text, ...  ) {
	ConfigArrayPtr settings[2];
	dbUserMainDef maind;
	#if FACEBOOK_SUPPORT
	dbUserInfoDef infod;
	#endif
	struct stat stdata;
	bool boxopen = false;
	char *data;
	char DIRCHECKER[PATH_MAX], DATAPOOL[DEFAULT_BUFFER];
	FILE *file;
	int id, len, notices;
	va_list ap;

len = notices = 0;

if( text != NULL ) {
	va_start( ap, text );
	len = vsnprintf( DATAPOOL, DEFAULT_BUFFER, text, ap );
	va_end( ap );
	text = DATAPOOL;
}


iohtmlBase( cnt, 8 );

if( ( id = iohtmlIdentify( cnt, 0 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 ) {
		return;
	}
}

#if FACEBOOK_SUPPORT
iohtmlFBSDK( cnt );
#endif

iohtmlFunc_frontmenu( cnt, FMENU_MAIN );

if( len > 0 ) {
	httpPrintf( cnt, "<h3>%s</h3>", text );
}

httpString( cnt, "<td valign=\"top\">" );

//read notices from notices.txt and format for display. -- If this file is missing, or empty it is skipped.
settings[0] = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/notices.txt", settings[0]->string_value );
settings[1] = GetSetting( "Display Notices" );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( data[0] == '#' ) {
						continue;
					}
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), DEFAULT_BUFFER );
						html_boxstart( cnt, true, DATAPOOL );
						boxopen = true;
						notices++;
					} else if ( strlen( trimwhitespace(data) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), DEFAULT_BUFFER );
						httpPrintf( cnt, "&nbsp;&nbsp;%s<br>", DATAPOOL );
					}
					if( (boxopen) && ( strlen( trimwhitespace(data) ) == false ) ) {
						html_boxend( cnt );
						boxopen = false;
						httpString( cnt, "<br><br>" );
						if( notices == (int)settings[1]->num_value ) {
							httpString( cnt, "<table align=\"right\"><tr><td width=\"40%\" valign=\"top\">" );
							httpPrintf( cnt, "<a href=\"%s&amp;request=true\" rel=\"ajaxpanel\" data-loadtype=\"ajax\">See full list...</a>", URLAppend( cnt, "notices" ) );
							httpString( cnt, "</td></tr></table>" );
							break;
						}
					}
				}
			if( boxopen ) {
				html_boxend( cnt );
			}
			httpString( cnt, "<br>" );
			httpString( cnt, "<br>" );
			}
			fclose( file );
		}
	free( data );
	}
}
//end notices

httpString( cnt, "</td><td width=\"4%\">" );
httpString( cnt, "&nbsp;" );
httpString( cnt, "</td><td width=\"40%\" valign=\"top\">" );

httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\" class=\"left\"><font color=\"#FFFFFF\" size=\"2\"><b>Log in</b></font></td></tr>" );
httpString( cnt, "<tr><td>" );
httpString( cnt, "<table cellspacing=\"8\"><tr><td>" );

if( (id < 0) ) {
	httpPrintf( cnt, "<font size=\"2\"><form action=\"%s\" method=\"POST\">", URLAppend( cnt, "login" ) );
	httpString( cnt, "Name<br><input type=\"text\" name=\"name\" size=\"24\"><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "Password<br><input type=\"password\" name=\"pass\" size=\"24\"><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<input type=\"submit\" value=\"Log in\"></form>" );
} else {
	#if FACEBOOK_SUPPORT
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBMADE ) ) {
		dbUserInfoRetrieve( id, &infod );
		httpPrintf( cnt, "<br><b>You are already logged in as <i>%s</i></b><br>", infod.fbinfo.full_name );
	} else
	#endif
	httpPrintf( cnt, "<br><b>You are already logged in as <i>%s</i></b><br>", ((cnt->session)->dbuser)->name );
	httpString( cnt, "<br>" );
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) {
		httpPrintf( cnt, "<a href=\"%s\">Proceed to game</a><br>", URLAppend( cnt, "hq" ) );
	} else {
		httpPrintf( cnt, "<a href=\"%s\">Activate Now!</a><br>", URLAppend( cnt, "register" ) );
	}
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s\">Log out</a><br>", URLAppend( cnt, "logout" ) );
	httpString( cnt, "<br>" );
	httpString( cnt, "<br>" );
}

#if FACEBOOK_SUPPORT
httpString( cnt, "</td><td>&nbsp;</td><td valign=\"bottom\">" );
if( ((cnt->session)->dbuser) ) {
	dbUserInfoRetrieve( ((cnt->session)->dbuser)->id, &infod );
	if( -timediff( *localtime( &infod.fbinfo.updated ) ) >= day ) {
		facebook_update_user( (cnt->session)->dbuser );
	}
}
iohtmlFBConnect( cnt );
if( (id >= 0) ) {
	httpString( cnt, "<br>" );
}
#endif

httpString( cnt, "</td></table></td></tr>" );

//read the todo list from todo.txt and format for display. -- If this file is missing, or empty it is skipped.
sprintf( DIRCHECKER, "%s/todo.txt", settings[0]->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				httpString( cnt, "<tr><td>&nbsp;</td></tr>" );
				httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\" class=\"left\">" );
				httpString( cnt, "<font color=\"#FFFFFF\" size=\"2\"><i>Items on the admins to-do list :</i></font></td></tr>" );
				httpString( cnt, "<tr><td>" );
				httpString( cnt, "<table cellspacing=\"8\"><tr><td align=\"left\">" );
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( data[0] == '#' ) {
						continue;
					}
					if( strlen(data) > 1 ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), DEFAULT_BUFFER );
						httpPrintf( cnt, "&nbsp;&#9734;&nbsp;&nbsp;%s<br>", DATAPOOL );
					}
				}
				httpString( cnt, "</td></tr></table></td></tr>" );
			}
			fclose( file );
		}
		free( data );
	}
}

//end todo list
httpString( cnt, "<tr><td>&nbsp;</td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\" class=\"center\"><font color=\"#FFFFFF\" size=\"2\">Adverts Below</font></td></tr>" );
httpString( cnt, "<tr><td>" );
httpString( cnt, "<script async src=\"//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js\"></script>\n" );
httpString( cnt, "<!-- AutoSize -->\n" );
httpString( cnt, "<ins class=\"adsbygoogle\"\n" );
httpString( cnt, "     style=\"display:block\"\n" );
httpString( cnt, "     data-ad-client=\"ca-pub-4553701062747705\"\n" );
httpString( cnt, "     data-ad-slot=\"2637942730\"\n" );
httpString( cnt, "     data-ad-format=\"auto\"></ins>\n" );
httpString( cnt, "<script>\n" );
httpString( cnt, "(adsbygoogle = window.adsbygoogle || []).push({});\n" );
httpString( cnt, "</script>\n" );

httpString( cnt, "</td></tr>" );

httpString( cnt, "</table>" );
httpString( cnt, "<td width=\"5%\">&nbsp;</td>" );

iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_faq( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
	struct stat stdata;
	char *data;
	char DIRCHECKER[PATH_MAX];
	FILE *file;

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_FAQ );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );

httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Frequently Asked Question</b></font></td></tr>" );
httpString( cnt, "<tr><td class=\"left\">" );
settings = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/faq.html", settings->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( ( fread( data, 1, stdata.st_size, file ) < 1 ) && ( stdata.st_size ) ) {
				error( "Failure reading faq file." );
			} else {
				httpString( cnt, trimwhitespace(data) );
			}
			fclose( file );
		}
		free( data );
	}
}
httpString( cnt, "</td></tr></table><br><br>" );
httpString( cnt, "</td><td width=\"7%\">&nbsp;</td></tr></table>" );
iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_gettingstarted( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_GSTART );

settings = GetSetting( "Server Name" );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );


httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpPrintf( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Getting started in the galaxy of %s</b></font></td></tr>", settings->string_value );
httpString( cnt, "<tr><td class=\"left\"><font size=\"2\">" );
httpString( cnt, "<center>This page is a basic guide to new players on how to get started.<br>It is important to read this to get the best possible experience when first playing!</center>" );

httpString( cnt, "<br><b>Creating an account:</b><br>" );
httpString( cnt, "<a href=\"#a0\">0. Registering the account.</a><br>" );
httpString( cnt, "<a href=\"#a1\">1. The user name and faction name.</a><br>" );
httpString( cnt, "<a href=\"#a2\">2. Joining an empire.</a><br>" );
httpString( cnt, "<a href=\"#a3\">3. Choosing your race.</a><br>" );
httpString( cnt, "<a href=\"#a4\">4. Completion and logging in.</a><br>" );
httpString( cnt, "</a><br>" );

httpString( cnt, "<br><b>Playing the game:</b><br>" );
httpString( cnt, "<a href=\"#b0\">0. Resources & Buildings.</a><br>" );
httpString( cnt, "<a href=\"#b1\">1. Planets.</a><br>" );
httpString( cnt, "<a href=\"#b2\">2. Research.</a><br>" );
httpString( cnt, "<a href=\"#b3\">3. Military.</a><br>" );
httpString( cnt, "</a><br>" );

httpString( cnt, "<br><b>Tips:</b><br>" );
httpString( cnt, "<a href=\"#c0\">0. Map generation.</a><br>" );
httpString( cnt, "<a href=\"#c1\">1. Fleet page.</a><br>" );
httpString( cnt, "<a href=\"#c2\">2. Account page.</a><br>" );
httpString( cnt, "</a><br>" );

httpString( cnt, "</td></tr></table><br><br>" );


httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\" class=\"left\">" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Creating an account:</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\"><br>" );

httpString( cnt, "<a name=\"a0\"><b><i>0. Registering the account.</i></b></a><br>" );
httpPrintf( cnt, "The great thing about %s is that the signup is simple. You won't need to input your e-mail address and no other activation is required.", settings->string_value );
#if FACEBOOK_SUPPORT
httpString( cnt, "<br>Facebook support is enabled too, so you can even use your Facebook account to signup and play!" );
#endif
httpString( cnt, "<br><br>" );

httpString( cnt, "<a name=\"a1\"><b><i>1. The user name and faction name.</i></b></a><br>" );
httpString( cnt, "Your user name is something personal. Other players won't see this.<br>" );
httpString( cnt, "Your Faction name however, is your handle in the game. Choose carefully.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "<a name=\"a2\"><b><i>2. Joining an Empire.</i></b></a><br>" );
httpString( cnt, "Being part of an Empire is one of the most important aspects of the game.<br>" );
httpString( cnt, "You start out small and will need other players to grow.<br>" );
httpString( cnt, "Know anyone in the game? Ask them for their empire number and password.<br>" );
httpString( cnt, "<br>" );
httpPrintf( cnt, "If you want to start an Empire of your own, look at the <a href=\"%s&amp;type=1\">Empire rankings</a> and pick a number not yet in the list!", URLAppend( cnt, "rankings" ) );
httpString( cnt, "<br>Want to team up with random players? Leave blank. But remember, be a teamplayer and you'll earn a rank in the Empire.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "<a name=\"a3\"><b><i>3. Choosing your race.</i></b></a><br>" );
httpString( cnt, "You are well on your way to making a name for yourself.<br>" );
httpString( cnt, "But how will people remember you? As an aggressive attacker? A proud and rich Energy provider? A self made and self sufficient powerhouse?<br>" );
httpString( cnt, "<br>" );
httpString( cnt, "Your race will decide which path you will walk.<br>" );
httpPrintf( cnt, "<a href=\"%s&amp;type=races\">View the stats for each race here</a>.<br><br>", URLAppend( cnt, "info" ) );

httpString( cnt, "<a name=\"a4\"><b><i>4. Completion and logging in.</i></b></a><br>" );
httpString( cnt, "Congratulations. You have created an account, chosen an Empire to fight for and selected your race.<br>" );
httpString( cnt, "You are now ready to explore, build and fight your way to the top.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "</td></tr></table><br><br>" );

httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\" class=\"left\">" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Playing the game:</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\"><br>" );

httpString( cnt, "<a name=\"b0\"><b><i>0. Resources & Buildings.</i></b></a><br>" );
httpString( cnt, "Your main concern is growth. You need to spend your resources by making resources.<br>" );
httpString( cnt, "How do you do that? Simple. Build buildings on your planets.<br>" );
httpString( cnt, "There are 4 resources in the galaxy. Energy, Minerals, Crystal and Ectrolium. <br>" );
httpString( cnt, "Most buildings need 3 of the 4 resources to be built, so you need to plan your building a bit at the start.<br><br>" );

httpString( cnt, "<a name=\"b1\"><b><i>1. Planets.</i></b></a><br>" );
httpString( cnt, "Exploring and conquering planets is your way of getting to the top.<br>" );
httpString( cnt, "You start out with 4 exploration ships. Send these out into the galaxy to colonise planets.<br>" );
httpString( cnt, "Don't worry, you'll be building a lot of them soon enough. Afterall, who is pleased with <i>just</i> 4 planets....<br><br>" );

httpString( cnt, "<a name=\"b2\"><b><i>2. Research.</i></b></a><br>" );
httpString( cnt, "Get smarter and do it smart.<br>" );
httpString( cnt, "Research is a very important aspect of the game.<br>" );
httpString( cnt, "Head over to your Research tab and distribute points wisely!<br><br>" );

httpString( cnt, "<a name=\"b3\"><b><i>3. Military.</i></b></a><br>" );
httpString( cnt, "A big fleet makes you strong, but they also increase your Networth.<br>" );
httpString( cnt, "A higher networth makes it harder to grow.<br>" );
httpString( cnt, "Build up your fleet steadily, don't go overboard.<br>" );
httpString( cnt, "And remember, you need to pay upkeep for your little Cruisers too!<br><br>" );

httpString( cnt, "</td></tr></table><br><br>" );



httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\" class=\"left\">" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Tips:</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\"><br>" );

httpString( cnt, "<a name=\"c0\"><b><i>0. Map generation</i></b></a><br>" );
httpString( cnt, "The galaxy map is the place where you'll find planets to explore and conquer... But it is easy to lose your way and location." );
httpString( cnt, "<br>Luckily you can generate a map using the Generation page." );
httpString( cnt, "<br>You can filter away planets that are already owned, that are of your Empire or are that of enemies!" );
httpString( cnt, "<br>A usefull tool to say the least.<br><br>" );

httpString( cnt, "<a name=\"c1\"><b><i>1. Fleet page.</i></b></a><br>" );
httpString( cnt, "Check your fleet page. In the top right you see <b>\"Fleet Orders\"</b>, select the dropdown and chose <b>\"Wait in System\"</b> instead of the default one." );
httpString( cnt, "<br>It is easy to forget this option, and spend 10 minutes wondering where your mighty fleet has gone!.<br><br>" );

httpString( cnt, "<a name=\"c2\"><b><i>2. Account page.</i></b></a><br>" );
httpString( cnt, "This will here page allows you to alter your Faction name when the game hasn't started yet." );
httpString( cnt, "<br>Once the time starts flowing, you won't be able to change your name until the next round has started.<br><br>" );


httpString( cnt, "</td></tr></table><br><br>" );
httpString( cnt, "</td><td width=\"7%\">&nbsp;</td></tr></table>" );

iohtmlFunc_endhtml( cnt );

return;
}


void iohtmlFunc_halloffame( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
	int a;
	struct stat stdata;
	char DIRCHECKER[PATH_MAX];
	char LINKSTRING[256];


iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_RANKS );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Hall of Fame / Server Rankings</b></font></td></tr>" );
httpString( cnt, "<tr><td align=\"left\">" );
httpString( cnt, "<table>" );
httpPrintf( cnt, "<tr><td><br>Round %d - Current round<br><a href=\"%s&amp;type=1\">Empires</a> - <a href=\"%s\">Players</a></td></tr>", ticks.round, URLAppend( cnt, "rankings" ), URLAppend( cnt, "rankings" ) );

settings = GetSetting( "Directory" );
for( a = ( ticks.round - 1 ); a > -1; a--) {
	httpPrintf( cnt, "<tr><td><br>Round %d<br>", a );

	sprintf( DIRCHECKER, "%s/rankings/round%dfamranks.txt", settings->string_value, a );
	sprintf( LINKSTRING, "<a href=\"%s&amp;round=%d&amp;type=1\">", URLAppend( cnt, "rankings" ), a );
	httpPrintf( cnt, "%sEmpires%s - ", ((stat( DIRCHECKER, &stdata ) != -1) ? LINKSTRING : ""), ((stat( DIRCHECKER, &stdata ) != -1) ? "</a>" : "") );

	sprintf( DIRCHECKER, "%s/rankings/round%dranks.txt", settings->string_value, a );
	sprintf( LINKSTRING, "<a href=\"%s&amp;round=%d\">", URLAppend( cnt, "rankings" ), a );
	httpPrintf( cnt, "%sPlayers%s", ((stat( DIRCHECKER, &stdata ) != -1) ? LINKSTRING : ""), ((stat( DIRCHECKER, &stdata ) != -1) ? "</a>" : "") );

	httpString( cnt, "</td></tr>" );
}
httpString( cnt, "</table>" );

httpString( cnt, "</table><br><br><br><br><br><br><br><br></td><td width=\"7%\">&nbsp;</td></tr>" );

iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_notices( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
	dbUserMainDef maind;
	struct stat stdata;
	bool boxopen = false;
	char *data;
	char DATAPOOL[USER_DESC_MAX];
	char DIRCHECKER[PATH_MAX];
	FILE *file;
	int id;

if( iohtmlVarsFind( cnt, "request" ) ) {
	iohtmlBase( cnt, 1|32 );
	httpString( cnt, "<table width=\"100%\">" );
} else {
	iohtmlBase( cnt, 8 );
	if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
		if( dbUserMainRetrieve( id, &maind ) < 0 ) {
			return;
		}
	}
	iohtmlFunc_frontmenu( cnt, FMENU_NOTICES );
}

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td class=\"center\"><div class=\"genlarge\"><b>Listing of all Server Notices</b></span></td></tr>" );
httpString( cnt, "<tr><td></td></tr>" );
httpString( cnt, "<tr><td align=\"left\">" );

//read notices from notices.txt and format for display. -- If this file is missing, or empty it is skipped.
settings = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/notices.txt", settings->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( data[0] == '#' ) {
						continue;
					}
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody><tr>" );
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
						httpPrintf( cnt, "<td background=\"files?type=image&amp;name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>%s</b></font></td>", DATAPOOL );
						httpString( cnt, "</tr><tr><td><font size=\"2\">" );
						boxopen = true;
					} else if ( strlen( trimwhitespace(data) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
						httpPrintf( cnt, "&nbsp;&nbsp;%s<br>", DATAPOOL );
					}
					if( (boxopen) && ( strlen( trimwhitespace(data) ) == false ) ) {
						httpString( cnt, "</font></td></tr></tbody></table><br>" );
						boxopen = false;
					}
				}
				if(boxopen) {
					httpString( cnt, "</font></td></tr></tbody></table>" );
				}
			}
			fclose( file );
		}
	free( data );
	}
}
//end notices

httpString( cnt, "</td></tr></table></td><td width=\"7%\">&nbsp;</td></tr>" );

iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_login( ReplyDataPtr cnt, char *text, ... ) {
	ConfigArrayPtr settings;
	dbUserInfoDef infod;
	int a, id, num;
	unsigned int i;
	int64_t *newsp, *newsd;
	char *data, *name, *pass;
	char timebuf[512], rtpass[USER_PASS_MAX], DIRCHECKER[PATH_MAX], DATAPOOL[DEFAULT_BUFFER];
	struct stat stdata;
	FILE *file = NULL;
	va_list ap;
	#if FACEBOOK_SUPPORT
	char *token = NULL;
	#endif

if( text != NULL ) {
	va_start( ap, text );
	vsnprintf( DATAPOOL, USER_DESC_MAX, text, ap );
	va_end( ap );
	text = DATAPOOL;
}


name = iohtmlVarsFind( cnt, "name" );
pass = iohtmlVarsFind( cnt, "pass" );

#if FACEBOOK_SUPPORT
token = iohtmlVarsFind( cnt, "fblogin_token" );
#endif

iohtmlBase( cnt, 8 );
#if FACEBOOK_SUPPORT
if( ( !(name) && !(pass) ) && !(token) ) {
	iohtmlFBSDK( cnt );
}
#endif
iohtmlFunc_frontmenu( cnt, FMENU_NONE );

id = iohtmlIdentify( cnt, 0 );

if( ( id >= 0 ) ) {
	goto LOGIN_SUCESS;
}

if( ( name ) && ( pass ) ) {
	settings = GetSetting( "Directory" );
	sprintf( DIRCHECKER, "%s/logs/login.log", settings->string_value );
	if( ( file = fopen( DIRCHECKER, "a" ) ) ) {
		time( &now );
		strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &now ) );
		fprintf( file, "Time: %s\n", timebuf );
		fprintf( file, "Name: %s;\n", name );
		if( (cnt->connection)->addr->sa_family == AF_INET )
			fprintf( file, "IP %s;\n", inet_ntoa( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ) );
		strcpy(DIRCHECKER, iohtmlHeaderFind( cnt, "User-Agent" ) );
		for(i=0;i<strlen(DIRCHECKER);i++) {
			if(DIRCHECKER[i] == ';')
				DIRCHECKER[i] = ',';
		}
		fprintf( file, "User Agent: %s;\n", DIRCHECKER );
	}
 
	for( a = 0 ; name[a] ; a++ ) {
		if( name[a] == '+' )
			name[a] = ' ';
		else if( ( name[a] == 10 ) || ( name[a] == 13 ) )
			name[a] = 0;
	}
	for( a = 0 ; pass[a] ; a++ ) {
		if( pass[a] == '+' )
			pass[a] = ' ';
		else if( ( pass[a] == 10 ) || ( pass[a] == 13 ) )
    			pass[a] = 0;
	}
	if( strncmp( name, "FBUSER", 6 ) == 0 ) {
		if( file ) {
			fprintf( file, "Ban Match\n" );
		}
		info( "ban match" );
		goto LOGIN_FAIL;
	}
	if( ( id = dbUserSearch( name ) ) < 0 ) {
		if( file ) {
			fprintf( file, "No User named: %s\n", name );
		}
		goto LOGIN_FAIL;
	}
	if( dbUserRetrievePassword( id, rtpass ) < 0 ) {
		if( file ) {
			fprintf( file, "Error Getting Password.\n" );
		}
		error( "Getting Password for User: %d", id );
		goto LOGIN_FAIL;
	}
	if( !( checkencrypt( pass, rtpass ) ) ) {
		if( file ) {
			fprintf( file, "Password Missmatch.\n" );
		}
		goto LOGIN_FAIL;
	}
	if( dbUserLinkDatabase( cnt, id ) < 0 ) {
		if( file ) {
			fprintf( file, "Unable to link user -> database\n" );
		}
		error( "Getting linking User: %d", id );
		goto LOGIN_FAIL;
	}

	if( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 ) {
		if( file ) {
			fprintf( file, "Setting session failed.\n" );
		}
		error( "Getting setting session for User: %d", id );
		goto LOGIN_FAIL;
	}
	dbRegisteredInfo[DB_TOTALS_USERS_ONLINE]++;
	dbUserInfoRetrieve( id, &infod );
	infod.lasttime = time( 0 );
	if( ((cnt->connection)->addr)->sa_family == AF_INET ) {
		for( a = (MAXIPRECORD-2); a >= 0 ; a-- ) {
			if( strcmp(inet_ntoa( infod.sin_addr[a] ),"0.0.0.0") ) {
				memcpy( &(infod.sin_addr[a+1]), &(infod.sin_addr[a]), sizeof(struct in_addr) );
			}
		}
		memcpy( &(infod.sin_addr[0]), &(((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr), sizeof(struct in_addr) );
		dbUserInfoSet( id, &infod );
	}

	if( ( file ) ) {
		fprintf( file, "ID : %d ( %x ) %s\n\n\n", id, id, ( bitflag( ((cnt->session)->dbuser)->flags, ( CMD_USER_FLAGS_KILLED | CMD_USER_FLAGS_DELETED | CMD_USER_FLAGS_NEWROUND ) ) ? "Deactivated" : "Active") );
		fclose( file );
	}

	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_KILLED ) ) {
		httpString( cnt, "Your Home Planet has been conquered and whiped out, your faction has been destroyed!<br><br>" );
		httpPrintf( cnt, "<a href=\"%s\">Rejoin the Galaxy</a>", URLAppend( cnt, "register" ) );
		httpString( cnt, "<br><br>" );
		num = dbUserNewsList( id, &newsp );
		newsd = newsp;
		if( !( num ) ) {
			httpString( cnt, "<br><b>There are no news reports to display.</b><br>" );
		}
		for( a = 0 ; a < num ; a++, newsd += DB_USER_NEWS_BASE ) {
			iohtmlNewsString( cnt, newsd );
		}
		if( newsp ) {
			free( newsp );
		}
		goto iohtmlFunc_mainL1;
	}
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_DELETED ) ) {
		httpString( cnt, "<br>Your account have been deleted by an administrator, most likely for not respecting a rule of the game.<br><br>" );
		httpPrintf( cnt, "<a href=\"%s\">Register this account again</a>", URLAppend( cnt, "register" ) );
		httpString( cnt, "<br><br>" );
		goto iohtmlFunc_mainL1;
	}
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_NEWROUND ) ) {
		httpString( cnt, "<br>The account has been deactivated for the new round, starting soon!<br>You'll be asked to join an empire of your choice again.<br><br>" );
		httpPrintf( cnt, "<a href=\"%s\">Complete account registration</a>", URLAppend( cnt, "register" ) );
		httpString( cnt, "<br><br>" );
		goto iohtmlFunc_mainL1;
	}

	if( !( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
		httpString( cnt, "<br>The activation of this account was not completed.<br><br>" );
		httpPrintf( cnt, "<a href=\"%s\">Continue Registration</a>", URLAppend( cnt, "register" ) );
		httpString( cnt, "<br><br>" );
		iohtmlFunc_mainL1:
		httpPrintf( cnt, "<a href=\"%s\">Public Forums</a>", URLAppend( cnt, "forum" ) );
		if( (cnt->session)->dbuser != NULL ) {
			if( ((cnt->session)->dbuser)->level >= LEVEL_ADMINISTRATOR ) {
				httpString( cnt, "<br><br>" );
				httpPrintf( cnt, "<a href=\"%s\">Admin panel</a>", URLAppend( cnt, "administration" ) );
			} else if( ((cnt->session)->dbuser)->level == LEVEL_MODERATOR ) {
				httpString( cnt, "<br>" );
				httpPrintf( cnt, "<a href=\"%s\">Moderator panel</a>", URLAppend( cnt, "moderator" ) );
			}
		}
	iohtmlFunc_endhtml( cnt );
	return;
	}
	goto LOGIN_SUCESS;
} else {
	goto LOGIN_RAW;
}
LOGIN_FAIL:
if( file ) {
	fprintf( file, "Failed!\n\n\n" );
	fclose( file );
}
httpString( cnt, "<span class=\"genlargered\">Login Failed</span><br>" );
LOGIN_RAW:

if( ( text != NULL ) ) {
	httpPrintf( cnt, "<h3>%s</h3>", text );
	settings = GetSetting( "HTTP Text" );
	sprintf( DIRCHECKER, "%s/login.txt", settings->string_value );
	if( stat( DIRCHECKER, &stdata ) != -1 ) {
		if( ( data = malloc( stdata.st_size + 1 ) ) ) {
			data[stdata.st_size] = 0;
			if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
				if( stdata.st_size > 0 ) {
					httpString( cnt, "<br>" );
					while( fgets( data, stdata.st_size, file ) != NULL ) {
						if( data[0] == '#' ) {
							continue;
						}
						httpPrintf( cnt, "%s<br>", trimwhitespace(data) );
					}
					httpString( cnt, "<br><br>" );
				}
				fclose( file );
			}
			free( data );
		}
	}
} else {
	httpString( cnt, "<h3>Login</h3>" );
}

httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "login" ) );

httpString( cnt, "Name<br><input type=\"text\" name=\"name\"><br>" );
httpString( cnt, "<br>Password<br><input type=\"password\" name=\"pass\"><br>" );
httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );


goto LOGIN_END;

LOGIN_SUCESS:
#if FACEBOOK_SUPPORT
if( token ) {
	redirect( cnt, "%s", URLAppend( cnt, "facebook") );
} else
#endif
if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) {
	redirect( cnt, "%s", URLAppend( cnt, "hq") );
} else {
	redirect( cnt, "%s", URLAppend( cnt, "register") );
}

httpString( cnt, "<b>Login sucess, you should be redirected shortly...</b><br>" );
httpString( cnt, "<br>" );
httpPrintf( cnt, "<a href=\"%s\">Click here if it takes too long.</a>", URLAppend( cnt, bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ? "hq" : "register" ) );
httpString( cnt, "<br>" );

LOGIN_END:
iohtmlFunc_endhtml( cnt );
return;
}

#define REGISTER_DISABLE TRUE

void iohtmlFunc_register( ReplyDataPtr cnt ) {
	int a, id, raceid;
	unsigned int i;
	FILE *file;
	char timebuf[256];
	char COREDIR[PATH_MAX];
	struct stat stdata;
	char *data;
	char *name, *pass, *faction, *race, *empire, *fampass, *rules;
	char *token = NULL;
	dbUserMainDef maind;
	#if FACEBOOK_SUPPORT
	FBUserDef fbdata;
	FBTokenDef token_post;
	dbUserInfoDef infod;
	char fbtemp[2][USER_NAME_MAX];
	#endif
	int64_t newd[DB_USER_NEWS_BASE];
	dbMailDef maild;
	dbMainEmpireDef empired;
	ConfigArrayPtr settings;
	char Message[] = "Congratulations! You have successfully registered your account!<br>Good luck and have fun.";


race = iohtmlVarsFind( cnt, "race" );
rules = iohtmlVarsFind( cnt, "rule_agree" );
empire = iohtmlVarsFind( cnt, "empire" );
fampass = iohtmlVarsFind( cnt, "fampass" );
name = iohtmlVarsFind( cnt, "name" );
pass = iohtmlVarsFind( cnt, "pass" );
faction = iohtmlVarsFind( cnt, "faction" );
#if FACEBOOK_SUPPORT
token = iohtmlVarsFind( cnt, "fblogin_token" );
#endif

id = iohtmlIdentify( cnt, 8 );

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );

httpPrintf( cnt, "<h3>%s</h3>", ( ( (cnt->session)->dbuser ) ? "Activation" : "Registration" ) );

//Used to skip/disable registration.
#if REGISTER_DISABLE
goto DISABLE;
#endif
if( race ) {
	if( id < 0 ) {
		httpString( cnt, "Error encountered while validating user registration..." );
		goto END;
	}
	if( fampass ) {
		for( a = 0 ; a < 31 ; a++ ) {
			if( ( fampass[a] == 10 ) || ( fampass[a] == 13 ) )
				break;
		}
	}
	if( !( empire ) || ( empire[0] == 0 ) )
		a = -1;
	else if( empire[0] == '#' )
		sscanf( &empire[1], "%d", &a );
	else
		sscanf( empire, "%d", &a );

	sscanf( race, "%d", &raceid );

	if( cmdExecNewUserEmpire( id, a, fampass, raceid, ((cnt->session)->dbuser)->level ) < 0 ) {
		httpString( cnt, cmdErrorString ? cmdErrorString : "Error encountered while registering user" );
 		httpString( cnt, "<br><br>" );
 		httpPrintf( cnt, "<a href=\"%s\">Try Again.</a>", URLAppend( cnt, "register" ) );
 		goto END;
  	}
	httpPrintf( cnt, "<b>Account activated!</b><br>" );
	httpString( cnt, "<br><br><br>" );
	httpPrintf( cnt, "<a href=\"%s\">Click here if not redirected.</a>", URLAppend( cnt, "hq" ) );
	redirect( cnt, "%s", URLAppend( cnt, "hq" ) );
	dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED]++;
	goto END;
} else if( ( ( token != NULL ) && ( ( faction != NULL ) && ( strlen(faction) > 0 ) ) ) || ( ( name != NULL ) && ( pass != NULL ) && ( faction != NULL ) ) ) {
	if( ( name != NULL ) && ( strncmp( name, "FBUSER", 6 ) == 0 ) ) {
		httpPrintf( cnt, "Username format prohibited<br>%s is blacklisted due to FBUSER*", name );
		httpString( cnt, "<br><br>" );
 		httpPrintf( cnt, "<a href=\"%s&amp;rule_agree=true\">Try Again.</a>", URLAppend( cnt, "register" ) );
		goto END;
	}
	#if FACEBOOK_SUPPORT
	if( ( name != NULL ) && ( pass != NULL ) ) {
		snprintf( fbtemp[0], USER_NAME_MAX-1, "%s", name );
		snprintf( fbtemp[1], USER_NAME_MAX-1, "%s", pass );
	} else if ( token != NULL ) {
		strncpy( token_post.val, token, sizeof( token_post.val ) );
		facebook_getdata_token( &fbdata, token_post );
		if( strlen( fbdata.id ) == 0 ) {
			httpString( cnt, "Invalid Token Detected... Aborting!" );
			redirect( cnt, "%s", URLAppend( cnt, "index" ) );
			goto END;
		}
		snprintf( fbtemp[0], USER_NAME_MAX-1, "FBUSER%s", fbdata.id );
		RANDOMIZE_SEED;
		snprintf( fbtemp[1], USER_NAME_MAX-1, "P%X%X%X%X", (unsigned int)random(), (unsigned int)random(), (unsigned int)random(), (unsigned int)random() );
	}
	if( ( id = cmdExecNewUser( fbtemp[0], fbtemp[1], faction ) ) < 0 ) {
	#else
	if( ( id = cmdExecNewUser( name, pass, faction ) ) < 0 ) {
	#endif

		httpString( cnt, cmdErrorString ? cmdErrorString  : "Error encountered while registering user" );

		httpString( cnt, "<br><br>" );
		httpPrintf( cnt, "<a href=\"%s&amp;rule_agree=true\">Try Again.</a>", URLAppend( cnt, "register" ) );
		goto END;
	}
  	newd[0] = ticks.number;
	newd[1] = CMD_NEWS_FLAGS_NEW;
	newd[2] = CMD_NEWS_MAIL;
	newd[3] = 0;
	newd[4] = 0; //From the admin
	if( dbUserMainRetrieve( newd[4], &maind ) < 0 ) {
		newd[5] = 0; //From the admin team
		memcpy( &newd[6], "Administration", 15 );
		sprintf( (maild.mail).authorname, "Administration" );
		(maild.mail).authorempire = 0;
	} else {
		(maild.mail).authorempire = newd[5] = maind.empire;
		memcpy( &newd[6], maind.faction, strlen(maind.faction) );
		sprintf( (maild.mail).authorname, "%s", maind.faction );
	}
	cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_MAIL );

	(maild.mail).length = strlen(Message);
	maild.text = Message;
	(maild.mail).authorid = 0;
	(maild.mail).time = time( 0 );
	(maild.mail).tick = ticks.number;
	(maild.mail).flags = 0;
	if( dbMailAdd( id, 0, &maild ) < 0 )
		error( "Error sending registration mail" );

	if( ( dbUserLinkDatabase( cnt, id ) < 0 ) || ( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 ) ) {
		httpString( cnt, "Error encountered while registering session" );
		goto END;
	}
	httpString( cnt, "New user created<br>" );
	#if FACEBOOK_SUPPORT
	if( ( token != NULL ) && ( strlen( fbdata.id ) ) ) {
		dbUserInfoRetrieve( id, &infod );
		snprintf( fbdata.token.val, sizeof(fbdata.token.val), "%s", token );
		infod.fbinfo = fbdata;
		dbUserInfoSet( id, &infod );
		bitflag_add( &((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBMADE );
		bitflag_add( &((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBLINK );
		snprintf( ((cnt->session)->dbuser)->fbid, sizeof( ((cnt->session)->dbuser)->fbid ), "%s", fbdata.id );
		dbUserSave( id, (cnt->session)->dbuser );
	} else
	#endif	
	httpPrintf( cnt, "User name : %s<br>Password : %s<br>", ((cnt->session)->dbuser)->name, pass );
	httpPrintf( cnt, "Faction name : %s<br>Account ID : %d<br>", ((cnt->session)->dbuser)->faction, id );
	settings = GetSetting( "Directory" );
	sprintf( COREDIR, "%s/logs/register.log", settings->string_value );
	if( ( file = fopen( COREDIR, "a" ) ) ) {
		fprintf( file, "Register ID %d ( %x )\n", id, id );
		time( &now );
		strftime( timebuf, 256, "%T, %b %d %Y;", localtime( &now ) );
		fprintf( file, "Time %s\n", timebuf );
		fprintf( file, "Name %s;\n", ((cnt->session)->dbuser)->name );
		fprintf( file, "Faction %s;\n", ((cnt->session)->dbuser)->faction );
		if( (cnt->connection)->addr->sa_family == AF_INET )
			fprintf( file, "IP %s;\n", inet_ntoa( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ) );
		strcpy(timebuf, iohtmlHeaderFind( cnt, "User-Agent" ) );
		for(i=0;i<strlen(timebuf);i++) {
			if(timebuf[i] == ';')
				timebuf[i] = ',';
		}
		fprintf( file, "User Agent: %s;\n", timebuf );
		fprintf(file, "ID : %d ( %X );\n\n\n", id, id);
		fclose( file );
	}



} else if ( ( id < 0 ) ) {
	if( ( rules == NULL ) || ( ( rules != NULL ) && ( strcmp( rules, "true") != 0 ) ) ) {
		if( iohtmlVarsFind( cnt, "submitted" ) != NULL ) {
			httpString( cnt, "<h3>You must agree with the game rules to be able to continue...!</h3>" );
		} else {
			httpString( cnt, "<h3>Please read the rules below, and click the box if you agree...</h3>" );
		}
		settings = GetSetting( "HTTP Text" );
		sprintf( COREDIR, "%s/rules.txt", settings->string_value );
		if( stat( COREDIR, &stdata ) != -1 ) {
			if( ( data = malloc( stdata.st_size + 1 ) ) ) {
				data[stdata.st_size] = 0;
				if( ( file = fopen( COREDIR, "rb" ) ) ) {
					if( stdata.st_size > 0 ) {
					a = 0;
					httpString( cnt, "<table class=\"left\">" );
						while( fgets( data, stdata.st_size, file ) != NULL ) {
							if( data[0] == '#' ) {
								continue;
							}
							if( strlen(data) > 1 ) {
								a++;
								httpPrintf( cnt, "<tr><td>%d</td><td>&nbsp;</td><td>%s</td></tr>", a, trimwhitespace(data) );
							}
						}
					httpString( cnt, "</table><br>" );
					}
					fclose( file );
				}
				free( data );
				settings = GetSetting( "Server Name" );
				httpString( cnt, "<br>" );
				httpPrintf( cnt, "Rules are subject to change at any time and applicable to every instance of %s.<br>", settings->string_value );
				httpString( cnt, "<br>" );
				httpString( cnt, "Administration is open to discussion regarding these rules. In all cases the Administration's decision is final.<br>" );
				httpString( cnt, "While we are open to discuss these rules, they always apply unless specificly waived/altered by Administration. In which case you will be notified.<br>" );
				httpString( cnt, "<br>" );
				httpString( cnt, "Players breaking the rules will get a warning, an account reset/deletion or a permanent ban.<br>" );
				httpString( cnt, "When a player gets warned, his player tag will be changed to \"Warned\" for a minimum of 4 days.<br>" );
				httpString( cnt, "You do not get 2 warnings. A second violation is an account reset (your records will be kept).<br>" );
				httpString( cnt, "A third violation is an account deletion and a fourth violation will require me to go all out, and find a way to ban you.<br>" );
			}
		}
		httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "register" ) );
		httpPrintf( cnt, "<input type=\"checkbox\" name=\"rule_agree\" value=\"true\"> I agree<br>" );
		httpPrintf( cnt, "<input type=\"hidden\" name=\"submitted\" value=\"true\">" );
		if( token != NULL ) {
			httpPrintf( cnt, "<input type=\"hidden\" name=\"fblogin_token\" value=\"%s\">", token );
		}
		if( faction != NULL ) {
			httpPrintf( cnt, "<input type=\"hidden\" name=\"faction\" value=\"%s\">", faction );
		}
		httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );
		goto END;
	}
	httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "register" ) );
	if( token == NULL ) {
		httpString( cnt, "User name<br><input type=\"text\" name=\"name\"><br>" );
		httpString( cnt, "<br>Password<br><input type=\"password\" name=\"pass\"><br>" );
	} else {
		httpPrintf( cnt, "<input type=\"hidden\" name=\"fblogin_token\" value=\"%s\">", token );
	}
	if( rules != NULL ) {
			httpPrintf( cnt, "<input type=\"hidden\" name=\"rule_agree\" value=\"%s\">", rules );
	}
	httpString( cnt, "<br>Faction name<br><input type=\"text\" name=\"faction\"><br>" );
	httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );
	goto END;
}

if( ( ((cnt->session)->dbuser) ) && ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
	redirect( cnt, "/%s", URLAppend( cnt, "hq" ) );
	httpString( cnt, "This account has already been activated, you will now be redirected into the game...<br>" );
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s\">Click here if it takes too long<a>", URLAppend( cnt, "/main" ) );
} else {
	httpString( cnt, "<script type=\"text/javascript\">" );
	httpString( cnt, "function show(obj) {" );
	httpString( cnt, "no = obj.options[obj.selectedIndex].value;" );
	httpString( cnt, "count = obj.options.length;" );
	httpString( cnt, "if(no < 0) {" );
        httpString( cnt, "document.getElementById('fampass_block').style.display = 'none';" );
        httpString( cnt, "document.getElementById('fampass_text').value = '';" );
        httpString( cnt, "}" );
	for( a = ( ( ((cnt->session)->dbuser) ) ? ( ( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) ? 0 : 1  ) : 1 ); a < dbMapBInfoStatic[MAP_EMPIRES]; a++ ) {
		if( dbEmpireGetInfo( a, &empired ) < 0 ) {
			continue;
		}
		if( empired.password[0] ) {
			httpPrintf( cnt, "else if( no == %d ) {", a );
			httpString( cnt, "document.getElementById('fampass_block').style.display = '';" );
			httpPrintf( cnt, "}", a );
		} else {
			httpPrintf( cnt, "else if( no == %d ) {", a );
			httpString( cnt, "document.getElementById('fampass_block').style.display = 'none';" );
        		httpString( cnt, "document.getElementById('fampass_text').value = '';" );
        		httpPrintf( cnt, "}", a );
		}
	}
	httpString( cnt, "}" );
	httpString( cnt, "</script>" );
	
	httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><br><br>Empire number<br><i>Leave blank to join a random empire</i><br>", URLAppend( cnt, "register" ) );
	httpString( cnt, "<select name=\"empire\" onchange=\"show(this)\" onkeypress=\"show(this)\">" );
	httpString( cnt, "<option value=\"-1\" selected>Random Empire</option>" );
	for( a = ( ( ((cnt->session)->dbuser) ) ? ( ( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) ? 0 : 1  ) : 1 ); a < dbMapBInfoStatic[MAP_EMPIRES]; a++ ) {
		if( dbEmpireGetInfo( a, &empired ) < 0 ) {
			continue;
		}
		httpPrintf( cnt, "<option value=\"%d\">", a );
		if( empired.name[0] ) {
			httpPrintf( cnt, "%s</option>", empired.name );
		} else {
			httpPrintf( cnt, "Empire #%d</option>", a );
		}
	}
	httpString( cnt, "</select><br><br>" );
	
	httpString( cnt, "<div style=\"display:none\" id=\"fampass_block\">" );
	httpString( cnt, "Empire password<br><i>This empire has a password set, you must enter it here to be allowed entry!</i><br>" );
	httpString( cnt, "<input type=\"text\" name=\"fampass\" id=\"fampass_text\"><br><br>" );
	httpString( cnt, "</div>" );
	

	httpString( cnt, "Faction race<br><i>The race of your people define many characteristics affecting different aspects of your faction.</i> - " );
	httpPrintf( cnt, "<a href=\"%s&amp;type=races&amp;request=ajax\" rel=\"ajaxpanel\" data-loadtype=\"ajax\">See races</a><br><select name=\"race\">", URLAppend( cnt, "info" ) );
	for( a = 0 ; a < CMD_RACE_NUMUSED ; a++ ) {
		httpPrintf( cnt, "<option value=\"%d\">%s</option>", a, cmdRaceName[a] );
	}
	httpString( cnt, "</select><br><br>" );
	httpString( cnt, "<input type=\"submit\" value=\"OK\"></form>" );
}

httpPrintf( cnt, "<br><br><a href=\"%s&amp;type=1\" rel=\"ajaxpanel\" data-loadtype=\"iframe\">See empire rankings</a>", URLAppend( cnt, "rankings" ) );
httpPrintf( cnt, "<br><a href=\"%s\" rel=\"ajaxpanel\" data-loadtype=\"iframe\">See faction rankings</a>", URLAppend( cnt, "rankings" ) );

//End New functions
#if REGISTER_DISABLE
goto END;
DISABLE:
httpString ( cnt, "<b>Sorry... Currently disabled</b>" );
#endif

END:
iohtmlFunc_endhtml( cnt );


return;
}

void iohtmlFunc_ircapplet( ReplyDataPtr cnt ) {
	int id;
	unsigned int i, j;
	char szFaction[USER_NAME_MAX];
	dbUserMainDef maind;
	ConfigArrayPtr setting;

setting = GetSetting( "IRC Channel" );

id = iohtmlIdentify( cnt, 2 );
if( dbUserMainRetrieve( id, &maind ) < 0 ) {
	maind.empire = -1;
}
iohtmlBase( cnt, 8 );

httpString( cnt, "<table class=\"center\" cellspacing=\"0\" cellpadding=\"10\" style=\"width:100%;border-width:0;\"><tbody><tr><td class=\"bodyline\">" );
httpString( cnt, "<table class=\"center\" cellspacing=\"0\" cellpadding=\"10\" style=\"width:100%;border-width:0;\"><tbody><tr><td>" );
html_boxstart( cnt, false, "IRC Chat" );
if( id >= 0 ) {
	strcpy(szFaction, maind.faction);
	for(i=0;i<strlen(szFaction);i++) {
		if (szFaction[i] == ' ') {
			for(j=i;j<(strlen(szFaction)-1);j++)
				szFaction[j] = szFaction[j+1];
			szFaction[j] = '\0';
		}
		if(i == 15) {
			szFaction[i-1] = '\0';
			break;
		}
	}
	httpPrintf( cnt, "<iframe src=\"http://webchat.freenode.net?channels=%s&uio=MTA9dHJ1ZSYxMT0yNTY24&nick=%s\" width=\"647\" height=\"400\"></iframe>", setting->string_value, szFaction );
} else {
	httpPrintf( cnt, "<iframe src=\"http://webchat.freenode.net?channels=%s&uio=MTA9dHJ1ZSYxMT0yNTY24\" width=\"647\" height=\"400\"></iframe>", setting->string_value );
}
html_boxend( cnt );

iohtmlFunc_endhtml( cnt );

return;
}

