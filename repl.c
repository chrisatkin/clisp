#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char** argv) {
  puts("clisp v0.0.1");
  puts("ctrl+c to exit\n");
   
  /* In a never ending loop */
  while (1) {
    char* input = readline("clisp> ");
    
    /* Add input to history */
    add_history(input);
    
    /* Echo input back to user */    
    printf("%s\n", input);

    /* Free retrived input */
    free(input);
    
  }
  
  return 0;
}