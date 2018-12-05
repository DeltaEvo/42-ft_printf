/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_out.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:01:23 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/05 14:03:47 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "write_out.h"
#include <unistd.h>

void	outflush(t_ctx *ctx)
{
	t_out_wdata	*data;

	data = (t_out_wdata *)ctx->write_data;
	write(data->fd, data->buf, data->idx);
	data->idx = 0;
}

void	outc(t_ctx *ctx, char c)
{
	t_out_wdata	*data;

	data = (t_out_wdata *)ctx->write_data;
	if (data->idx + 1 == 4096)
		outflush(ctx);
	data->buf[data->idx++] = c;
}

void	outn(t_ctx *ctx, char *s, size_t n)
{
	if (n > 0)
	{
		((t_out_wdata *)ctx->write_data)->nb += n;
		while (n--)
			outc(ctx, *s++);
	}
}

void	outr(t_ctx *ctx, char s, size_t n)
{
	if (n > 0)
	{
		((t_out_wdata *)ctx->write_data)->nb += n;
		while (n--)
			outc(ctx, s);
	}
}
