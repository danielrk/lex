/* An implementation of the history list
 * object as a linked-list, as specified 
 * by Histlist.h
 * 
 * Daniel Kim 10/7/14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/c/cs323/Hwk4/lex.h"
#include "Histlist.h"


// Return empty Histlist
Histlist createH (void) {
    return NULL;
}




// Return copy of token list recursively
static token* copyT (token *list) {
    if (list == NULL)
        return NULL;
    else {
        token *copy = malloc (sizeof(token));
        
        // copy text - TEXT string must not be empty
        if (list->text == NULL) {
            fprintf (stderr, "error: token text empty\n");
            exit(EXIT_FAILURE);
        }

        char *textcpy = malloc (sizeof(char) * (strlen(list->text)+1));
        strcpy (textcpy, list->text);
        copy->text = textcpy;

        // copy type
        copy->type = list->type;
        
        // copy next
        copy->next = copyT(list->next);

        return copy;
    }
}




// Return pointer to malloc'd histlist
static Histlist makeNode (token *list, int ncmd) {
    Histlist h = malloc (sizeof(*h));
    h->next = NULL;
    h->N = ncmd;
    h->T = list;
    return h;
}





// Add a copy of LIST to the beginning of H
void addH (Histlist *pH, token *list, int ncmd) {
    token *copy = copyT(list);
    Histlist new_node = makeNode(copy, ncmd);
    if (*pH == NULL) 
        *pH = new_node;
    else {
        Histlist ptr = *pH;
        while (ptr->next != NULL)
            ptr = ptr->next;

        ptr->next = new_node;
    }
}




// Free nodes recursively
void destroyH (Histlist h) {
    if (h == NULL)
        ;
    else {
        freeList(h->T);
        destroyH(h->next);
        free(h);
    }
}

