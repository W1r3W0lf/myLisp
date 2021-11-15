#include "../interp.h"
#include <check.h>

START_TEST(test_eval_number){

} END_TEST


Suite *interp_suite(void){
	Suite *s;
	TCase *tc_core;

	s = suite_create("Interp");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_eval_number);
	suite_add_tcase(s, tc_core);

	return s;
}
