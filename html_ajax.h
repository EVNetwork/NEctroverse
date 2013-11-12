#ifndef AJAXINCLUDED
#define AJAXINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.

void iohttpFunc_ajax( svConnectionPtr cnt );

void iohttpFunc_javaforajax( svConnectionPtr cnt );

void iohtmlFunc_ajax( ReplyDataPtr cnt );

void iohtmlFunc_javaforajax( ReplyDataPtr cnt );

#endif
