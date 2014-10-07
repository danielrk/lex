// Histlist is a singly-linked list 
typedef struct {
    Histlist next;
    int N; // ncmd
    token *T;
} histlist *Histlist;

// Return pointer to empty histlist 
Histlist createH (void);

// Add T to end of H
void addH (Histlist h, token *list, int ncmd);

// Destroy H 
void destroyH (Histlist h);

