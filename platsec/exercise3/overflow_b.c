#include <stdio.h>
#include <string.h>


void evil() {
    puts("This should have had never happened");
}

void can_you_do_it()
{
	char buf[128];
	scanf("%s", buf);
}

int main()
{
	puts("I trust that you did not enter more than 127 chars ...");

	can_you_do_it();

	puts("Just stop it already!");

	return 0;
}
