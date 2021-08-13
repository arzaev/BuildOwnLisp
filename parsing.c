#include "mpc.h"

#include <stdio.h>
#include <stdlib.h>

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

  puts("Jplang Version 0.0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  /* Create Some Parsers */
  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Str   = mpc_new("str");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Jplang    = mpc_new("jplang");

  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    str   : /[aA-zZ]+/ ;                             \
    operator : '+' | '-' | '*' | '/' | '%' ;            \
    expr     : <number> | '(' <operator> <expr>+ ')' | <str> ;  \
    jplang    : /^/ <operator> <expr>+ /$/ | <expr>* ;             \
  ",
  Number, Str, Operator, Expr, Jplang);


  while (1) {

    /* Now in either case readline will be correctly defined */
    char* input = readline("jplang> ");
    add_history(input);

	mpc_result_t r;
	if (mpc_parse("<stdin>", input, Jplang, &r)) {
		mpc_ast_print(r.output);
		mpc_ast_delete(r.output);
	} else {
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
	}
    free(input);

  }

  mpc_cleanup(4, Number, Operator, Expr, Jplang); 
  return 0;
}
