#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../symtable.h"
#include "../ast.h"

START_TEST(test_symtable_create) {

	sym_node* table = NULL;

	// Crate a symbol for the table
	char name[] = "x";
	// Create an object for the table
	ast_node* object = ast_new_node(number);
	object->value.number = 12;

	table = sym_define(&table, name, object);

	// Ensure the table has been created
	ck_assert_ptr_nonnull(table);

	// Check if the new node was created propperly
	ck_assert_ptr_eq(object, table[0].value);
	ck_assert_str_eq(name, table[0].symbol);
	ck_assert_ptr_null(table[0].next);

} END_TEST

START_TEST(test_symtable_append) {
	
	// Create table.
	sym_node* table = NULL;
	// Make a node for that table.
	sym_node* test_node = malloc(sizeof(sym_node));
	// Make an object to be stored in the inital node.
	ast_node* object1 = ast_new_node(number);
	object1->value.number = 12;

	// Full the new node with it's vaules.
	test_node->next = NULL;
	test_node->symbol = strdup("a");
	test_node->value = object1;
	
	// Add the new node to the front of the table.
	table = test_node;

	// Create new vales to be added to the table.
	char name2[] = "b";
	ast_node* object2 = ast_new_node(number);
	object2->value.number = 13;

	// Add the new node to the front of the table.
	table = sym_define(&table, name2, object2);

	// Check if the new front node has been set correctly
	ck_assert_str_eq(name2, table->symbol);
	ck_assert_ptr_eq(object2, table->value);

	// Check if the next node has been stored correctly
	ck_assert_ptr_eq(test_node, table->next);
	ck_assert_ptr_eq(object1, table->next->value);
	ck_assert_str_eq("a", table->next->symbol);

} END_TEST


Suite *symtable_suite(void){
	Suite *s;
	TCase *tc_core;

	s = suite_create("Symtable");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_symtable_create);
	tcase_add_test(tc_core, test_symtable_append);
	suite_add_tcase(s, tc_core);

	return s;
}
