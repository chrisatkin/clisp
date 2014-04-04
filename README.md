# clisp

`clisp` (short of Chris's Lisp) is a small implementation (<1000 lines) of a subset of [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)) in C.

## Building

Builds only on OS X and Linux (untested). Doesn't build on Windows without modifications. You'll only need a C compiler with the [GNU readline library](http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html) - all other dependencies are included.

Make file nonwithstanding (coming soon), run:

`cc -std=c99 -Wall repl.c -ledit -o repl`

to build.

## License

See [LICENSE](LICENSE).

Copyright © 2014 Chris Atkin.