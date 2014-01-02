/* Converts a hex character to its integer value */
char from_hex(char ch);

/* Converts an integer value to its hex character*/
char to_hex(char code);

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str);

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str);


