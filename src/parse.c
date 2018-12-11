/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:21:24 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 12:06:03 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "utils.h"

static uint8_t	parse_flags(char **it)
{
	uint8_t	flag;

	if (FIDX(**it) < 0 || (size_t)FIDX(**it) >
		(sizeof(g_flags) / sizeof(*g_flags)))
		return (0);
	if ((flag = g_flags[FIDX(**it)]))
	{
		++*it;
		return (flag | parse_flags(it));
	}
	return (0);
}

static int32_t	parse_width(char **it, t_fmt *fmt, uint32_t *idx)
{
	fmt->width = 0;
	if (**it == '*')
		return ((*++*it >= '0' && **it <= '9') ? atoio(it) - 1 : (*idx)++);
	else if ((**it >= '0' && **it <= '9') || **it == '-')
		fmt->width = atoio(it);
	return (-1);
}

static int32_t	parse_precision(char **it, t_fmt *fmt, uint32_t *idx)
{
	if (**it == '.')
	{
		++*it;
		if (**it == '*')
		{
			fmt->precision = -1;
			return ((*++*it >= '0' && **it <= '9') ? atoio(it) - 1 : (*idx)++);
		}
		else if (**it >= '0' && **it <= '9')
			fmt->precision = atoio(it);
	}
	else
		fmt->precision = -1;
	return (-1);
}

static uint8_t	parse_length(char **it)
{
	uint8_t flag;

	if (LIDX(**it) < 0 || (size_t)LIDX(**it) >
		(sizeof(g_elens) / sizeof(*g_elens)))
		return (0);
	if ((flag = g_elens[LIDX(**it)]))
	{
		++*it;
		if ((flag & PF_H && **it == 'h') || (flag & PF_L && **it == 'l'))
		{
			flag <<= 1;
			++*it;
		}
		return (flag ? flag : parse_length(it));
	}
	return (0);
}

t_parse			parse(char **it, t_fmt *f, uint32_t *idx)
{
	t_parse	res;

	f->begin = ++*it;
	if (**it >= '9' && **it <= '9')
	{
		res.param = atoio(it) - 1;
		if (*(*it)++ != '$')
		{
			*it = f->begin;
			res.param = -1;
		}
	}
	else
		res.param = -1;
	f->flags = parse_flags(it);
	res.width = parse_width(it, f, idx);
	res.precision = parse_precision(it, f, idx);
	f->length = parse_length(it);
	f->end = **it ? ++(*it) : *it;
	if (res.param == -1 && f->end[-1] != '%')
		res.param = (*idx)++;
	res.max = MAX(MAX(MAX(res.param, res.width), res.precision), 0);
	return (res);
}
