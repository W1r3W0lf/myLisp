#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "parse.h"


const char *argp_program_version = "myLisp 0.0.1";
const char *argp_program_bug_address = "apple.link.level@gmail.com";
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


	struct arguments arguments;

	arguments.mode = TEST;
	arguments.isCaseInsensitive = false;

	argp_parse(&argp, argc, argv, 0,0, &arguments);


	printf("Welcom to myLisp!\n\n");


	while(1){
		char* input = readline("myLisp> ");

		add_history(input);

		yyparse();

		printf("that's all\n");

		free(input);
	}

	return 0;
}
