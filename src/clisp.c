#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "mpc.h"

// Possible lval types
enum { LVAL_NUM, LVAL_ERR };

// Possible error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

typedef struct {
  int type;
  long num;
  int err;
} lval;

// create a number type lval
lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

// create an error type lval
lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

void lval_print(lval v) {
  switch(v.type) {
    case LVAL_NUM: 
      printf("%li", v.num);
      break;

    case LVAL_ERR:
      if (v.err == LERR_DIV_ZERO) {
        printf("error: division by zero");
      }

      if (v.err == LERR_BAD_NUM) {
        printf("error: invalid number");
      }

      if (v.err == LERR_BAD_OP) {
        printf("error: invalid operator");
      }

      break;
  }
}

void lval_println(lval v) {
  lval_print(v);
  putchar('\n');
}

lval eval_operation(lval x, char* operation, lval y) {
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }


  if (strcmp(operation, "+") == 0) { return lval_num(x.num + y.num); }
  if (strcmp(operation, "-") == 0) { return lval_num(x.num - y.num); }
  if (strcmp(operation, "*") == 0) { return lval_num(x.num * y.num); }
  if (strcmp(operation, "%") == 0) { return lval_num(x.num % y.num); }
  if (strcmp(operation, "^") == 0) { return lval_num(pow(x.num, y.num)); }
  if (strcmp(operation, "/") == 0) { return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num); }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {
  if (strstr(t->tag, "number")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  char* op = t->children[1]->contents;
  lval x = eval(t->children[2]);

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
    operator    : '+' | '-' | '*' | '/' | '%' | '^' ;                          \
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
      lval result = eval(r.output);
      lval_println(result);
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