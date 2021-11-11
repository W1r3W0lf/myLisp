#include <check.h>
#include <stdlib.h>
#include "check_ast.h"
#include "check_symtable.h"

int main(int argc, char** argv){
	int faild_count = 0;
	Suite *ast_s;
	Suite *symtable_s;
	SRunner *runner;
	SRunner *runner2;

	ast_s = ast_suite();
	runner = srunner_create(ast_s);

	symtable_s = symtable_suite();
	runner2 = srunner_create(symtable_s);

	srunner_run_all(runner, CK_NORMAL);
	faild_count = srunner_ntests_failed(runner);
	srunner_free(runner);

	srunner_run_all(runner2, CK_NORMAL);
	faild_count += srunner_ntests_failed(runner2);
	srunner_free(runner2);

	return (faild_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
