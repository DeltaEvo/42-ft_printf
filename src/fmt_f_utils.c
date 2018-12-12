/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fmt_f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 14:00:12 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/12 14:58:35 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fmt_f.h"
#include "fmt.h"
#include "parse.h"
#include "ft/convert.h"
#include <stdio.h>

void		bigint_pow(uint32_t *bigint, uint8_t n, uint16_t exp, uint32_t *len)
{
	uint32_t	carry;
	uint64_t	t;
	uint32_t	i;

	while (exp--)
	{
		carry = 0;
		i = 0;
		while (i < *len)
		{
			t = (uint64_t)bigint[i] * n + carry;
			bigint[i] = t % POW10;
			carry = t / POW10;
			i++;
		}
		if (carry)
			bigint[(*len)++] = carry;
	}
}

void		bigint_round(uint32_t *bigint, uint32_t len, uint16_t exp,
		uint32_t precision)
{
	const uint32_t	pos = exp - precision - 1;
	uint64_t		pow;
	uint32_t		i;
	uint64_t		v;

	if (pos / 9 >= len)
		return ;
	bigint += pos / 9;
	len -= pos / 9;
	i = 0;
	pow = 6;
	while (++i < pos % 9)
		pow *= 10;
	while (pow)
	{
		v = (uint64_t)*bigint + pow;
		*bigint++ = v % POW10;
		pow = v / POW10;
		i++;
	}
}

size_t		nb_len(uint64_t n)
{
	size_t	len;

	len = 1;
	while (n /= 10)
		len++;
	return (len);
}

uint32_t	ft_min(uint32_t a, uint32_t b)
{
	return (a > b ? b : a);
}
