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

START_TEST(test_ast_adding_children) {
	ast_node* node = NULL;
	ast_node* child = NULL;

	// Create node
	node = ast_new_node(quote);


	char given_string[] = "Hello";
	// Create Child
	child = ast_new_node(string);
	child->value.string = strdup(given_string);


	// Add child to parent
	ast_add_child(node, child);


	// Check the status of node
	ck_assert_ptr_nonnull(node);
	ck_assert_int_eq(node->ref_count, 0);
	ck_assert_int_eq(node->type, quote);

	// Check the status of child
	ck_assert_ptr_nonnull(child);
	ck_assert_int_eq(child->ref_count, 1);
	ck_assert_int_eq(child->type, string);

	// Check node and child's connection
	ck_assert_ptr_eq(node->children[0], child);


	ast_free(node);

} END_TEST

START_TEST(test_ast_freeing) {
	ast_node* node = NULL;

	// Ensure that freeing a NULL retrns an error
	int free_result = ast_free(node);
	ck_assert_int_eq(free_result, -1);

	node = ast_new_node(number);
	node->value.number = 1;
	node->ref_count++;
	free_result = ast_free(node);

	// Ensure that free won't work with an active refrince.
	ck_assert_int_eq(free_result, 1);
	ck_assert_int_eq(node->value.number, 1);

	node->ref_count = 0;

	// Ensure that free will work without active refrince.
	free_result = ast_free(node);
	ck_assert_int_eq(free_result, 0);


} END_TEST

START_TEST(test_ast_create_definition) {
	ast_node* node = NULL;
	ast_node* child1 = NULL;
	ast_node* child2 = NULL;

	node = ast_new_node(definition);

	// Create the symbol to be used
	char symbol_name[] = "test";
	child1 = ast_new_node(symbol);
	child1->value.symbol = strdup(symbol_name);

	// Create the number to be used
	child2 = ast_new_node(number);
	child2->value.number = 1;


	// Add the children to the ast
	ast_add_child(node, child1);
	ast_add_child(node, child2);


	// Check assuptions about node
	ck_assert_ptr_nonnull(node);
	ck_assert_int_eq(node->type, definition);
	ck_assert_int_eq(node->child_count, 2);
	ck_assert_int_eq(node->ref_count, 0);


	// Check Symbol aka child 1
	ck_assert_ptr_eq(node->children[0], child1);
	ck_assert_int_eq(node->children[0]->ref_count, 1);


	// Check Symbol aka child 2
	ck_assert_ptr_eq(node->children[1], child2);
	ck_assert_int_eq(node->children[1]->ref_count, 1);


	// Free the ast
	int free_result = ast_free(node);

	ck_assert_int_eq(free_result, 0);

} END_TEST

Suite *ast_suite(void){
	Suite *s;
	TCase *tc_core;

	s = suite_create("AST");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_ast_create_int);
	tcase_add_test(tc_core, test_ast_create_string);
	tcase_add_test(tc_core, test_ast_adding_children);
	tcase_add_test(tc_core, test_ast_freeing);
	tcase_add_test(tc_core, test_ast_create_definition);
	suite_add_tcase(s, tc_core);

	return s;
}

