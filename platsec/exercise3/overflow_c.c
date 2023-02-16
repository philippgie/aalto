#include <stdio.h>
#include <string.h>



void can_you_do_it()
{
	puts("Please input your message");
	char buf[128];
	scanf("%s", buf);
}

void evil() {
    puts("This should have had never happened");
}

int main()
{
	int dummy=-1;
	puts("I trust that you did not enter more than 127 chars ...");
	printf("Address of dummy: %p\n", &can_you_do_it);

	can_you_do_it();

	puts("Just stop it already!");

	return 0;
}
