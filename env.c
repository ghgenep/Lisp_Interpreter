
#include "headers.h"

#define MAX_EXPR 4096 * 4

void env_make_zero(void) {
	env_make();
	Return = obj_make_keyword("", MAX_EXPR, KEYWORD, Env);
	//tree_push_obj(Return, Env->root);
	
	Lambda = obj_make_keyword("lambda", MAX_EXPR, KEYWORD, Env);
	//tree_push_obj(Lambda, Env->root);
	
	//Quote = obj_make_keyword("quote", MAX_EXPR, KEYWORD, Env);
	//tree_push_obj(Quote, Env->root);

	True = obj_make_new("true", "true", KEYWORD, Env);
	tree_push_obj(True, &Env->root);

	False = obj_make_new("false", "false", KEYWORD, Env);
	tree_push_obj(False, &Env->root);

	Return_lambda_env = NULL;

	tree_push_obj(obj_make_new("+", "", KEYWORD, Env), &Env->root);
	tree_push_obj(obj_make_new("define", "", KEYWORD, Env), &Env->root);
	tree_push_obj(obj_make_new("lambda", "", KEYWORD, Env), &Env->root);	
	tree_push_obj(obj_make_new("display", "", KEYWORD, Env), &Env->root);
	tree_push_obj(obj_make_new("equal", "", KEYWORD, Env), &Env->root);
	tree_push_obj(obj_make_new("if", "", KEYWORD, Env), &Env->root);
	tree_push_obj(obj_make_new("echo", "(lambda (x) x)", FUNCTION, Env), &Env->root);
}

void env_update_result(struct Obj *obj) {
	Env->result = obj;
}

struct Obj *env_take_result() {
	return Env->result;
}

void env_make(void) {
	struct Environment *env = (struct Environment *) mem_alloc_memory(sizeof(struct Environment));
	env->root = NULL;
	env->result = NULL;
	if (Env == NULL)
		env->deep = 0;
	else
		env->deep = Env->deep + 1;
	env->up = Env;
	Env = env;
	env->index = 0;
	Env->root = tree_push_obj(obj_make_new("", "", 0, Env), &Env->root);
}

void env_del(struct Environment *env) {
	mem_free_memory(env);
	//mem_update();
	// delete root
	//ree(env->buffer);
	//free(env);
}

