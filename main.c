#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>
#include <locale.h>
#include <float.h>

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");

	ft_printf("%10.1f\n", 3.0);
	printf("%10.1f\n", 3.0);
	ft_printf("%.1f\n", 3.2);
	printf("%.1f\n", 3.2);
	ft_printf("%.13f\n", 0.7324763284324);
	printf("%.13f\n", 0.7324763284324);
}
