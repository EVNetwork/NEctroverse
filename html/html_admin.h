#ifndef HTML_ADMIN_H
#define HTML_ADMIN_H


void iohtmlFunc_AdminMenu( ReplyDataPtr cnt );

void iohtmlFunc_admin( ReplyDataPtr cnt );
void iohtmlFunc_moderator( ReplyDataPtr cnt );



int iohtmlAdminAction( int act );


void iohtmlFunc_oldadmin( ReplyDataPtr cnt );
void iohtmlFunc_moderator( ReplyDataPtr cnt );

void iohtmlFunc_adminframe( ReplyDataPtr cnt );
void iohtmlFunc_adminmenu( ReplyDataPtr cnt );
void iohtmlFunc_adminforum( ReplyDataPtr cnt );

#endif
