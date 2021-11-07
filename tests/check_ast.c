#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../ast.h"
#include <check.h>


START_TEST(test_ast_create_int) {
	ast_node* node;

	node = ast_new_node(number);
	node->value.number = 12;
	ck_assert_int_eq(node->value.number, 12);
	ck_assert_int_eq(node->child_count, 0);
	ck_assert_int_eq(node->ref_count, 0);
	ck_assert_int_eq(node->type, number);
	ast_free(node);

} END_TEST

START_TEST(test_ast_create_string) {
	ast_node* node;

	char given_string[] = "Hello";

	node = ast_new_node(string);
	node->value.string = strdup(given_string);
	ck_assert_str_eq(node->value.string, "Hello");
	ck_assert_int_eq(node->child_count, 0);
	ck_assert_int_eq(node->ref_count, 0);
	ck_assert_int_eq(node->type, string);
	ast_free(node);

} END_TEST

START_TEST(test_ast_create_definition) {
	ast_node* node;
	ast_node* child1;
	ast_node* child2;

	char symbol_name[] = "test";

	node = ast_new_node(definition);

	child1 = ast_new_node(symbol);
	child1->value.symbol = strdup(symbol_name);

	child2 = ast_new_node(number);
	child2->value.number = 1;

	ast_add_child(node, child1);
	ast_add_child(node, child2);

	ck_assert_int_eq(node->child_count, 2);


	ck_assert_int_eq(node->ref_count, 0);
	ck_assert_int_eq(node->type, string);
	ast_free(node);

} END_TEST

Suite *ast_suite(void){
	Suite *s;
	TCase *tc_core;

	s = suite_create("AST");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_ast_create_int);
	tcase_add_test(tc_core, test_ast_create_string);
	tcase_add_test(tc_core, test_ast_create_definition);
	suite_add_tcase(s, tc_core);

	return s;
}


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