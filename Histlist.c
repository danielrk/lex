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

// Add LIST to the beginning of H
void addH (Histlist h, token *list, int ncmd) {
    Histlist new_node = makeNode(list, ncmd);
    if (h == NULL)
        *h = *new_node;
    else {
        Histlist ptr = h;
        while (ptr->next != NULL)
            ptr = ptr->next;

        ptr->next = new_node;
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

// Free nodes
void destroyH (Histlist h) {
    if (h != NULL) {
        if (h->next == NULL) {
            free (h) //////////
        Histlist temp;
        while (h->next != NULL) {
            temp = h->next;



    







