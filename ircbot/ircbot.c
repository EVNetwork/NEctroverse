#ifndef GLOBALINCLUDED
#include "../global.h"
#endif

#include "libircclient.c"

float ircbot_version = 0.01;

#if PIPEFILE_SUPPORT
int ircbot_command( char *command ) {
/*	char *sub = {0}, *chop = {0};

if( ( strlen(command) > 3 ) && ( strchr( command, ' ' ) ) )
sub = ( strchr( command, ' ' ) + 1 );

if( !( sub ) )
	sub = "status";

if( !( strcmp(sub,"status") ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"IRC Bot is not curently enabled!");
		return 0;
	} else {
		svPipeSend(0,"IRC Bot is curently enabled with host \'%s\' for channel \'%s\'", irccfg.host, irccfg.channel);
	}
	return 1;
} else if( !( strcmp(sub,"announce") ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"Bot is not enabled, can't announce tick!");
		return 0;
	}
	if( irccfg.announcetick ){
		irccfg.announcetick = false;
		svPipeSend(0,"Bot announce tick is now OFF!");
		ircbot_send("NOTICE %s :Administration has disabled channel notice of game tick!", irccfg.channel);
	} else {
		irccfg.announcetick = true;
		svPipeSend(0,"Bot announce tick is now ON!");
		ircbot_send("NOTICE %s :Administration has enabled channel notice of game tick!", irccfg.channel);
	}
	return 1;
} else if( !( strcmp(sub,"toggle") ) ){
	if( irccfg.bot == false ) {
		irccfg.bot = true;
		ircbot_connect();
	} else {
		ircbot_send("QUIT :Administration has requested IRC Bot shutdown!");
		if( close( options.botconn ) == -1 ) {
			loghandle(LOG_ERR, errno, "Error %03d, closing ircbot socket", errno);
		}
		irccfg.bot = false;
	}
	svPipeSend(0,"%s IRC Bot.", irccfg.bot ? "Starting" : "Stoping" );
	return 1;
} else if( !( strncmp(sub, "hop", 3) ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"IRC Bot is not enabled, can't channel hop!");
		return 0;
	}
	if( strlen(sub) > 3 )
		chop = ( strrchr( sub, ' ' ) + 1 );
	if( !( chop ) )
		chop = irccfg.channel;
	irccfg.bot = false;
	ircbot_send("NOTICE %s :Administration has requested IRC Bot channel hop to %s!", irccfg.channel, chop);
	ircbot_send("PART %s", irccfg.channel);
	svPipeSend(0,"IRC Bot Left channel %s.", irccfg.channel );
	ircbot_send("JOIN %s", chop);
	strcpy(irccfg.channel,chop);
	if( irccfg.botpass ){
		ircbot_send("PRIVMSG ChanServ :op %s %s", irccfg.channel, irccfg.botnick);
	}
	svPipeSend(0,"IRC Bot joined channel %s.", chop );
	irccfg.bot = true;
	return 1;
}
*/

return 0;
}
#endif

//Begin new IRC Test.
void addlog (const char * fmt, ...)
{
	FILE * fp;
	char buf[1024];
	va_list va_alist;

	va_start (va_alist, fmt);
	vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	//info( irc_color_strip_from_mirc( buf ) );

	if ( (fp = fopen ("irctest.log", "ab")) != 0 )
	{
		fprintf (fp, "%s\n", buf);
		fclose (fp);
	}
}


void dump_event (irc_session_t * irc_session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';

	for ( cnt = 0; cnt < count; cnt++ )
	{
		if ( cnt )
			strcat (buf, "|");

		strcat (buf, params[cnt]);
	}


	addlog ("Event \"%s\", origin: \"%s\", params: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
}


void event_join( irc_session_t *irc_session, const char *event, const char *origin, const char **params, unsigned int count ) {
	char buffer[512];

dump_event( irc_session, event, origin, params, count );
irc_cmd_user_mode( irc_session, "+i" );
irc_target_get_nick( origin, buffer, sizeof(buffer) );
if( !strncmp( origin, irccfg.botnick, strlen(irccfg.botnick) ) ) {
	sprintf( logString, "IRC Bot Joined Channel: %s", params[0] );
	info( logString );
	sprintf( buffer, "op %s", params[0] );	
	irc_cmd_msg( irc_session, "ChanServ", buffer );
}

return;
}


void event_connect( irc_session_t *irc_session, const char *event, const char *origin, const char **params, unsigned int count ) {
	char buffer[512];
dump_event (irc_session, event, origin, params, count);
sprintf( logString, "IRC Bot Connected to server: %s", origin );
info( logString );

irc_cmd_join (irc_session, irccfg.channel, 0);
if( irccfg.botpass ){
	sprintf(buffer, "identify %s", irccfg.botpass);
	irc_cmd_msg(irccfg.session, "NickServ", buffer);
}

return;
}


void event_privmsg (irc_session_t * irc_session, const char * event, const char * origin, const char ** params, unsigned int count) {
	char nickbuf[128];
	SessionPtr session;

//dump_event (irc_session, event, origin, params, count);
params[1] = irc_color_strip_from_mirc( params[1] );
addlog ("'%s' said to me (%s): %s", origin ? origin : "someone", params[0], params[1] );

if ( !origin )
	return;

irc_target_get_nick( origin, nickbuf, sizeof(nickbuf) );
session = get_session( SESSION_IRC, str2md5(origin) );

if( !( session->dbuser ) ) {
	irc_cmd_notice( irccfg.session, nickbuf, "You must be loged in to use any features." );
} else {
	irc_cmd_notice( irccfg.session, nickbuf, "Now just how did you manage that..." );
}

return;
}


void dcc_recv_callback (irc_session_t * irc_session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	static int count = 1;
	char buf[12];

	switch (status)
	{
	case LIBIRC_ERR_CLOSED:
		addlog ("DCC %d: chat closed", id);
		break;

	case 0:
		if ( !data )
		{
			addlog ("DCC %d: chat connected", id);
			irc_dcc_msg	(irc_session, id, "Hehe");
		}
		else 
		{
			addlog ("DCC %d: %s", id, data);
			sprintf (buf, "DCC [%d]: %d", id, count++);
			irc_dcc_msg	(irc_session, id, buf);
		}
		break;

	default:
		addlog ("DCC %d: error %s", id, irc_strerror(status));
		break;
	}
}


void dcc_file_send_callback (irc_session_t * irc_session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	if ( status == 0 && length == 0 )
	{
		addlog ("File sent successfully");

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else if ( status )
	{
		addlog ("File sent error: %d", status);

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else
	{
		if ( ctx )
			fwrite (data, 1, length, (FILE*) ctx);
		addlog ("File sent progress: %d", length);
	}
}


void event_channel (irc_session_t * irc_session, const char * event, const char * origin, const char ** params, unsigned int count) {
	char buffer[512];
	char nickbuf[128];
	SessionPtr session;

if ( count != 2 )
	return;

params[1] = irc_color_strip_from_mirc( params[1] );
addlog ("'%s' said in channel %s: %s", origin ? origin : "someone", params[0], params[1] );

if ( !origin )
	return;

irc_target_get_nick( origin, nickbuf, sizeof(nickbuf) );

session = get_session( SESSION_IRC, str2md5(origin) );


if ( !strcmp (params[1], "help") ) {
	if( ( session->dbuser ) && ( session->dbuser->level >= LEVEL_MODERATOR ) )
		sprintf( buffer, "%s", irc_color_convert_to_mirc( "[B]Usage[/B] - quit, help, dcc send, topic, mode, whois, nick" ) );
	else
		sprintf( buffer, "%s", irc_color_convert_to_mirc( "[B]Usage[/B] - help, dcc send, login" ) );
	irc_cmd_notice( irc_session, params[0], buffer );
}

/*if ( !strcmp (params[1], "ctcp") ) {
	irc_cmd_ctcp_request (irc_session, nickbuf, "PING 223");
	irc_cmd_ctcp_request (irc_session, nickbuf, "FINGER");
	irc_cmd_ctcp_request (irc_session, nickbuf, "VERSION");
	irc_cmd_ctcp_request (irc_session, nickbuf, "TIME");
}*/

/*if ( !strcmp (params[1], "dcc chat") ) {
		irc_dcc_t dccid;
	
	irc_dcc_chat (irc_session, 0, nickbuf, dcc_recv_callback, &dccid);
	addlog ("DCC chat ID: %d", dccid);
}*/

if ( !strcmp (params[1], "dcc send") ) {
		irc_dcc_t dccid;
		
	sprintf(buffer, "%s/cookie.gif", sysconfig.httpimages );
	irc_dcc_sendfile (irc_session, 0, nickbuf, buffer, dcc_file_send_callback, &dccid);
	addlog ("DCC send ID: %d", dccid);
}

if( ( session->dbuser ) && ( session->dbuser->level >= LEVEL_MODERATOR ) ) {

if ( !strcmp (params[1], "quit") ) {
		irc_cmd_quit (irc_session, "of course, Master!");
}

if ( !strcmp (params[1], "topic") ) {
	irc_cmd_topic (irc_session, params[0], 0);
} else if ( strstr (params[1], "topic ") == params[1] ) {
	irc_cmd_topic (irc_session, params[0], params[1] + 6);
}

if ( strstr (params[1], "mode ") == params[1] )
	irc_cmd_channel_mode (irc_session, params[0], params[1] + 5);

if ( strstr (params[1], "nick ") == params[1] )
	irc_cmd_nick (irc_session, params[1] + 5);

if ( strstr (params[1], "whois ") == params[1] )
	irc_cmd_whois (irc_session, params[1] + 5);

}

return;
}


void irc_event_dcc_chat (irc_session_t * irc_session, const char * nick, const char * addr, irc_dcc_t dccid)
{
	addlog ("DCC chat [%d] requested from '%s' (%s)", dccid, nick, addr);

	irc_dcc_accept (irc_session, dccid, 0, dcc_recv_callback);
}


void irc_event_dcc_send (irc_session_t * irc_session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
	FILE * fp;
	addlog ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)", dccid, nick, addr, filename, size);

	if ( (fp = fopen (filename, "wb")) == 0 )
		abort();

	irc_dcc_accept (irc_session, dccid, fp, dcc_file_send_callback);
}

void event_numeric (irc_session_t * irc_session, unsigned int event, const char * origin, const char ** params, unsigned int count) {
	char buf[24];

sprintf (buf, "%d", event);
dump_event (irc_session, buf, origin, params, count);

return;
}


int ircbot_prepare( ) {
	irc_callbacks_t	callbacks;

memset (&callbacks, 0, sizeof(callbacks));

callbacks.event_connect = event_connect;
callbacks.event_join = event_join;
callbacks.event_nick = dump_event;
callbacks.event_quit = dump_event;
callbacks.event_part = dump_event;
callbacks.event_mode = dump_event;
callbacks.event_topic = dump_event;
callbacks.event_kick = dump_event;
callbacks.event_channel = event_channel;
callbacks.event_privmsg = event_privmsg;
callbacks.event_notice = event_privmsg;
callbacks.event_invite = dump_event;
callbacks.event_umode = dump_event;
callbacks.event_ctcp_rep = dump_event;
callbacks.event_ctcp_action = dump_event;
callbacks.event_unknown = dump_event;
callbacks.event_numeric = event_numeric;

callbacks.event_dcc_chat_req = irc_event_dcc_chat;
callbacks.event_dcc_send_req = irc_event_dcc_send;

irccfg.session = irc_create_session( &callbacks );

if ( !irccfg.session ) {
	error("Could not create IRC session");
	return 1;
}

return 0;
}

int ircbot_connect() {

// Initiate the IRC server connection
if ( irc_connect( irccfg.session, irccfg.host, irccfg.port, 0, irccfg.botnick, irccfg.botnick, irccfg.botnick ) ) {
	error("Connecting to IRC Network");
	return 1;
} 

return 0;
}


void ircbot_select() {

// Make sure that all the IRC sessions are connected
if ( !irc_is_connected(irccfg.session) ) {
	return;
}

// Create the structures for select()
struct timeval tv;
fd_set in_set, out_set;
int maxfd = 0;

// Wait 0.25 sec for the events
tv.tv_usec = 250000;
tv.tv_sec = 0;

// Initialize the sets
FD_ZERO (&in_set);
FD_ZERO (&out_set);

// Add the IRC session descriptors - call irc_add_select_descriptors() for each active session
irc_add_select_descriptors( irccfg.session, &in_set, &out_set, &maxfd );

// Call select()
if( select(maxfd + 1, &in_set, &out_set, 0, &tv) < 0 ) {
	// Error
	if( errno != 4 )
		error("IRC Select error");
	return;
}

// Call irc_process_select_descriptors() for each session with the descriptor set
if ( irc_process_select_descriptors(irccfg.session, &in_set, &out_set) ) {
	// The connection failed, or the server disconnected. Handle it.
	if( errno != 115 )
		error("IRC Session Error");
}


return;
}
