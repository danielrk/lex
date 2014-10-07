/* Interface for the Histlist object,
 * a linked-list of text, ncmd
 * pairs.
 *
 * Daniel Kim 10/7/14
 */

#include "/c/cs323/Hwk4/lex.h"


// Histlist is a singly-linked list 
typedef struct histlist *Histlist;
struct histlist {
    Histlist next;
    int N; // ncmd
    token *T;
};



// Return pointer to empty histlist 
Histlist createH (void);



// Add T to end of H
void addH (Histlist *pH, token *list, int ncmd);



// Destroy H 
void destroyH (Histlist h);

