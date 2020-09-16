#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "eval.h"
#include "mpc.h"

// 使用箭头键不会前后移动输入的光标，而是会产生像 ^[[D 或 ^[[C 这种奇怪的字符。
// 很明显这不是我们想要的结果。我们需要用到 editline 库来解决这个问题。
#ifdef _WIN32

static char input[2048];

/* Fake readline function */
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer) + 1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

#else

#include <editline.h>

#endif

lval* lval_read_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err("Invalid Number.");
}

lval* lval_read(mpc_ast_t* t) {
    if (strstr(t->tag, "number")) { return lval_read_num(t); }
    if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

    lval* x = NULL;
    if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
    if (strstr(t->tag, "sexpr")) { x = lval_sexpr(); }
    if (strstr(t->tag, "qexpr")) { x = lval_qexpr(); }

    for (int i = 0; i < t->children_num; i++) {
        if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
        if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
        if (strcmp(t->children[i]->tag, "regex") == 0) { continue; }
        x = lval_add(x, lval_read(t->children[i]));
    }

    return x;
}

int main(int argc, char** argv) {
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Qexpr = mpc_new("qexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
    number : /-?[0-9]+/ ;                               \
    symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;         \
    sexpr  : '(' <expr>* ')' ;                          \
    qexpr  : '{' <expr>* '}' ;                          \
    expr   : <number> | <symbol> | <sexpr> | <qexpr> ;  \
    lispy  : /^/ <expr>* /$/ ;                          \
  ",
              Number, Symbol, Sexpr, Qexpr, Expr, Lispy);

    lenv* e = lenv_new();
    lenv_add_builtins(e);

    puts("Lispy Version 0.0.0.0.7");
    puts("Press Ctrl+c to Exit\n");

    while (1) {
        char* input = readline("lispy> ");
        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            lval* x = lval_eval(e, lval_read(r.output));
            lval_println(x);
            lval_del(x);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Lispy);

    return 0;
}
