// lex.h                                          Stan Eisenstat (09/22/14)
//
// Header file for command-line lexer.

#ifndef LEX_INCLUDED
#define LEX_INCLUDED            // lex.h has been #include-d

// A token is one of
//
//  1) a maximal contiguous sequence of nonblank printing characters other
//       than the metacharacters <, >, ;, &, |, (, and ).
//
//  2) a redirection symbol (<, <<, >, >>, or |);
//
//  3) a command separator (;, &, &&, or ||)
//
//  4) a left or right parenthesis (used to group commands).
//
//  The token type is specified by the symbolic constants defined below.


#define SIMPLE          10      // Maximal contiguous sequence of nonblanks
				 
#define REDIR_IN        20      // <
#define REDIR_HERE      21      // <<

#define REDIR_PIPE      30      // |
#define REDIR_OUT       31      // >
#define REDIR_APP       32      // >>
				 
#define SEP_END         40      // ;
#define SEP_BG          41      // &
#define SEP_AND         42      // &&
#define SEP_OR          43      // ||
				 
#define PAREN_LEFT      50      // (
#define PAREN_RIGHT     51      // )

#define METACHARS       "<>;&|()"       // Chars that start a non-SIMPLE


// A token list is a headless linked list of typed tokens.  All storage is
// allocated by malloc() / realloc().

typedef struct token {          // Struct for each token in linked list
  char *text;                   //   String containing token
  int type;                     //   Corresponding type
  struct token *next;           //   Pointer to next token in linked list
} token;


// lex() breaks the string LINE into a headless linked list of typed tokens
// and returns a pointer to the first token (or NULL if none were found).

token *lex (const char *line);


// Free list of tokens LIST (implemented in mainLex.c)
void freeList (token *list);

#endif
