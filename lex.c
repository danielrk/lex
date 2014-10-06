// Return a recursively built linked list of
// tokens in LINE, or NULL if no tokens
//
// LINE assumed to lack nonprinting chars,
// except tabs and terminating newline; 
// i.e. LINE consists of some sequence of: 
//      printing chars (incl. spaces, metachars, others)
//      tabs
//      terminating newline
// Note: line can be empty string
token *lex (const char *line) {

    char *p = line; // pointer along line

    token *list;    // pointer to immediate token
    char *s;        // token text
    int t_type;     // token type
    
    // Skip blanks (spaces, tabs)
    while (*p != '\0' && isblank(*p))
        p++;

    // "[nonblank]+..." or "" or "\n"
    if (*p == '\0' || *p == '\n')
        return NULL;
    
    // "[nonblank]+..."

    // metachar token
    else if (strspn(p, METACHARS)) {
        s = metatext(p, &t_type);
    }

    // simple nonblanks
    else {


    list->text = s;
    list->type = ;
    list->next = lex( )

}

// Return copy of initial substring token
// of metachars, and set *T_TYPE to token type
// S is in the form "[META]+..."
static char* metatext(char *s, int *t_type) {
    
    // token text at most length 2
    char *text = malloc(sizeof(char) * 3); 

    // among ";()"
    if (strspn(s, ";()")) {
        text[0] = *s;
        text[1] = '\0';

    }

    // among "<>&|", need to check single/double
    else {
        char *single = malloc(sizeof(char) * 2);
        single[0] = s[0];
        single[1] = '\0';

        int size = strspn(s, single); // 1 or >=2
        
    return text;
}

// Return type of token specified by META, 
// which is a null-terminated string consisting
// of the text of a single metacharacter token
static int get_type (char *meta) {
    if (strcmp(

}
    

    
    

