#include <stdio.h>

typedef int (*fptr)(void);

extern fptr get_implementation(int);

int (*foo)(void);

int main(int argc, char **argv) {
	foo = get_implementation(argc);
	return printf("Indirect call of function foo returned: %d\n", foo());
}
