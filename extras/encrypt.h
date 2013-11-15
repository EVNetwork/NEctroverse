#ifndef ENCRYPT_H
#define ENCRYPT_H

extern char *saltgen();

extern char *hashencrypt( char *pass2hash );

extern int checkencrypt( char *passentered, char *passcheck );

#endif
