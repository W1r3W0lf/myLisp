#include "ast.h"
#include "interp.h"
#include "parse.h"
#include "symtable.h"
#include <argp.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

sym_node* global_symboltable;
ast_node* ast;

// Flex scan string
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

// Argument parsing
const char *argp_program_version = "myLisp 0.1.0";
const char *argp_program_bug_address = "https://git.sr.ht/~wire_wolf/mylisp";
static char doc[] = "An interpritor written to learn about interpritors and compilers.";
static char args_doc[] = "[FILE]...";
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

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};


int main(int argc, char** argv){

#ifdef YYDEBUG
//	yydebug = 1;
#endif

	struct arguments arguments;

	arguments.mode = TEST;
	arguments.isCaseInsensitive = false;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

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
		repl_print(&global_symboltable, result);

		ast_free(result);
		ast_free(ast);

		yy_delete_buffer(buffer);

		free(input);
	}

	return 0;
}
