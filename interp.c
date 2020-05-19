
#include "headers.h"

int is_list(char *s) {
	if (*s == '(')
		return R_TRUE;
	return R_FALSE;
}

int is_lambda(char *s) {
	if (*s != '(')
		return R_FALSE;
	char buf[MAX_EXPR];
	s++;
	get_next_token(&s, buf);
	if (strcmp(buf, "lambda") == 0)
		return R_TRUE;
	else
		return R_FALSE;
}

int is_number(char *expr) {
	if (*expr == '\0')
		return 0;
	while (*expr != '\0') {
		if (isdigit(*expr))
			expr++;
		else
			return 0;
	}
	return 1;
}

struct Obj *eval(char *expr) {
	char buf[MAX_EXPR];
	char *p = expr + 1;
	struct Obj *obj, *newobj;

	if (!is_list(expr)) {
		if ((obj = tree_env_pop_obj(expr)) == NULL) {
			if (is_number(expr)) {
				env_update_result(obj_change_code(Return, expr));
				Return->type = NUMBER;
			}
			else
				error("the variable doesn't exist", NULL, 0);
		} else
			env_update_result(obj);
	} else {
		p = get_next_token(&p, buf);	
		obj = tree_env_pop_obj(buf);
		
		if (obj->type == KEYWORD)
			do_keyword(obj, get_tokens_except_first(expr, buf));
		else if (obj->type == MACRO)
			do_macro(obj, get_tokens_except_first(expr, buf));
		else if (obj->type == FUNCTION)
			do_function(obj, get_tokens_except_first(expr, buf));
		else {

		}
	}
}

int is_equal(char *first, char *second) {
	char buf[MAX_EXPR];
	char buf2[MAX_EXPR];
	char value[MAX_EXPR];
	char value2[MAX_EXPR];
	if (*first == '(' && *second == '(') {
		get_rest_tokens(first + 1, value);
		get_rest_tokens(second + 1, value2);
		first = value;
		second = value2;
		do {
			get_next_token(&first, buf);
			get_next_token(&second, buf2);
			if (buf[0] != buf2[0])
				return 0;
			if (buf[0] == '\0')
				return 1;
			if (is_equal(buf, buf2) != 1)
				return 0;
		} while (1);
	} else {
		if (strcmp(first, second) == 0)
			return 1;
		else
			return 0;
	}
}

int main(int argc, char **argv) {
	char buf[MAX_EXPR];
	char buf2[MAX_EXPR];
	char in[MAX_EXPR * 100];
	char *q = in;
	char *p = buf + 1;
	int c, i;

	get_tokens_except_first("echo a)", buf);
	for (i = 0; (c = getchar()) != EOF; i++)
		in[i] = c;
	in[i] = '\0';

	env_make_zero();
	env_make();
	while (get_expr(&q, buf) != NULL){
		eval(buf);
		//print_obj(Env->last_obj);
	}
	//printf("%d", is_equal("(a     	3 2)", "(a 3)"));
	//printf("\n\n");
	//print_tree(Env->root);
	return 0;
}