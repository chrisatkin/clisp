#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "mpc.h"

long eval_operation(long x, char* operation, long y) {
  if (strcmp(operation, "+") == 0) { return x + y; }
  if (strcmp(operation, "-") == 0) { return x - y; }
  if (strcmp(operation, "*") == 0) { return x * y; }
  if (strcmp(operation, "/") == 0) { return x / y; }
  if (strcmp(operation, "%") == 0) { return x % y; }
  return 0;
} 

long eval(mpc_ast_t* t) {
  // is it just a number? 
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }
  
  // operator always second child
  char* op = t->children[1]->contents;
  
  long x = eval(t->children[2]); // not use of recursive `eval()` allows sub/nested expressions

  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_operation(x, op, eval(t->children[i]));
    i++;
  }
  
  return x;  
}

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
   
  while (1) { 
    char* input = readline("> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Clisp, &r)) {
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);    
  }
  
  mpc_cleanup(4, Number, Operator, Expression, Clisp);
  return 0;
}