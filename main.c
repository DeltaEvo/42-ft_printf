#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>
#include <locale.h>
#include <float.h>

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");

	printf("%*d t\n", -5, 4);
	ft_printf("%*d t\n", -5, 4);
	ft_printf("{%f}{%F}\n", 1444565444646.6465424242242, 1444565444646.6465424242242);
	printf("{%f}{%F}\n", 1444565444646.6465424242242, 1444565444646.6465424242242);
}
