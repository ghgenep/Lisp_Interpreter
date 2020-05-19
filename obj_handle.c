
#include "headers.h"
#include "mem_alloc.h"

struct Obj *obj_make_new(char *name, char *code, int type, struct Environment *env) {
	struct Obj *newobj = (struct Obj *) mem_alloc_memory(sizeof(struct Obj));
	newobj->name       = (char *) mem_alloc_memory(strlen(name) + 1);
	newobj->code       = mem_alloc_memory(strlen(code) + 1);
	strcpy(newobj->name, name);
	strcpy(newobj->code, code); 
	newobj->type = type;
	newobj->env = env;
	newobj->lenv = NULL;
	newobj->right = newobj->left = NULL;
	return newobj;
}

struct Obj *obj_make_keyword(char *name, unsigned long int l, int type, struct Environment *env) {
	struct Obj *obj = obj_make_new(name, "", type, env);
	mem_free_memory(obj->code);
	obj->code = mem_alloc_memory(l);
	return obj;
}

struct Obj *obj_change_code(struct Obj *obj, char *code) {
	strcpy(obj->code, code);
	return obj;
}

struct Obj *obj_get_copy(struct Obj *obj, char *name) {
	return obj_make_new(name, obj->code, obj->type, obj->env);
}
