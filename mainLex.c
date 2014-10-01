// mainLex.c                                      Stan Eisenstat (09/22/14)
//
// Main program for Lex (see specification for more details)

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "history.h"
#include "lex.h"
#include "getLine.h"

int main (int argc, char *argv[])
{
    int ncmd = 1;                   // Command number
    char *line;                     // Initial command line
    char *cmd;                      // Command after history expansion
    int status;                     // Status of expansion
    token *list;                    // Linked list of tokens in command
    void process (token *);
    void freeList (token *);

    for (;;)  {
	printf ("(%d)$ ", ncmd);                // Prompt for command
	if ((line = getLine(stdin)) == NULL)    // Read line
	    break;                              // Break on end of file
									  
	cmd = hExpand (line, &status);          // Expand line
	free (line);
	if (status > 0)                         // Substitution?
	    fputs (cmd, stdout);                //   Print expanded line
	else if (status < 0)
	    fputs ("substitution failed\n", stderr);

	list = lex (cmd);                       // Lex line into tokens
	free (cmd);
	if (list == NULL)                       // Empty token list?
	    continue;
									  
	hRemember (ncmd, list);                 // Remember command
	if (status >= 0)                        // No substitution error?
	    process (list);                     //   Process token list

	freeList (list);                        // Free token list
	ncmd++;                                 // Adjust prompt
    }

    hClear();                                   // Free all storage
    return 0;
}

// -- PROCESS MODULE -------------------------------------------------------

// Print list of tokens LIST
void process(struct token *list)
{
    struct token *p;
    void freeList (token *);

    for (p = list;  p != NULL;  p = p->next) {  // Walk down linked list
	printf ("%s:%d ", p->text, p->type);    //   printing token and type
    }
    putchar ('\n');                             // Terminate line
						
    // history command dumps most recent 9 commands
    if (list && !strcmp (list->text, "history"))
	hDump(9);

    // hclear command clears history
    else if (list && !strcmp (list->text, "hclear"))
	hClear();
}


// Free list of tokens LIST
void freeList (token *list)
{
    token *p, *pnext;
    for (p = list;  p;  p = pnext)  {
	pnext = p->next;
	free(p->text);
	free(p);
    }
}
