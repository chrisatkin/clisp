#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "mpc.h"

int main(int argc, char** argv) {
  // create some parers
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expression = mpc_new("expression");
  mpc_parser_t* Clisp = mpc_new("clisp");

  mpca_lang(MPC_LANG_DEFAULT,
  "                                                                \
    number      : /-?[0-9]+/ ;                                     \
    operator    : '+' | '-' | '*' | '/' | '%' ;                          \
    expression  : <number> | '(' <operator> <expression>+ ')' ;    \
    clisp       : /^/ <operator> <expression>+ /$/ ;               \
  ",
  Number, Operator, Expression, Clisp);


  puts("clisp v0.0.1");
   
  /* In a never ending loop */
  while (1) { 
    char* input = readline("> ");
    
    /* Add input to history */
    add_history(input);

    mpc_result_t result;
    if (mpc_parse("<stdin>", input, Clisp, &result)) {
      mpc_ast_print(result.output);
      mpc_ast_delete(result.output);
    } else {
      mpc_err_print(result.error);
      mpc_err_delete(result.error);
    }

    /* Free retrived input */
    free(input);
    
  }
  
  mpc_cleanup(4, Number, Operator, Expression, Clisp);
  return 0;
}