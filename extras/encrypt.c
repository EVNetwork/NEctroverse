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
char *hashencrypt( char *passhash ) {
	char md5sum[MD5_HASHSUM_SIZE], salt[SALT_SIZE];
	struct crypt_data data_crypt_buff;

data_crypt_buff.initialized = 0;
md5_string( passhash, md5sum );
saltgen( salt );

return crypt_r( md5sum, salt, &data_crypt_buff );
}

//Good stuff, now to check those encypted passwords.
int checkencrypt( char *passentered, char *passcheck ) {
	char md5sum[MD5_HASHSUM_SIZE];
	struct crypt_data data_crypt_check;

data_crypt_check.initialized = 0;
md5_string( passentered, md5sum );

return ( ( strcmp( crypt_r( md5sum, passcheck, &data_crypt_check), passcheck) == 0 ) ? 1 : 0 );
}





