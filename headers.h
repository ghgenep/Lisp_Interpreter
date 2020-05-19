
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "mem_alloc.h"
#include "obj_handle.h"

#define MAX_EXPR    4096 * 4

#define KEYWORD  1
#define MACRO    2
#define FUNCTION 3
#define VARIABLE 4
#define NUMBER   5
#define R_TRUE   1
#define R_FALSE  0

int is_equal(char *first, char *second);

/* parse.c */
char *get_expr(char **code, char *expr);
char *get_next_token(char **from, char *to);
char *get_rest_tokens(char *from, char *to);
char *get_tokens_except_first(char *from, char *to);
void print_obj(struct Obj *obj);
char *del_pars(char *s);
int is_lambda(char *s);

/* env.c */
void env_make(void);
void env_make_zero(void);
struct Obj *env_take_result(void);
void env_update_result(struct Obj *obj);

/* errors.c */
void *ec_malloc(size_t size);

/* interp.c */
struct Obj *eval(char *expr);

/* tree_fun.c */
struct Obj *env_pop_obj(char *name);
struct Obj *make_obj(char *name, char *code, int type, struct Environment *env);
struct Obj *make_new_obj(char *name, char *code, int type, struct Environment *env);
struct Obj *copy_obj(struct Obj *obj, char *name);
struct Obj *change_obj(struct Obj *obj, struct Obj *newobj);
struct Obj *tree_pop_obj(char *name, struct Obj *root);
struct Obj *tree_push_obj(struct Obj *obj, struct Obj **root);
struct Obj *tree_env_pop_obj(char *name);
void print_obj(struct Obj *obj);
void print_env(struct Environment *env);

/* do_fun.c */
void do_function(struct Obj *obj, char *rest_args);
void do_keyword(struct Obj *obj, char *rest_args);
void do_macro(struct Obj *obj, char *rest_args);