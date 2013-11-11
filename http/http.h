
extern MHD_DaemonPtr server;

void call_clean();

int main_clone();

extern char httpbuffer[ 256 * 1024 ];

extern void expire_sessions ();

extern int create_response (void *cls, MHD_ConnectionPtr connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr);

extern void request_completed_callback (void *cls, MHD_ConnectionPtr connection, void **con_cls, enum MHD_RequestTerminationCode toe);

