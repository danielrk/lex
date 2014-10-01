// history.h                                      Stan Eisenstat (09/22/14)
//
// Header file for history mechanism used in Lex.

#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED        // history.h has been #include-d

#include "lex.h"

//  hExpand() returns a copy of the string OLDLINE in which any substring of
//  the command line of the form !! or !N or !-N or !?STRING? as follows:
//
//    !!        -> last remembered command
//
//    !N        -> N-th command read (N is any sequence of digits)
//
//    !-N       -> N-th last remembered command (N is any sequence of digits)
//
//    !?STRING? -> most recent remembered command with a token that contains
//                 STRING (= a nonempty sequence of characters that does not
//                 contain whitespace or !  or ?)
//
//  The replacement string consists of the specified sequence of tokens
//  separated by single blanks.
//
//  If the notation above is followed by a *, then the leading token in the
//  command is omitted.  If it is followed by a ^, then only the first token is
//  used.  If it is followed by a :M (where M is any sequence of digits), then
//  only the M-th token is used.  If it is followed by a $, then only the last
//  token is used.
//
//  The search for substrings to expand proceeds from left to right and
//  continues after each replacement string.  If the requested substitution
//  is impossible, then the substring is deleted, but the search continues.
//  Storage for the new line is allocated by malloc()/realloc().  The string
//  OLDLINE is not modified.
//
//  hExpand() sets STATUS to 1 if substitutions were made and all succeeded, to
//  0 if no substitutions were requested, and to -1 if some substitution failed
//  (in which case NULL is returned).

char *hExpand (const char *oldLine, int *status);


// hRemember() adds (a copy of) the command represented by the token list LIST
// to the list of remembered commands as the NCMD-th command (where NCMD = 1 on
// the first invocation and increases by one on every subsequent invocation).

void hRemember (int ncmd, token *list);


// hClear() reinitializes the list of remembered commands, freeing all
// associated storage.

void hClear (void);


// hDump(n) writes the most recent N remembered commands by increasing number
// to the standard output using something equivalent to
//
//   printf ("%6d %s %s ... %s\n", icmd, token0, token1, ..., tokenLast)
//
// where ICMD is the number of the command and TOKEN0, ..., TOKENLAST are its
// tokens.

void hDump (int n);

#endif
