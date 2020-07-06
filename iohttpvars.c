
char *iohtmlVarsFind( ReplyDataPtr cnt, char *id ) {
	PostDataPtr data;

for( data = (cnt->session)->postdata ; data ; data = data->next ) {
	if( ( strcmp( id, data->key ) == 0 ) )
		return data->value;
}


return (char *)MHD_lookup_connection_value(cnt->connection, MHD_GET_ARGUMENT_KIND, id);
}

void iohtmlVarsPrint( ReplyDataPtr cnt ) {
	PostDataPtr data;

for( data = (cnt->session)->postdata ; data ; data = data->next ) {
	printf("key %s value %s\n",data->key, data->value);
}

}


char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_HEADER_KIND, id);
}

char *iohtmlGetHeader( ReplyDataPtr cnt) {

return MHD_print_connections_values(cnt->connection, MHD_HEADER_KIND);
}

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_COOKIE_KIND, id);
}


int iohtmlVarsMapcoords( ReplyDataPtr cnt, int *coords )
{

char *string = iohtmlVarsFind( cnt, "sectorzoom" );

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





