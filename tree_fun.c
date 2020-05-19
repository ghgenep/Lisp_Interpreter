
#include "headers.h"


struct Obj *tree_pop_obj(char *name, struct Obj *root) {
	int cmp;

	if (root == NULL)
		return NULL;
	else if ((cmp = strcmp(name, root->name)) > 0)
		return tree_pop_obj(name, root->right);
	else if (cmp < 0)
		return tree_pop_obj(name, root->left);
	else
		return root;
}

struct Obj *tree_push_obj(struct Obj *obj, struct Obj **root) {
	int cmp;
	char *name = obj->name;
	struct Obj *sroot = *root;
	
	if (sroot == NULL) {
		*root = obj;
		/*root = (struct Obj *) ec_malloc(sizeof(struct Obj));
		root->left = root->right = NULL;
		root->name = name;
		root->code = obj->code;
		root->env = obj->env;
		root->type = obj->type;
		root->lenv = obj->lenv; */
	} else if ((cmp = strcmp(name, sroot->name)) > 0)
		(*root)->right = tree_push_obj(obj, &(sroot->right));
	else if (cmp < 0)
		(*root)->left = tree_push_obj(obj, &(sroot->left));
	else {
		//free_objects(root->obj);
		(*root)->code = obj->code;
		(*root)->type = obj->type;
		(*root)->env  = obj->env;
		(*root)->lenv = obj->lenv;
	}
	return *root;
}

void print_tree(struct Obj *p) {
	if (p != NULL)
		print_tree(p->left);
	else
		return;
	print_obj(p);
	print_tree(p->right);
}

struct Obj *tree_env_pop_obj(char *name) {
	struct Obj *obj;

	Last_env = Env;
	do {
		if ((obj = tree_pop_obj(name, Env->root)) == NULL)
			Env = Env->up;
		else {
			Env = Last_env;
			return obj;
		}
	} while (Env != NULL);
	Env = Last_env;
	return NULL;
}

void print_obj(struct Obj *obj) {
	printf("%p -> struct Obj {\n  name = %s\n  code = %s\n  type = %d\n  env = %p\n  left = %p\n  right = %p\n}\n",
		obj, obj->name, obj->code, obj->type, obj->env, obj->left, obj->right);
}
void print_env(struct Environment *env) {
	printf("%p -> struct Environment {\n  root = %p\n  up = %p\n}\n",
		env, env->root, env->up);
}
/*
int main(void) {
	//struct Environment *i = ec_malloc(sizeof(struct Environment));
	//print_obj(make_obj("first", "(1)", 1, i));
	env_make();
	Env->root = tree_push_obj(make_obj("first", "(1)", 1, Env), Env->root);
	tree_push_obj(make_obj("second", "(2)", 2, Env), Env->root);
	print_env(Env);
	env_make();
	Env->root = tree_push_obj(make_obj("third", "(3)", 3, Env), Env->root);
	print_env(Env);
	env_make();
	Env->root = tree_push_obj(make_obj("fourth", "(4)", 4, Env), Env->root);
	tree_push_obj(make_obj("second", "(2222)", 2, Env), Env->root);
	print_env(Env);
	print_obj(env_pop_obj("first"));
	print_obj(env_pop_obj("second"));
	print_obj(env_pop_obj("third"));
	print_obj(env_pop_obj("fourth"));
	return 0;
}*/
