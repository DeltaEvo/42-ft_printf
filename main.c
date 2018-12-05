#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>
#include <locale.h>

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");
	ft_printf("%15.4S\n", L"我是一只猫。");
	printf("%15.4S\n", L"我是一只猫。");
	ft_printf("%15.8S\n", L"hello");
	printf("%15.8S\n", L"hello");
}
