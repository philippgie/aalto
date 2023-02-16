#include<stdio.h>

struct THREE_NUMBERS {
    int a;
    int b;
    int c;
};

struct TWO_NUMBERS {
    int a;
    int b;
};

void increment_THREE_NUMBERS(void* ptr) {
    struct THREE_NUMBERS *data = (struct THREE_NUMBERS*) ptr;
    data->a++;
    data->b++;
    data->c++;
}

int main() {
    int secret_code = 0xdeadbeef;
    printf("value of secret code is %x\n", secret_code);
    struct TWO_NUMBERS data = {.a=0,.b=1};
    increment_THREE_NUMBERS(&data);
    printf("value of secret code is %x\n", secret_code);
}