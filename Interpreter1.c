
/* A simple interpreter */

#include <stdio.h>  /* include declarations for i/o routines */
#include <ctype.h>  /* ... and for character test routines */
#include <stdlib.h> /*and for some standard routines, such as exit*/


#define NONE   -1

#define NUM    256
#define DONE   260

void emit (int t, int tval);  /*  generates output  */
void error(char* m);  /* generates all error messages  */

//-----------------------------------------------------

/* lexer.c */

int  tokenval = NONE;

int lexan (){  /*  lexical analyzer  */
  int t;
  while(1) {
    t = getchar ();
    if (t == ' ' || t == '\t')
      ;  /*  strip out white space  */
    else if (isdigit (t)) {  /*  t is a digit  */
      ungetc(t, stdin);
      scanf("%d", &tokenval);
      return NUM;
    }
    else if (t == EOF)
      return DONE;
    else if (t == '*' || t == '+') {
      tokenval = NONE;
      return t;
    }
    else if (t == '\n')
      return DONE;
    else {  /*  any character */
      error("compiler error");
    }
  }
}


//-----------------------------------------------------

/* parser.c -- without the optimizations */

int lookahead;

void match(int);
void start(), list(), expr(), moreterms(), term(), morefactors(), factor();

void parse()  /*  parses and translates expression list  */
{
  lookahead = lexan();
  start();
}

void start ()
{
  expr(); match(DONE);
}

void expr ()
{
  term(); moreterms();
}

void moreterms()
{
  if (lookahead == '+') {
    match('+'); term(); emit('+', tokenval); moreterms();
  }
  else if (lookahead == '-') {
    match('-'); term(); emit('-', tokenval); moreterms();
  }
  else {
    /* Empty */
  }
}

void term ()
{
  factor(); morefactors();
}

void morefactors ()
{
  if (lookahead == '*') {
    match('*'); factor(); emit('*', tokenval); morefactors();
  }
  else if (lookahead == '/') {
    match('/'); factor(); emit('/', tokenval); morefactors();
  }
  else if (lookahead == '%') {
    match('%'); factor(); emit('%', tokenval); morefactors();
  }
  else {
    /* Empty */
  }
}

void factor ()
{
  if (lookahead == '(') {
    match('('); expr(); match(')');
  }
  else if (lookahead == NUM) {
    int num_value = tokenval;
    match(NUM);
    emit(NUM, num_value);
  }
  else
    error("syntax error in factor");
}

void match(int t)
{
  if (lookahead == t)
    lookahead = lexan();
  else
    error ("syntax error in match");
}

//-----------------------------------------------------

/* emitter.c */
void emit (int t, int tval)  /*  generates output  */
{
  switch(t) {
  case '+' : case '-' : case '*' : case '/': case '%': case '^':
    printf("%c\n", t); break;
  case NUM:
    printf("%d\n", tval); break;
  default:     
    printf("token %d, tokenval %d\n", t, tval);
  }
}

//-----------------------------------------------------
/* error.c */


void error(char* m)  /* generates all error messages  */
{
  exit(EXIT_FAILURE);  /*  unsuccessful termination  */
}


//-----------------------------------------------------

/* main.c */


int main( )
{
  printf("\n Welcome to my Interpreter!");
  printf("\nEnter an integer expression:\n");
  parse();
  exit(0);    /*  successful termination  */
}



