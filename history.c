#include <stdlib.h>
#include "Histlist.h"

static int histsize = 0;
static Histlist hist = createH();

char *hExpand (const char *oldLine, int *status)
{
}

// Add LIST and NCMD to HIST
void hRemember(int ncmd, token *list)
{
    // get effective histsize limit    
    int limit = 323; 
    char *env = getenv("HISTSIZE");
    if (env != NULL && strtol(env, NULL, 10) > 0)
        limit = strtol(env, NULL, 10);

    // add LIST,NCMD to HIST
    addH (hist, list, ncmd);
    histsize++;

    // now assume if histsize > limit,
    // histsize == limit + 1
    if (histsize > limit)
        hist = hist->next;
}

// Free hist and reinitialize
void hClear (void) {
    destroyH(hist);
    hist = createH();
}

