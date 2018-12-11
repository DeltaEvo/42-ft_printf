#include "ft_printf.h"
#include <stdio.h>

#define TEST(...) ft_printf("Mine: " __VA_ARGS__); printf("Real: " __VA_ARGS__);

int main()
{
	TEST("%6.2f\n", 0.42);
	TEST("%6.1f\n", 10.0);
	TEST("%6.1f\n", -10.0);
	TEST("%+6.1f\n", +10.0);
	//TEST("%6.1f\n", 100000000.0);
	TEST("%40.1f\n", 100000000000000000000000.0);
	TEST("%+40.1f\n", 100000000000000000000000.0);
	TEST("% 40.1f\n", 100000000000000000000000.0);
	TEST("%40.1f\n", -100000000000000000000000.0);
	TEST("%040.1f\n", -100000000000000000000000.0);
}
