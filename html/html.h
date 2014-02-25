#ifndef HTML_H
#define HTML_H
//The above line MUST STAY HERE! -- This prevents double calling.

#include "html_main.h"
#include "html_gameplay.h"
#include "html_admin.h"
#include "html_user.h"
#include "html_info.h"
#include "html_forum.h"
#include "html_ajax.h"

#if FACEBOOK_SUPPORT
#include "html_facebook.h"
#endif


void iohttpFunc_Approve( ReplyDataPtr cnt );
char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen);

#endif
