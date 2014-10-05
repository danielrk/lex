// Histlist is a singly-linked list 
struct histlist {
    Histlist next;
    int N; // ncmd
    token *T;
}

// Return empty Histlist
Histlist createH (void) {
    return NULL;
}

// Add a copy of LIST to the beginning of H
void addH (Histlist h, token *list, int ncmd) {
    token *copy = copyT(list);
    Histlist new_node = makeNode(copy, ncmd);
    if (h == NULL)
        *h = *new_node;
    else {
        Histlist ptr = h;
        while (ptr->next != NULL)
            ptr = ptr->next;

        ptr->next = new_node;
    }
}

// Return copy of token list recursively
static token* copyT (token *list) {
    if (list == NULL)
        return NULL;
    else {
        token *copy = malloc (sizeof(token));
        
        // copy text - assumes TEXT string is not empty
        textcpy = malloc (sizeof(char) * (strlen(list->text)+1));
        strcpy (textcpy, list->text);
        copy->text = textcpy;

        // copy type
        copy->type = list->type;
        
        // copy next
        copy->next = copyT(list->next);
    }
}


// Return pointer to malloc'd histlist
static Histlist makeNode (token *list, int ncmd) {
    Histlist h = malloc(sizeof(histlist));
    h->next = NULL;
    h->N = ncmd;
    h->T = list;
    return h;
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


    







