#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>

int main() {
	ft_printf("%05.4d\n", 0);
	printf("%-5.d\n", 0);
}
