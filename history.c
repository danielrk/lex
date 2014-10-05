// #include <stdlib.h>  // getLine.c includes this
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

// Write most recent N commands by increasing number
// to stdout
void hDump (int n) {
    if (n < 0)
        fprintf (stderr, "%s = %d\n", "hDump arg", 2);
    else if (n == 0)
        ;
    else {
        nMoves = histsize - n;            // histlist head ptr moves nMoves times
        nMoves = (nMoves < 0 ? 0:nMoves); // force nMoves >=0
        Histlist p = hist;

        for (int i = 0; i < nMoves; i++) {
            if (p == NULL) {
                fprintf (stderr, "histsize = %d but hist==NULL after %d nexts\n",
                        histsize, i);
                exit(EXIT_FAILURE);
            }
            else
                p = p->next;
        }

        int nPrints = histsize - nMoves;
        for (; p != NULL; p=p->next) {
            print_tlist(p->T, p->N);
            nPrints--;
        }
        if (nPrints != 0) {
            fprintf(stderr, "hDump not printing right #\n");
            exit(EXIT_FAILURE);
        }
    }
}

// Print a command (list of tokens) in the following form:
//     printf ("%6d %s %s ... %s\n", icmd, token0, token1, ..., tokenLast)
static void print_tlist (token *list, int icmd) {
    if (list == NULL) {
        fprintf (stderr, "print_tlist NULL token\n");
        exit (EXIT_FAILURE);
    }

    printf ("%6d",icmd);
    token *t;
    for (t = list; t != NULL; t = t->next) {
        printf(" %s", t->text);
    }
    printf ("\n");
}








