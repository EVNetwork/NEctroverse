#include "md5.c"
#include "base64.c"

void saltgen( char salt[SALT_SIZE] ) {
	int i;
	const char *const seedchars = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	unsigned long seed[2];
	char md5sum[MD5_HASHSUM_SIZE];
	char random[33], urandom[9];
	FILE *fh;

strcpy(salt,"$1$");

for(i = 0; i < 6; ++i) {
	urandom[i] = rand() % (126 - 33 + 1) + 33;
}
urandom[i] = '\0';

md5_string( urandom, md5sum );
strcat( salt, md5sum );

if( (fh = fopen("/dev/urandom", "rb")) ) {
	if( fread( &random, 1, 32, fh ) < 1 ) {
		critical( "Error Getting random for MD5... Critical failure." );
	}
	fclose(fh);
}

seed[0] = (long)random;
seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

for (i = 0; i < 6; i++)
	salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];
	
return;
}

//Time to add some hash protection to our passwords.
static StringBufferPtr enc_buffer;

char *hashencrypt( char *passhash ) {
	char *enc;
	char md5sum[MD5_HASHSUM_SIZE], salt[SALT_SIZE];

if( enc_buffer ) {
	enc_buffer->off = 0;
	memset( enc_buffer->buf, 0, enc_buffer->buf_len );
} else if( NULL == ( enc_buffer = calloc( 1, sizeof(StringBufferDef) ) ) ) {
	critical( "memory allocation error!" );
	return NULL;
}
md5_string( passhash, md5sum );
saltgen( salt );
enc = crypt( md5sum, salt );

AddBufferString( enc_buffer, enc );
enc_buffer->buf[enc_buffer->off] = '\0';

return enc_buffer->buf;
}

//Good stuff, now to check those encypted passwords.
int checkencrypt( char *passentered, char *passcheck ) {
	char *enc;
	char md5sum[MD5_HASHSUM_SIZE];

md5_string( passentered, md5sum );
enc = crypt( md5sum, passcheck );

return ( ( strcmp( enc, passcheck) == 0 ) ? 1 : 0 );
}





