// #include <string.h>  // included in mainLex.c
// #include <stdlib.h>  // getLine.c includes this
#include "Histlist.h"

static int histsize = 0;
static Histlist hist = createH();

// Return recursively-expanded string
char *hExpand (const char *oldLine, int *status)
{
    int exclam = strspn(oldLine, "!"); // null chars can't be put in stdin

    char *prefix; // oldLine before first '!'; may be empty/entire string
    char *sub;    // hist substitution if requested, otherwise "!"
    char *suffix; // substring of oldLine beyond substring to be 
                  //   substituted, if any; otherwise oldLine[exclam+1 to end]
                  //   can be empty string


    // PREFIX same in all cases
    prefix = malloc(sizeof(char) * (exclam + 1));
    strncpy (prefix, oldLine, exclam);
    prefix[exclam] = '\0';
    

    // Decode event
    int first_stat = get_sub_suf (oldLine+exclam, &sub, &suffix);

    if (oldLine[exclam+1] == '!') { // "!!"
        return three_cat (prefix, sub, );
    }
    else if () { // "!N"
    }
    else if () { // "!-N"
    }
    else if () { // "!?STRING?"
    }
    else { // "![x]*"; SUB = "!"
    }
    free(prefix);
    free(sub);
    free(suffix);
}

// EXSTR is a string of the form "![...]", which begins with a possible
// substitution event.
//
// Set *psub to command string in histlist (NULL if no sub)
//     *psuffix to copy of exstr beyond substitution (can be empty)
// Return 1 if EXSTR begins with valid substitution event
//        0 if EXSTR does not begin with a substitution event
//       -1 if EXSTR begins with an impossible substitution event
//
static int get_sub_suf (char *exstr, char **psub, char **psuffix) {
    int status;
    
    // "!!"
    if (exstr[1] == '!') {
        
        if (histsize >= 1) {
            *psub =;
        }
    }
}

static Histlist nthlast (Histlist h, )
{
}

// Return string of recursively concatenated 
// texts in token list, or NULL if list is NULL
static char* stringify (token *list) {
    if (list == NULL)
        return NULL;
    else {
        char *rest = stringify (list->next);
        char *result = concat (list->text, rest);
        free(rest);
        return result;
    }
}

// Return concatenation of s1 and s2, separated by blank.
// If s2 NULL, return copy of s1.
static char* concat (const char *s1, const char *s2) {
    char *result;
    if (s2 == NULL) {
        result = malloc(sizeof(char) * (strlen(s1) + 1));
        strcpy(result, s1);
    }        
    else {
        result = malloc(sizeof(char) * (strlen(s1)+strlen(s2)+2));
        strncpy(result, s1, strlen(s1));
        result[strlen(s1)] = ' ';
        strcpy(result+strlen(s1)+1, s2);
    }
    return result;
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








