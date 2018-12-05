#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>
#include <locale.h>

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");

	ft_printf("%.3d\n", -1);
	printf("%.3d\n", -1);
}
