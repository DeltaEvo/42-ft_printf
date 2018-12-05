/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:08:40 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/05 14:09:52 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "write_buffer.h"
#include <stdlib.h>
#include <string.h>

void	bufferflush(t_ctx *ctx)
{
	t_buffer_wdata *data;

	data = ((t_buffer_wdata *)ctx->write_data);
	if (data->buffer)
		data->buffer =
			realloc(data->buffer, sizeof(char) * (data->len + data->idx));
	else
		data->buffer = malloc(sizeof(char) * data->idx);
	memcpy(data->buffer + data->len, data->buf, data->idx);
	data->len = data->len + data->idx;
	data->idx = 0;
}

void	bufferc(t_ctx *ctx, char c)
{
	t_buffer_wdata *data;

	data = ((t_buffer_wdata *)ctx->write_data);
	if (data->idx + 1 == 4096)
		bufferflush(ctx);
	data->buf[data->idx++] = c;
}

void	buffern(t_ctx *ctx, char *s, size_t n)
{
	if (n > 0)
	{
		((t_buffer_wdata *)ctx->write_data)->nb += n;
		while (n--)
			bufferc(ctx, *s++);
	}
}

void	bufferr(t_ctx *ctx, char s, size_t n)
{
	if (n > 0)
	{
		((t_buffer_wdata *)ctx->write_data)->nb += n;
		while (n--)
			bufferc(ctx, s);
	}
}
