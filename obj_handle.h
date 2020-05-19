
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Obj *obj_make_new(char *name, char *code, int type, struct Environment *env);
struct Obj *obj_make_keyword(char *name, unsigned long int l, int type, struct Environment *env);
struct Obj *obj_change_code(struct Obj *obj, char *code);
struct Obj *obj_get_copy(struct Obj *obj, char *name);
