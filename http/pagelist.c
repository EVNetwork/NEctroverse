
/**
 * List of all pages served by this HTTP server.
 * { "url", "type", "call_function", "internal_function", "extra string" }
 * Most pages will just use "page_render", but a few with variable input will use "key_page"
 * As page_render simply calls the function listed, and will cause issues if the function requires paramaters
 */
 
void front_define( ReplyDataPtr rd ) {
	iohtmlFunc_front( rd, NULL, NULL );
}
void login_define( ReplyDataPtr rd ) {
	iohtmlFunc_login( rd, false, NULL );
}

PageDef html_page[] =
  {
    //Basic/Main pages
    { "/", "text/html", &page_render, front_define, NULL },
    { "/login", "text/html",  &page_render, login_define, NULL },
    { "/notices", "text/html",  &page_render, iohtmlFunc_notices, NULL },
    { "/halloffame", "text/html",  &page_render, iohtmlFunc_halloffame, NULL },
    { "/gettingstarted", "text/html",  &page_render, iohtmlFunc_gettingstarted, NULL },
    { "/faq", "text/html",  &page_render, iohtmlFunc_faq, NULL },
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
    { "/main", "text/html",  &page_render, iohtmlFunc_main, NULL },
    { "/menu", "text/html",  &page_render, iohtmlFunc_menu, NULL },
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
    { "/raze", "text/html",  &page_render, iohtmlFunc_raze, NULL },
    { "/fleets", "text/html",  &page_render, iohtmlFunc_fleets, NULL },
    { "/fleetsend", "text/html",  &page_render, iohtmlFunc_fleetsend, NULL },
    { "/fleetssplit", "text/html",  &page_render, iohtmlFunc_fleetssplit, NULL },
    { "/fleetsmerge", "text/html",  &page_render, iohtmlFunc_fleetsmerge, NULL },
    { "/fleetdisband", "text/html",  &page_render, iohtmlFunc_fleetdisband, NULL },
    { "/fleetchange", "text/html",  &page_render, iohtmlFunc_fleetchange, NULL },
    { "/fleetattack", "text/html",  &page_render, iohtmlFunc_fleetattack, NULL },
    { "/explore", "text/html",  &page_render, iohtmlFunc_explore, NULL },
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

    { "/search", "text/html",  &page_render, iohtmlFunc_search, NULL },
    { "/vote", "text/html",  &page_render, iohtmlFunc_vote, NULL },
    
    //User Account Pages
    { "/account", "text/html",  &page_render, iohtmlFunc_account, NULL },
    { "/delete", "text/html",  &page_render, iohtmlFunc_delete, NULL },
    { "/logout", "text/html",  &page_render, iohtmlFunc_logout, NULL },
    { "/changepass", "text/html",  &page_render, iohtmlFunc_changepass, NULL },
    #if FACEBOOK_SUPPORT
    { "/facebook", "text/html",  &page_render, iohtmlFunc_facebook, NULL },
    #endif
    //Administration Pages
    { "/administration", "text/html",  &page_render, iohtmlFunc_adminframe, NULL },
    { "/adminmenu", "text/html",  &page_render, iohtmlFunc_adminmenu, NULL },
    { "/adminforum", "text/html",  &page_render, iohtmlFunc_adminforum, NULL },
    { "/moderator", "text/html",  &page_render, iohtmlFunc_moderator, NULL },
    { "/admin", "text/html",  &page_render, iohtmlFunc_oldadmin, NULL },
    //Extras
    { "/files", "text/html",  &file_render, NULL, NULL },
    //Not Found.
    { NULL, NULL, &not_found_page, NULL, NULL } /* 404 */
  };


