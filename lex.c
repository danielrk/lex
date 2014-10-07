#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include "/c/cs323/Hwk4/lex.h"

// Return type of token specified by META, 
// which is a null-terminated string consisting
// of the text of a single metacharacter token
static int get_type (char *meta) {
    if (strcmp(meta, "<")==0)
        return REDIR_IN;
    if (strcmp(meta, "<<")==0)
        return REDIR_HERE;
    if (strcmp(meta, "|")==0)
        return REDIR_PIPE;
    if (strcmp(meta, ">")==0)
        return REDIR_OUT;
    if (strcmp(meta, ">>")==0)
        return REDIR_APP;
    if (strcmp(meta, ";")==0)
        return SEP_END;
    if (strcmp(meta, "&")==0)
        return SEP_BG;
    if (strcmp(meta, "&&")==0)
        return SEP_AND;
    if (strcmp(meta, "||")==0)
        return SEP_OR;
    if (strcmp(meta, "(")==0)
        return PAREN_LEFT;
    if (strcmp(meta, ")")==0)
        return PAREN_RIGHT;

    return EXIT_FAILURE; // bad input
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
        text[0] = s[0];
        text[1] = '\0';
        if (strspn(s, text) >= 2) { // double the char
            text[1] = text[0];
            text[2] = '\0';
        }
    }            

    *t_type = get_type(text);
    return text;
}

    
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

    token *list = malloc(sizeof(*list));    // pointer to immediate token
    char *s;        // token text
    int t_type;     // token type
    
    // Skip blanks (spaces, tabs, newlines, etc.)
    while (*p != '\0' && isspace(*p))
        p++;

    // "[nonblank]+..." or ""
    if (*p == '\0')
        return NULL;
   

    // TOKEN EXISTS
    // P now fixed at beginning of token
    // "[nonblank]+..."

    // metachar token
    else if (strspn(p, METACHARS)) {
        s = metatext(p, &t_type);
    }

    // simple nonblanks
    else {
        // take everything until
        // metachar, blank, newline, null, etc. 
        int s_len = strcspn(p, "<>;&|() \t\n\v\f\r\0");
        s = malloc(sizeof(char) * (s_len + 1));
        strncpy(s, p, s_len);
        s[s_len] = '\0';
        t_type = SIMPLE;
    }


    list->text = s;
    list->type = t_type;
    list->next = lex(p+strlen(s));
    
    return list;
}

    

