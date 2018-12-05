#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>
#include <locale.h>

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");

	ft_printf("%#.5x", 120);
}
