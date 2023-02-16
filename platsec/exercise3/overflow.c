#include <stdio.h>
#include <string.h>

void can_you_do_it(char *arg)
{
	char buf[8];
	strcpy(buf, arg);
}

int main()
{
	char* arg = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	can_you_do_it(arg);

	puts("Just stop it already!");

	return 0;
}
