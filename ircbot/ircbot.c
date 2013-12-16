#ifndef GLOBALINCLUDED
#include "../global.h"
#endif

#include "libircclient.c"

float ircbot_version = 0.01;

#if PIPEFILE_SUPPORT
int ircbot_command( char *command ) {
	char *sub = {0}, *chop = {0};
/*
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

#define IS_CTRL  (1 << 0)
#define IS_EXT	 (1 << 1)
#define IS_ALPHA (1 << 2)

unsigned int char_tbl[256] = {0};


void init_table() {
	int i;

	for (i = 0; i < 32; i++) char_tbl[i] |= IS_CTRL;
	char_tbl[127] |= IS_CTRL;

	for (i = 'A'; i <= 'Z'; i++) {
		char_tbl[i] |= IS_ALPHA;
		char_tbl[i + 0x20] |= IS_ALPHA; /* lower case */
	}

	for (i = 128; i < 256; i++) char_tbl[i] |= IS_EXT;
}

void strip(char * str, int what) {
	unsigned char *ptr, *s = (void*)str;
	ptr = s;
	while (*s != '\0') {
		if ((char_tbl[(int)*s] & what) == 0)
			*(ptr++) = *s;
		s++;
	}
	*ptr = '\0';
}
/*
void ircbot_messagephrase(ircmessageDef *irc) {


init_table();
strip(irc->input, IS_CTRL | IS_EXT);

if( options.verbose )
	loghandle(LOG_INFO, false, "[from:%s] [host:%s] [reply-to:%s] %s", irc->nick, irc->host, irc->target, irc->input);

if( !strcmp(irc->input,"tick") ){
	if( ticks.status ) {
		ircbot_send("NOTICE %s :Week %d, Year %d (Tick #%d)", irc->target, ticks.number % 52, ticks.number / 52, ticks.number );
	} else {
		ircbot_send("NOTICE %s :Game time is frozen!", irc->target );
	}
} else if( !strncmp(irc->input,"login", 5) ) {
	printf("%s of host:%s attempted to login via IRC bot.\n", irc->nick, irc->host);
	if(irc->target != irc->nick) {
		ircbot_send("NOTICE %s :Login via public is so unsafe... I'm going to deny login.", irc->nick);
		ircbot_send("NOTICE %s :Please only use Private Message or Notice to login.", irc->nick);
		ircbot_send("NOTICE %s :Sorry %s, your login attempt is invalid!", irc->target, irc->nick);
		return;
	}
	ircbot_send("NOTICE %s :Sorry, %s is currently still under construction!", irc->target, irc->input);
} else {
	//Deny ability for any input not listed above.
	ircbot_send("NOTICE %s :<%s> is not currently a command I recognize!", irc->target, irc->input);
}

return;
}
*/
//Begin new IRC Test.

void addlog (const char * fmt, ...)
{
	FILE * fp;
	char buf[1024];
	va_list va_alist;

	va_start (va_alist, fmt);
	vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	printf ("%s\n", buf);

	if ( (fp = fopen ("irctest.log", "ab")) != 0 )
	{
		fprintf (fp, "%s\n", buf);
		fclose (fp);
	}
}


void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
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


void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);
	irc_cmd_user_mode (session, "+i");
	irc_cmd_msg (session, params[0], "Hi all");
}


void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	dump_event (session, event, origin, params, count);

	irc_cmd_join (session, ctx->channel, 0);
}


void event_privmsg (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);

	printf ("'%s' said me (%s): %s\n", 
		origin ? origin : "someone",
		params[0], params[1] );
}


void dcc_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	static int count = 1;
	char buf[12];

	switch (status)
	{
	case LIBIRC_ERR_CLOSED:
		printf ("DCC %d: chat closed\n", id);
		break;

	case 0:
		if ( !data )
		{
			printf ("DCC %d: chat connected\n", id);
			irc_dcc_msg	(session, id, "Hehe");
		}
		else 
		{
			printf ("DCC %d: %s\n", id, data);
			sprintf (buf, "DCC [%d]: %d", id, count++);
			irc_dcc_msg	(session, id, buf);
		}
		break;

	default:
		printf ("DCC %d: error %s\n", id, irc_strerror(status));
		break;
	}
}


void dcc_file_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	if ( status == 0 && length == 0 )
	{
		printf ("File sent successfully\n");

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else if ( status )
	{
		printf ("File sent error: %d\n", status);

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else
	{
		if ( ctx )
			fwrite (data, 1, length, (FILE*) ctx);
		printf ("File sent progress: %d\n", length);
	}
}


void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char nickbuf[128];
	char buffer[512];

	if ( count != 2 )
		return;

	printf ("'%s' said in channel %s: %s\n", 
		origin ? origin : "someone",
		params[0], params[1] );

	if ( !origin )
		return;

	irc_target_get_nick (origin, nickbuf, sizeof(nickbuf));

	if ( !strcmp (params[1], "quit") )
		irc_cmd_quit (session, "of course, Master!");

	if ( !strcmp (params[1], "help") )
	{
		irc_cmd_msg (session, params[0], "quit, help, dcc chat, dcc send, ctcp");
	}

	if ( !strcmp (params[1], "ctcp") )
	{
		irc_cmd_ctcp_request (session, nickbuf, "PING 223");
		irc_cmd_ctcp_request (session, nickbuf, "FINGER");
		irc_cmd_ctcp_request (session, nickbuf, "VERSION");
		irc_cmd_ctcp_request (session, nickbuf, "TIME");
	}

	if ( !strcmp (params[1], "dcc chat") )
	{
		irc_dcc_t dccid;
		irc_dcc_chat (session, 0, nickbuf, dcc_recv_callback, &dccid);
		printf ("DCC chat ID: %d\n", dccid);
	}

	if ( !strcmp (params[1], "dcc send") )
	{
		irc_dcc_t dccid;
		sprintf(buffer, "%s/cookie.gif", sysconfig.httpimages );
		irc_dcc_sendfile (session, 0, nickbuf, buffer, dcc_file_recv_callback, &dccid);
		printf ("DCC send ID: %d\n", dccid);
	}

	if ( !strcmp (params[1], "topic") )
		irc_cmd_topic (session, params[0], 0);
	else if ( strstr (params[1], "topic ") == params[1] )
		irc_cmd_topic (session, params[0], params[1] + 6);

	if ( strstr (params[1], "mode ") == params[1] )
		irc_cmd_channel_mode (session, params[0], params[1] + 5);

	if ( strstr (params[1], "nick ") == params[1] )
		irc_cmd_nick (session, params[1] + 5);

	if ( strstr (params[1], "whois ") == params[1] )
		irc_cmd_whois (session, params[1] + 5);
}


void irc_event_dcc_chat (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
	printf ("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

	irc_dcc_accept (session, dccid, 0, dcc_recv_callback);
}


void irc_event_dcc_send (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
	FILE * fp;
	printf ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", dccid, nick, addr, filename, size);

	if ( (fp = fopen (filename, "wb")) == 0 )
		abort();

	irc_dcc_accept (session, dccid, fp, dcc_file_recv_callback);
}

void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count) {
	char buf[24], buffer[512];

if( event == 376 ) {
	if( irccfg.botpass ){
		sprintf(buffer, "identify %s", irccfg.botpass);
		irc_cmd_msg(irccfg.session, "NickServ", buffer);
	}
	irc_cmd_join(irccfg.session, irccfg.channel, NULL);
}

sprintf (buf, "%d", event);
dump_event (session, buf, origin, params, count);

return;
}


int ircbot_connect( ) {
	irc_callbacks_t	callbacks;
	irc_ctx_t ctx;

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
	callbacks.event_notice = dump_event;
	callbacks.event_invite = dump_event;
	callbacks.event_umode = dump_event;
	callbacks.event_ctcp_rep = dump_event;
	callbacks.event_ctcp_action = dump_event;
	callbacks.event_unknown = dump_event;
	callbacks.event_numeric = event_numeric;

	callbacks.event_dcc_chat_req = irc_event_dcc_chat;
	callbacks.event_dcc_send_req = irc_event_dcc_send;

	irccfg.session = irc_create_session (&callbacks);

	if ( !irccfg.session )
	{
		printf ("Could not create session\n");
		return 1;
	}

	ctx.channel = irccfg.channel;
        ctx.nick = irccfg.botnick;
	irc_set_ctx(irccfg.session, &ctx);


	// Initiate the IRC server connection
	if ( irc_connect (irccfg.session, irccfg.host, irccfg.port, 0, irccfg.botnick, irccfg.botnick, irccfg.botnick) ) {
		printf ("Could not connect: %s\n", irc_strerror (irc_errno(irccfg.session)));
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
	error("IRC Session Error");
}

return;
}
