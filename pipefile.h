#ifndef PIPEFILE_INCLUDED
#define PIPEFILE_INCLUDED

void svPipeScan(int pipefileid);

int svPipeSend(int pipedirection, char *message, ...);

#endif
