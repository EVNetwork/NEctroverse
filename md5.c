#ifndef GLOBALINCLUDED
#include "global.h"
#endif

//Time to add some hash protection to our passwords.
char *hashencrypt( char *passhash ) {
	int i;
	char *password;
	const char *const seedchars = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	unsigned long seed[2];
	char salted[128];
	char urandom[9];
	char random[33];
	FILE *fh;     

strcpy(salted,"$1$");
srand((unsigned int) time(0));  
   
for(i = 0; i < 6; ++i) {
	urandom[i] = rand() % (126 - 33 + 1) + 33;
}
urandom[i] = '\0';

strcat(salted, str2md5(urandom) );

if( (fh = fopen("/dev/urandom", "rb")) ) {
	if( fread( &random, 1, 32, fh ) != 1 ) {
	 	if( options.verbose )
			printf("Error Getting random for MD5... Critical failure.\n");
		syslog(LOG_ERR, "Error Getting random for MD5... Critical failure." );
	}
	fclose(fh);
}

seed[0] = (long)random;
seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);
    
for (i = 0; i < 6; i++)
	salted[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];
//Well, we have "spare" MD5... lets use it.
password = crypt( str2md5(passhash), salted);

return password;
}

//Good stuff, now to check those encypted passwords.
int checkencrypt( char *passentered, char *passcheck ) {
	char *result;
	int ok;
//Well, we have "spare" MD5... lets use it.
result = crypt( str2md5(passentered), passcheck);
ok = ( strcmp(result, passcheck) == 0 );

return ( ok ? 1 : 0 );
}


//Do we need to create a MD5 string sum?
char *str2md5(const char *str) {
	int i, length;
	char *out = (char*)malloc(33);
	unsigned char digest[16];
	MD5_CTX c;

length = strlen(str);
MD5_Init(&c);

while(length > 0) {
	if (length > 512) {
		MD5_Update(&c, str, 512);
	} else {
		MD5_Update(&c, str, length);
	}
	length -= 512;
	str += 512;
}

MD5_Final(digest, &c);

for(i = 0; i < 16; ++i) {
	snprintf(&(out[i*2]), 16*2, "%02x", (unsigned int)digest[i]);
}


return out;
}


//Well, we can MD5 a string... so why not a file.
char *md5file( char *filename ) {
	int bytes;
	int i;
	char *out = (char*)malloc(33);
	unsigned char data[1024];
	unsigned char digest[MD5_DIGEST_LENGTH];
	FILE *inFile = fopen (filename, "rb");
	MD5_CTX mdContext;

if (inFile == NULL) {
	syslog(LOG_ERR, "Error opening file for md5 hash: \"%s\"\n", filename );
        printf ("%s can't be opened.\n", filename);
        return 0;
}

MD5_Init(&mdContext);

while( (bytes = fread (data, 1, 1024, inFile)) != 0 )
	MD5_Update(&mdContext, data, bytes);
fclose(inFile);
MD5_Final(digest,&mdContext);

for(i = 0; i < 16; ++i) {
	snprintf(&(out[i*2]), 16*2, "%02x", (unsigned int)digest[i]);
}

return out;
}



