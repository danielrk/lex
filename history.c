// #include <string.h>  // included in mainLex.c
// #include <stdlib.h>  // getLine.c includes this
#include "Histlist.h"

static long histsize = 0;
static Histlist hist = createH();

// Return recursively-expanded string
char *hExpand (const char *oldLine, int *status)
{
    int exclam = strcspn(oldLine, "!"); // null chars can't be put in stdin

    char *prefix; // oldLine before first '!'; may be empty/entire string
    char *sub;    // hist substitution if requested, otherwise NULL
    char *suffix; // substring of oldLine beyond substring to be 
                  //   substituted, if event; otherwise oldLine from first '!' to end
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

    // Convert nonprinting chars to blanks
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

    int status; // valid sub?
    Histlist h; // ptr to hist node to be substituted, or NULL 
    char *suf_start; // ptr to suffix substring in exstr

    // "!!"
    if (exstr[1] == '!') {
        *psub    = (get_nthlast(&h, 1)==0 ? stringify(h->T) : NULL);
        *psuffix = malloc (sizeof(char) * (strlen(exstr+2) + 1));
                   strcpy(*psuffix, exstr+2);
    }
    
    // "!N"
    else if (strspn(exstr+1, "0123456789")) {
        int n = strtol(exstr+1, &suf_start, 10);
        *psub = (get_cmd(&h, n)==0 ? stringify(h->T) : NULL);
        *psuffix = malloc (sizeof(char) * (strlen(suf_start) + 1));
                   strcpy(*psuffix, suf_start);
    }

    // "!-N"
    else if (exstr[1] == '-' && strspn(exstr+2, "0123456789")) {
        int n = strtol(exstr+2, &suf_start, 10);
        *psub = (get_nthlast(&h, n)==0 ? stringify(h->T) : NULL);
        *psuffix = malloc (sizeof(char) * (strlen(suf_start) + 1));
                   strcpy(*psuffix, suf_start);
    }

    // "!?[...]"
    else if (exstr[1] == '?') { 
        int string_len = strcspn(exstr+2, " !?\n");
        if (string_len) { // "!?STRING[...]"

            // "!?STRING\n" or "!?STRING?[...]"
            if (exstr[2+string_len] == '\n' ||
                exstr[2+string_len] == '?'    ) {
                
                // extract STRING
                char *s = malloc(sizeof(char) * (string_len + 1));
                strncpy (s, exstr+2, string_len);
                s[string_len] = '\0';
                
                *psub = (get_match(&h, s)==0 ? stringify(h->T) : NULL);
                *psuffix = malloc (sizeof(char) * (strlen(exstr+2+string_len) + 1));
                           strcpy(*psuffix, exstr+2+string_len);
                free(s);
            }
            else { // not an event
                *psub = NULL;
                *psuffix = malloc (sizeof(char) * (strlen(exstr) + 1));
                           strcpy (*psuffix, exstr);
                return 0;
            }
        }
        else { // not an event
            *psub = NULL;
            *psuffix = malloc (sizeof(char) * (strlen(exstr) + 1));
                       strcpy (*psuffix, exstr);
            return 0;
        }
    }

    // "![...]" not an event
    else {
        *psub = NULL;
        *psuffix = malloc (sizeof(char) * (strlen(exstr) + 1));
                   strcpy (*psuffix, exstr);
        return 0;
    }
    
    // if event, return status of substitution
    status = (*psub == NULL ? -1 : 1);
    return status;
}

// Set *pH to point to the most recent remembered command
// with a token that contains S, which is assumed to be
// a nonempty sequence of characters that does not contain
// whitespace or ! or ?; or NULL otherwise.
//
// Return 0 if such node exists, or -1 otherwise
static int get_match (Histlist *pH, char *s) {
    if (histsize == 0) {
        *pH = NULL;
        return -1;
    }
    
    int loc = -1;   // nMoves to reach target command in HIST 
    int nMoves = 0; // number of headptr moves performed
    Histlist ptr;
    for (ptr = hist; ptr != NULL; ptr = ptr->next) {
        if (isMatch(ptr->T, s))
            loc = nMoves;
        
        nMoves++;
    } 
    
    if (loc == -1) { // no such cmd
        *pH = NULL;
        return -1;
    }

    ptr = hist;      // success; return pointer to target
    for (int i = 0; i < loc; i++)
        ptr = ptr->next;

    *pH = ptr;
    return 0;
}

// Return 1 if LIST contains a token containing S
//        0 otherwise
static int isMatch (token *list, char *s) {
    token *t;
    for (t = list; t != NULL; t = t->next) {
        if (strstr(t->text, s) != NULL)   // is match?
            return 1;

    }
    return 0;
}

// Set *pH to point to the node in hist
// whose ncmd = n, or NULL
// If node exists, return 0
// Otherwise, return -1
// Assume n >= 0 
static int get_cmd (Histlist *pH, int n) {
    if (n == 0 || histsize==0) {
        *pH = NULL;
        return -1;
    }
    else {
        Histlist ptr = hist;
        for (; ptr != NULL && ptr->N != n; ptr = ptr->next) {
            if (n < ptr->N) { // N only increases down hist
                *pH = NULL;
                return -1;
            }
        }
        if (ptr == NULL) {
            *pH = NULL;
            return -1;
        }
        else { // ptr->N == n
            *pH = ptr;
            return 0;
        }
    }
}
// Set *pH to point to Nth last node in HIST 
// Return 0 if such node exists; -1 otherwise
// Assume N >= 0 
static int get_nthlast (Histlist *pH, int n)
{
    if (n == 0) {
        *pH = NULL;
        return -1;
    }
    
    int nMoves = histsize - n;
    if (nMoves < 0) { 
        *pH = NULL;
        return -1;
    }
    else { // Nth last command remembered
        Histlist ptr = hist;
        for (int i = 0; i < nMoves; i++)
            ptr = ptr->next;

        *pH = ptr;
        return 0;
    }
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
    long limit = 323; 
    char *env = getenv("HISTSIZE");
    if (env != NULL && strtol(env, NULL, 10) > 0)
        limit = strtol(env, NULL, 10);

    // add LIST,NCMD to HIST
    addH (hist, list, ncmd);
    histsize++;

    // now assume if histsize > limit,
    // histsize == limit + 1
    if (histsize > limit) {
        Histlist tmp = hist;
        hist = hist->next;
        tmp->next = NULL;
        destroyH(tmp);
    }
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








