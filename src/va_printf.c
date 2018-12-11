/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 13:59:50 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/11 11:56:44 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "eval.h"
#include "write_out.h"
#include "write_buffer.h"
#include <stdlib.h>

ssize_t	ft_vdprintf(int fd, char const *fmt, va_list ap)
{
	t_ctx		ctx;
	t_out_wdata	data;

	data = (t_out_wdata) {
		.fd = fd,
		.nb = 0,
		.idx = 0
	};
	ctx = (t_ctx) {
		.va = (t_va_slist) {
			.idx = 0,
			.lock = 0
		},
		.idx = 0,
		.write = outn,
		.writer = outr,
		.write_data = &data
	};
	va_copy(ctx.va.ap, ap);
	eval_fmt((char *)fmt, &ctx);
	outflush(&ctx);
	return (data.nb);
}

ssize_t	ft_vasnprintf(char **out, char const *fmt, va_list ap)
{
	t_ctx			ctx;
	t_buffer_wdata	data;

	data = (t_buffer_wdata) {
		.buffer = NULL,
		.nb = 0,
		.len = 0,
		.idx = 0
	};
	ctx = (t_ctx) {
		.va = (t_va_slist) {
			.idx = 0,
			.lock = 0
		},
		.idx = 0,
		.write = buffern,
		.writer = bufferr,
		.write_data = &data
	};
	va_copy(ctx.va.ap, ap);
	eval_fmt((char *)fmt, &ctx);
	bufferr(&ctx, 0, 1);
	bufferflush(&ctx);
	*out = data.buffer;
	return (data.nb);
}
