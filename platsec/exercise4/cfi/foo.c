#include <stdio.h>

typedef int (*fptr)(void);

int foo_impl(void) {
	int res = 0;
	res = printf("\nfoo_impl has been called !\n\n");
	return res + 42; 
}

int foo_impl_2(void) {
	int res = 0;
	res = printf("\nfoo_impl_2 has been called !\n\n");
	return res + 42 + 2; 
}

int foo_impl_3(void) {
	int res = 0;
	res = printf("\nfoo_impl_3 has been called !\n\n");
	return res + 3000; 
}

int foo_impl_4(void) {
	int res = 0;
	res = printf("\nfoo_impl_4 has been called !\n\n");
	return res + 4000; 
}

int bar_impl(int value) {
	int res = 0;
	res = printf("\nbar_impl has been called !\n\n");
	return res + value; 
}

fptr get_implementation(int choice) {
	if (choice == 1)
		return &foo_impl;
	else if (choice == 2)
		return &foo_impl_2;
	else if (choice == 3)
		return &foo_impl_3;
	else if (choice == 4)
		return &foo_impl_4;
	return NULL;
}
