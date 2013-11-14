
/**
 * List of all pages served by this HTTP server.
 * { "url", "type", "call_function", "internal_function", "extra string" }
 * Most pages will just use "page_render", but a few with variable input will use "key_page"
 * As page_render simply calls the function listed, and will cause issues if the function requires paramaters
 */
PageDef pages[] =
  {
    //Basic/Main pages
    { "/", "text/html", &key_page, NULL, NULL },
    { "/login", "text/html",  &key_page, NULL, "login" },
    { "/halloffame", "text/html",  &page_render, iohtmlFunc_halloffame, NULL },
    { "/gettingstarted", "text/html",  &page_render, iohtmlFunc_gettingstarted, NULL },
    { "/faq", "text/html",  &page_render, iohtmlFunc_faq, NULL },
    { "/status", "text/html",  &page_render, iohtmlFunc_status, NULL },
    { "/races", "text/html",  &page_render, iohtmlFunc_races, NULL },
    { "/register", "text/html",  &page_render, iohtmlFunc_register, NULL },
    { "/register2", "text/html",  &page_render, iohtmlFunc_register2, NULL },
    { "/register3", "text/html",  &page_render, iohtmlFunc_register3, NULL },
    //Generic
    { "/ajax", "text/xml",  &page_render, iohtmlFunc_ajax, NULL },
    { "/ajax.js", "text/javascript",  &page_render, iohtmlFunc_javaforajax, NULL },
    { "/style.css", "text/css",  &file_page, NULL, "/style.css" },
    { "/javascript.js", "text/javascript",  &file_page, NULL, "/javascript.js" },
    { "/stats", "text/html",  &file_page, NULL, "/stats.html" },
    { "/chat", "text/html",  &file_page, NULL, "/chat.html" },
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
    { "/famleader", "text/html",  &page_render, iohtmlFunc_famleader, NULL },
    
    { "/map", "text/html",  &page_render, iohtmlFunc_map, NULL },
    { "/mappick", "text/html",  &page_render, iohtmlFunc_mappick, NULL },
    { "/mapadv", "text/html",  &page_render, iohtmlFunc_mapadv, NULL },
    { "/system", "text/html",  &page_render, iohtmlFunc_system, NULL },
    { "/player", "text/html",  &page_render, iohtmlFunc_player, NULL },
    { "/playerlist", "text/html",  &page_render, iohtmlFunc_playerlist, NULL },
    { "/planet", "text/html",  &page_render, iohtmlFunc_planet, NULL },


    { "/vote", "text/html",  &page_render, iohtmlFunc_vote, NULL },
    
    //User Account Pages
    { "/account", "text/html",  &page_render, iohtmlFunc_account, NULL },
    { "/delete", "text/html",  &page_render, iohtmlFunc_delete, NULL },
    { "/logout", "text/html",  &page_render, iohtmlFunc_logout, NULL },
    { "/changepass", "text/html",  &page_render, iohtmlFunc_changepass, NULL },
    //Administration Pages
    { "/administration", "text/html",  &page_render, iohtmlFunc_adminframe, NULL },
    { "/adminmenu", "text/html",  &page_render, iohtmlFunc_adminmenu, NULL },
    { "/adminforum", "text/html",  &page_render, iohtmlFunc_adminforum, NULL },
    //Not Found.
    { NULL, NULL, &not_found_page, NULL, NULL } /* 404 */
  };


