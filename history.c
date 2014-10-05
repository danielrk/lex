#include "Histlist.h"
static int list_size = 0;
static Histlist hist;

char *hExpand (const char *oldLine, int *status)
{
}

void hRemember(int ncmd, token *list)
{
    char *histsize = getenv("HISTSIZE");

