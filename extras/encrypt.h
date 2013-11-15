#ifndef ENCRYPT_H
#define ENCRYPT_H

char *saltgen();

char *hashencrypt( char *pass2hash );

int checkencrypt( char *passentered, char *passcheck );

#endif
