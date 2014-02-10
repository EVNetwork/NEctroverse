#ifndef PIPEFILE_H
#define PIPEFILE_H

void svPipeScan(int pipefileid);

int svPipeSend(int pipedirection, char *message, ...);

#endif
