
struct Obj {
	char *name;
	char *code;
	int type; 
	struct Environment *lenv;
	struct Environment *env;
	struct Obj *right;
	struct Obj *left;
};

struct Environment {
	struct Environment *up;
	unsigned long int  index;
	unsigned long int  buf_size;
	unsigned long int  deep;
	struct Obj         *root;
	struct Obj         *result;
	int                saved;
};

struct Obj *Upper_tree;
struct Environment *Env;
struct Environment *Last_env;
struct Obj *Lambda;
struct Obj *Quote;
struct Obj *True;
struct Obj *False;
struct Environment *Return_lambda_env;
struct Obj *Return;