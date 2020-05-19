
#include "headers.h"

void env_make_recursive(struct Environment *env) {
	if (env == NULL)
		return;
	env_make_recursive(env->up);
	env->up = Env;
	Env = env;
}
//(define a (lambda (x) 
//  (lambda (z t) 
//    (if (equal z 1) 
//        (define x t) 
//        (display x)))))
void do_function(struct Obj *obj, char *rest_args) {
	char *lam_code, *param_code;
	char buf[MAX_EXPR], param[MAX_EXPR], arg[MAX_EXPR];
	buf[0] = param[0] = arg[0] = '\0';
	struct Obj *newobj;
	struct Environment *env;
	int second_env = 0;

	if (obj->lenv != NULL) {
		env_make_recursive(obj->lenv);
		second_env = 1;
	}
	env_make();

	lam_code = obj->code;  /* get a function lambda code */
	lam_code++;
	get_next_token(&lam_code, buf);  /* skip 'lambda' word */
	get_next_token(&lam_code, buf);  /* take a list of arguments */

	param_code = buf + 1;
	while (get_next_token(&param_code, param) != NULL) {
		if (get_next_token(&rest_args, arg) == NULL)
			strcpy(arg, "()");
		eval(arg);

		newobj = obj_get_copy(Env->result, param);
		tree_push_obj(newobj, &Env->root);
	}
	while (get_next_token(&lam_code, buf) != NULL)
		eval(buf);
	
	if (second_env == 0) {
		Env->up->result = Env->result;
		env = Env->up;

		if (Env->result == Lambda) {
			if (Return_lambda_env != NULL)
				env_del(Return_lambda_env);
			Return_lambda_env = Env;
			Return_lambda_env->up = NULL;
			Return_lambda_env->saved = 1;
		} else 
			env_del(Env);
		Env = env;
	} else {
		env = Env;
		while (env->up->saved == 1)
			env = env->up;
		env->up->result = Env->result;
		struct Environment *outenv = env->up;
		env->up = NULL;

		if (Env->result == Lambda) {
			if (Return_lambda_env != NULL)
				env_del(Return_lambda_env);
			Return_lambda_env = Env;
			Return_lambda_env->saved = 1;
		}

		Env = outenv;
	}
}

void do_macro(struct Obj *obj, char *rest_args) {

}

void do_keyword(struct Obj *obj, char *rest_args) {
	char buf[MAX_EXPR];
	char buf2[MAX_EXPR];
	char value[MAX_EXPR];
	char value2[MAX_EXPR];
	struct Obj *newobj;

	if (strcmp(obj->name, "define") == 0) {
		get_next_token(&rest_args, buf);   /* take definition word (define [var] value) */
		if (is_list(buf))
			error("buf[] is list!", NULL, 0);
		get_next_token(&rest_args, value); /* take value in (define var [value]) */
		eval(value);

		newobj = obj_get_copy(Env->result, buf);  /* make new object with [var] name */
		if (is_lambda(Env->result->code))
			newobj->type = FUNCTION;
		else
			newobj->type = VARIABLE;

		newobj->lenv = Return_lambda_env;  /* if the [value] is a function that return lambda */
		Return_lambda_env = NULL;          /* if Return_lambda_env == NULL, return value was not a lambda */

		env_update_result(newobj);
		tree_push_obj(newobj, &Env->root);
		return;
	} else if (strcmp(obj->name, "lambda") == 0) {
		strcat(buf, "(lambda ");
		strcat(buf, rest_args);
		strcat(buf, ")");
		strcpy(Lambda->code, buf);
		env_update_result(Lambda);
	} else if (strcmp(obj->name, "quote") == 0) {
		get_next_token(&rest_args, buf);
		strcpy(Return->code, buf);
		env_update_result(Return);
	} else if (strcmp(obj->name, "display") == 0) {
		get_next_token(&rest_args, buf);
		eval(buf);
		printf("%s", Env->result->code);
	} else if (strcmp(obj->name, "equal") == 0) {
		struct Obj *first;
		struct Obj *second;
		get_next_token(&rest_args, buf);
		get_next_token(&rest_args, buf2);
		eval(buf);
		first = env_take_result();
		eval(buf2);
		second = env_take_result();
		if (is_equal(first->code, second->code) == 1)
			env_update_result(True);
		else
			env_update_result(False);
	} else if (strcmp(obj->name, "if") == 0) {
		get_next_token(&rest_args, buf);
		eval(buf);
		get_next_token(&rest_args, buf);
		get_next_token(&rest_args, buf2);
		if (env_take_result() == True)
			eval(buf);
		else
			eval(buf2);
	} else if (strcmp(obj->name, "+") == 0) {
		double a;
		get_next_token(&rest_args, buf);
		eval(buf);
		if (env_take_result()->type != NUMBER)
			error("+: it's not a variable", NULL, 0);
		get_next_token(&rest_args, buf2);
		eval(buf2);
		if (env_take_result()->type != NUMBER)
			error("+: it's not a variable", NULL, 0);
		a = atof(buf) + atof(buf2);
		strcpy(Return->name, "");
		a = sprintf(Return->code, "%f", a);
	}
}


