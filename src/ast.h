#ifndef __ast_h__
#define __ast_h__

/* Forward Declarations */

struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

typedef enum {
    LVAL_ERR,
    LVAL_NUM,
    LVAL_SYM,
    LVAL_FUN,
    LVAL_SEXPR,
    LVAL_QEXPR
} val_t;
// typedef enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM } err_t;

// Typedef can be put before any standard variable declaration.
// It results in the name of the variable, being declared a new type,
// matching what would be the inferred type of that variable.
// This is why in the above declaration what looks like the function name
// becomes the new type name.
typedef lval* (*lbuiltin)(lenv*, lval*);

typedef struct lval {
    val_t type;
    long num;
    char* err;
    char* sym;
    lbuiltin fun;
    /* Count and Pointer to a list of "lval*" */
    int count;
    struct lval** cell;
} lval;

typedef struct lenv {
    int count;
    char** syms;
    lval** vals;
} lenv;

// ---

lval* lval_num(long x);

lval* lval_err(char* fmt, ...);

lval* lval_sym(char* s);

lval* lval_fun(lbuiltin func);

lval* lval_sexpr(void);

lval* lval_qexpr(void);

// ---

void lval_del(lval* v);

// ---

lval* lval_copy(lval* v);

lval* lval_add(lval* v, lval* x);

lval* lval_join(lval* x, lval* y);

lval* lval_pop(lval* v, int i);

lval* lval_take(lval* v, int i);

// ---

lenv* lenv_new(void);

void lenv_del(lenv* e);

lval* lenv_get(lenv* e, lval* k);

void lenv_put(lenv* e, lval* k, lval* v);

// ---

void lval_print_expr(lval* v, char open, char close);

void lval_print(lval* v);

void lval_println(lval* v);

char* ltype_name(int t);

#endif
