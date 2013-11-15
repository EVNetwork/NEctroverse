

char *iohtmlVarsFind( ReplyDataPtr cnt, char *id ) {
	int a;
	char *value;

for( a = 0; a < (cnt->session)->posts; a++ ) {
	if( ( (cnt->session)->key[a] ) && ( strcmp( id, (cnt->session)->key[a] ) == 0 ) ) {
		value = (cnt->session)->value[a];
		(cnt->session)->key[a] = NULL;
		(cnt->session)->value[a] = NULL;
		return value;
	}
}

value = (char *)MHD_lookup_connection_value(cnt->connection, MHD_GET_ARGUMENT_KIND, id);
if( ( value ) && strlen(value) ) {
	return value;
}

return 0;
}

char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_HEADER_KIND, id);
}

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_COOKIE_KIND, id);
}

int iohtmlCookieAdd( ReplyDataPtr cnt, char *name, char *value, ... ) {
	char cstr[256];
	char text[256];
	va_list ap;

va_start( ap, value );
vsnprintf( text, 256, value, ap );
va_end( ap );

snprintf(cstr, sizeof (cstr), "%s=%s", name, text);

strncpy((cnt->cookies).value[(cnt->cookies).num], cstr, sizeof(cstr) );
(cnt->cookies).num = (cnt->cookies).num + 1;
	

return (cnt->cookies).num;
}


/*
//FIXME
int iohttpVarsMapcoords( svConnectionPtr cnt, int *coords )
{
  iohttpDataPtr iohttp = cnt->iodata;
  char *string;
  if( iohttp->method == 3 )
    string = iohttp->content;
  else
    string = iohttp->query_string;
  if( !( string ) )
    return 0;
  if( sscanf( string, "%d", &coords[0] ) != 1 )
    return 0;
  string = ioCompareFindWords( string, "," );
  if( !( string ) )
    return 0;
  if( sscanf( string, "%d", &coords[1] ) != 1 )
    return 0;
  return 1;
}
*/




