#include <check.h>
#include "../symtable.h"
#include "../ast.h"

START_TEST(test_symtable_create) {
	sym_node** table = NULL;
	sym_node* node = NULL;
	char name[] = "x";
	ast_node* object = ast_new_node(number);
	object->value.number = 12;

	*table = sym_define(table, name, object);

	ck_assert_ptr_nonnull(*table);

} END_TEST



Suite *symtable_suite(void){
	Suite *s;
	TCase *tc_core;

	s = suite_create("Symtable");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_symtable_create);
	suite_add_tcase(s, tc_core);

	return s;
}
