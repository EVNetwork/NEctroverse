
/**
 * List of all pages served by this HTTP server.
 * { "url", "type", "call_function", "internal_function", "extra string" }
 * for dynamic page use { "URL", "MIME", &page_render, FUNCTION, NULL },
 * for static pages use { "URL", "MIME", &file_render, NULL, "FILENAME.TYPE" },
 * static pages are stored in read/files
 */
 
static void __link_function_front( ReplyDataPtr rd ) {
	iohtmlFunc_front( rd, NULL, NULL );
}
static void  __link_function_login( ReplyDataPtr rd ) {
	iohtmlFunc_login( rd, NULL );
}

/*
typedef struct Page
{
  // Acceptable URL for this page.
  const char *url;

  // Mime type to set for the page. 
  const char *mime;

  //  Handler to call to generate response.  
  PageHandler handler;

  //Handler to call to generate response.
  PageFunction function;

  //Extra argument to handler.
  const void *handler_cls;
  
} PageDef, *PagePtr;
*/

//typedef void (*PageFunction)( ReplyDataPtr cnt );

/*
typedef struct ReplyData
{
   //Post processor handling form data (IF this is a POST request).
  StringBufferDef cache;

  //Associated session.
  struct Session *session;

  // URL to serve in response to this POST (if this request was a 'POST')

  struct MHD_Connection *connection;

} ReplyDataDef, *ReplyDataPtr;
*/

//	ReplyDataDef rd;

// html_page[id].function( &rd );

PageDef html_page[] =
  {
    //Basic/Main pages
    { "/", "text/html", &page_render,  __link_function_front, NULL },
    { "/index", "text/html", &page_render,  __link_function_front, NULL },
    //{ "/chat", "text/html", &file_render, NULL, "chat.html" }, //Static Page Example
    { "/chat", "text/html", &page_render,  iohtmlFunc_ircapplet, NULL },
    { "/login", "text/html",  &page_render,  __link_function_login, NULL },
    { "/notices", "text/html",  &page_render, iohtmlFunc_notices, NULL },
    { "/halloffame", "text/html",  &page_render, iohtmlFunc_halloffame, NULL },
    { "/gettingstarted", "text/html",  &page_render, iohtmlFunc_gettingstarted, NULL },
    { "/faq", "text/html",  &page_render, iohtmlFunc_faq, NULL },
	{ "/guide", "text/html",  &page_render, iohtmlFunc_guide, NULL },
    { "/status", "text/html",  &page_render, iohtmlFunc_status, NULL },
    { "/info", "text/html",  &page_render, iohtmlFunc_info, NULL },
    { "/rules", "text/html",  &page_render, iohtmlFunc_rules, NULL },
    { "/register", "text/html",  &page_render, iohtmlFunc_register, NULL },
    //Ajax pages
    { "/ajax", "text/xml",  &page_render, iohtmlFunc_ajax, NULL },
    { "/ajax.js", "text/javascript",  &page_render, iohtmlFunc_javaforajax, NULL },
    //Game Pages
    { "/rankings", "text/html",  &page_render, iohtmlFunc_rankings, NULL },
    { "/hq", "text/html",  &page_render, iohtmlFunc_hq, NULL },
    { "/news", "text/html",  &page_render, iohtmlFunc_news, NULL },
    { "/council", "text/html",  &page_render, iohtmlFunc_council, NULL },
    { "/units", "text/html",  &page_render, iohtmlFunc_units, NULL },
    { "/market", "text/html",  &page_render, iohtmlFunc_market, NULL },
    { "/planets", "text/html",  &page_render, iohtmlFunc_planets, NULL },
    { "/pltake", "text/html",  &page_render, iohtmlFunc_pltake, NULL },
    { "/empire", "text/html",  &page_render, iohtmlFunc_empire, NULL },
    { "/famaid", "text/html",  &page_render, iohtmlFunc_famaid, NULL },
    { "/famgetaid", "text/html",  &page_render, iohtmlFunc_famgetaid, NULL },
    { "/famnews", "text/html",  &page_render, iohtmlFunc_famnews, NULL },
    { "/famrels", "text/html",  &page_render, iohtmlFunc_famrels, NULL },
    { "/ministers", "text/html",  &page_render, iohtmlFunc_ministers, NULL },
    { "/map", "text/html",  &page_render, iohtmlFunc_map, NULL },
    { "/mappick", "text/html",  &page_render, iohtmlFunc_mappick, NULL },
    { "/mapadv", "text/html",  &page_render, iohtmlFunc_mapadv, NULL },
    { "/system", "text/html",  &page_render, iohtmlFunc_system, NULL },
    { "/player", "text/html",  &page_render, iohtmlFunc_player, NULL },
    { "/playerlist", "text/html",  &page_render, iohtmlFunc_playerlist, NULL },
    { "/planet", "text/html",  &page_render, iohtmlFunc_planet, NULL },
    { "/build", "text/html",  &page_render, iohtmlFunc_build, NULL },
    { "/cancelbuild", "text/html",  &page_render, iohtmlFunc_cancelbuild, NULL },
    { "/massbuild", "text/html",  &page_render, iohtmlFunc_massbuild, NULL },
	{ "/massraze", "text/html",  &page_render, iohtmlFunc_massRaze, NULL },
    { "/raze", "text/html",  &page_render, iohtmlFunc_raze, NULL },
    { "/fleets", "text/html",  &page_render, iohtmlFunc_fleets, NULL },
    { "/fleetsend", "text/html",  &page_render, iohtmlFunc_fleetsend, NULL },
    { "/fleetssplit", "text/html",  &page_render, iohtmlFunc_fleetssplit, NULL },
    { "/fleetsmerge", "text/html",  &page_render, iohtmlFunc_fleetsmerge, NULL },
    { "/fleetdisband", "text/html",  &page_render, iohtmlFunc_fleetdisband, NULL },
    { "/fleetchange", "text/html",  &page_render, iohtmlFunc_fleetchange, NULL },
    { "/fleetattack", "text/html",  &page_render, iohtmlFunc_fleetattack, NULL },
    { "/explore", "text/html",  &page_render, iohtmlFunc_explore, NULL },
	{ "/massexplore", "text/html",  &page_render, iohtmlFunc_massexplore, NULL },
	{ "/observe", "text/html",  &page_render, iohtmlFunc_observe, NULL },
    { "/attack", "text/html",  &page_render, iohtmlFunc_attack, NULL },
    { "/station", "text/html",  &page_render, iohtmlFunc_station, NULL },
    { "/spec", "text/html",  &page_render, iohtmlFunc_spec, NULL },
    { "/specinfos", "text/html",  &page_render, iohtmlFunc_specinfos, NULL },
    { "/operation", "text/html",  &page_render, iohtmlFunc_operation, NULL },
    { "/spell", "text/html",  &page_render, iohtmlFunc_spell, NULL },
    { "/incant", "text/html",  &page_render, iohtmlFunc_incant, NULL },
    { "/operationsend", "text/html",  &page_render, iohtmlFunc_operationsend, NULL },
    { "/spellsend", "text/html",  &page_render, iohtmlFunc_spellsend, NULL },
    { "/incantsend", "text/html",  &page_render, iohtmlFunc_incantsend, NULL },
    { "/research", "text/html",  &page_render, iohtmlFunc_research, NULL },
    { "/mail", "text/html",  &page_render, iohtmlFunc_mail, NULL },
    { "/forum", "text/html",  &page_render, iohtmlFunc_forum, NULL },
	{ "/observessurveys", "text/html",  &page_render, iohtmlFunc_ObservesSurveys, NULL },

    { "/search", "text/html",  &page_render, iohtmlFunc_search, NULL },
    { "/vote", "text/html",  &page_render, iohtmlFunc_vote, NULL },
    
    //User Account Pages
    { "/account", "text/html",  &page_render, iohtmlFunc_account, NULL },
    { "/delete", "text/html",  &page_render, iohtmlFunc_delete, NULL },
    { "/logout", "text/html",  &page_render, iohtmlFunc_logout, NULL },
    { "/expired", "text/html",  &page_render, iohtmlFunc_expired, NULL }, 
    { "/changepass", "text/html",  &page_render, iohtmlFunc_changepass, NULL },
    #if FACEBOOK_SUPPORT
    { "/facebook", "text/html",  &page_render, iohtmlFunc_facebook, NULL },
    #endif
    //Administration Pages
    { "/adminforum", "text/html",  &page_render, iohtmlFunc_adminforum, NULL },
    { "/moderator", "text/html",  &page_render, iohtmlFunc_moderator, NULL },
    { "/administration", "text/html",  &page_render, iohtmlFunc_administration, NULL },
    //File Rendering such as Images and Static Files
    { "/files", "text/html",  &file_render, NULL, NULL },
    //Not Found -- End of List.
    { NULL, NULL, &not_found_page, NULL, NULL } /* 404 */
};
