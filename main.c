#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "parse.h"
#include "symtable.h"
#include "interp.h"
#include "ast.h"

sym_node* global_symboltable;
ast_node* ast;

// Flex scan string
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

// Argument parsing
const char *argp_program_version = "myLisp 0.0.2";
const char *argp_program_bug_address = "https://github.com/W1r3W0lf/myLisp/issues";
static char doc[] = "Wire_Wolf's first stab at writting a lisp.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
	{ "test", 't', 0, 0, "A test value to see how this works." },
	{ 0 },
};

struct arguments {
	enum { TEST } mode;
	bool isCaseInsensitive;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state){
	struct arguments *arguments = state->input;
	switch (key) {
		case 't': arguments->mode = TEST; break;
		case ARGP_KEY_ARG: return 0;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
};

static struct argp argp = { options, parse_opt, args_doc, doc, 0,0,0 };


int main(int argc, char** argv){

#ifdef YYDEBUG
//	yydebug = 1;
#endif

	struct arguments arguments;

	arguments.mode = TEST;
	arguments.isCaseInsensitive = false;

	argp_parse(&argp, argc, argv, 0,0, &arguments);

	global_symboltable = NULL;

	global_symboltable = default_symtable();

	printf("Welcom to myLisp!\n\n");

	ast_node* result = NULL;

	while(1){
		char* input = readline("myLisp> ");

		// Clear out the AST
		ast = NULL;

		YY_BUFFER_STATE buffer = yy_scan_string(input);

		add_history(input);

		yyparse();

		result = eval(&global_symboltable, ast);
		print(&global_symboltable, result);

		yy_delete_buffer(buffer);

		free(input);
	}

	return 0;
}
