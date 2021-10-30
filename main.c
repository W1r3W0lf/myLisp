#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char** argv){
	printf("myLisp\n");


	while(1){
		char* input = readline("myLisp> ");

		add_history(input);

		printf("that's all\n");

		free(input);
	}

	return 0;
}
