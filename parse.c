
#include "headers.h"
#define R_TRUE      1
#define R_FALSE     0

/* is_sep: return true if c is one of the separate characters */
int is_sep(const int c) {
	if (c == '(' || c == ')' || isspace(c))
		return R_TRUE;
	return R_FALSE;
}

/* skip_spaces: skip spaces and comments and return pointer to
                next character; comment starts with ';' */
char *skip_spaces(char *p) {
	while (*p != '\0') {
		if (isspace(*p))
			p++;
		else if (*p == ';') {
			p++;
			while (*p != '\0') {
				if (*p++ == '\n')
					break;
			}
			if (*p == '\0')
				return p;
			continue;
		} else
			break;
	}
	return p;
}

/* is_null: return true, if buf is just '()' empty list */
int is_null(char *buf) {
	char *p = buf;
	if (*buf == '(') {
		p = skip_spaces(buf + 1);
		if (*p == ')')
			return R_TRUE;
	}
	return R_FALSE;
}

/* get_first_token: if an expression is (...) return result status of
        get_expr() function inside; else return 0; copy first token 
        into to[] array  */
char *get_next_token(char **from, char *to) {
	to[0] = '\0';
	char *p = *from;
	p = skip_spaces(p);
	if (*p == ')')
		return NULL;
	return get_expr(from, to);
}

/* get_rest_tokens: if an expression is (...) return result status of
        get_expr() function; else return 0; copy all tokens except first
        one into to[] array */
char *get_rest_tokens(char *from, char *to) {
	to[0] = '\0';
	from = skip_spaces(from);
	if (*from == ')')
		return NULL;
	strcpy(to, from);
	to[strlen(to) - 1] = '\0';
	return to;
}

char *get_tokens_except_first(char *from, char *to) {
	to[0] = '\0';
	from++;
	if (get_next_token(&from, to) == NULL)
		return NULL;
	return get_rest_tokens(from, to);
}

char *get_token(char **code, char *to) {
	char *p = *code;
	int i = 0;

	if (*p == ')')
		error("')' character is coming, firsm must be '('\n", NULL, 0);

	do {
		*to++ = *p++;
		if (is_sep(*p) || *p == '\0'){
			*to = '\0';
			*code = p;
			return p;
		}
		i++;
	} while (i < MAX_EXPR - 1);
	error("too big input token\n", NULL, 0);
}

char *get_list(char **code, char *to) {
	char *p = *code;
	int par = 0, i;

	for (i = 0; i < MAX_EXPR - 1; i++) {
		if (*p == '\0')    /* if there's no ')' character */
			error("0x00 is coming, there's no ')' character\n", NULL, 0);
		*to = *p;
		if (*p == '(')
			par++;
		else if (*p == ';') {
			p = skip_spaces(p);
			if (*p == '\0')    /* if there's no ')' character */
				error("0x00 is coming in skip_spaces(), there's no ')' character\n", NULL, 0);
			continue;
		} else if (*p == ')') {
			par--;
			if (par == 0) {
				*(to + 1) = '\0';
				p++;
				*code = p;
				return p;
			}
		}
		to++, p++;
	}
	/* if we have written max number of characters */
	error("too big input list\n", NULL, 0);
}
zz
char *get_expr(char **code, char *to) {
	char *p = *code;
	to[0] = '\0';

	p = skip_spaces(p);
	if (*p == '\0')
		return NULL;
	*code = p;
	if (*p == '(')
		return get_list(code, to);
	else
		return get_token(code, to);
}

/* test for parse.c 
int main(int argc, char **argv) {
	char buf[MAX_EXPR];
	char buf2[MAX_EXPR];
	char in[MAX_EXPR * 100];
	char *q = in;
	char *p = buf + 1;
	int c, i;

	for (i = 0; (c = getchar()) != EOF; i++)
		in[i] = c;
	in[i] = '\0';

	while (get_expr(&q, buf) != NULL) {
		printf("%s ---> ", buf);
		while (get_next_token(&p, buf2) != NULL)
			printf("%s ", buf2);
		p = buf + 1;
		printf(" ---> ");
		if (get_tokens_except_first(p, buf2) != NULL)
			printf("%s", buf2);
		printf("\n");
		p = buf + 1;

	}
	printf("NULL\n");	
	return 0;
}
/**/
