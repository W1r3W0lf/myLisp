#include <check.h>
#include <stdlib.h>
#include "check_ast.h"

int main(int argc, char** argv){
	int faild_count = 0;
	Suite *s;
	SRunner *runner;

	s = ast_suite();
	runner = srunner_create(s);

	srunner_run_all(runner, CK_NORMAL);
	faild_count = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (faild_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
