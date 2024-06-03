/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:04:18 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/02 23:11:30 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	l_redir(t_data *data, t_ast *c)
{
	if (data->in != STDIN_FILENO)
		close(data->in);
	data->in = open(c->str, O_RDONLY);
	if (data->in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	ll_redir(t_data *data, t_ast *c)
{
	if (data->in != STDIN_FILENO)
		close(data->in);
	data->in = c->hdfd;
}

void	r_redir(t_data *data, t_ast *c)
{
	if (data->out != STDOUT_FILENO)
		close(data->out);
	data->out = open(c->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->out == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	rr_redir(t_data *data, t_ast *c)
{
	if (data->out != STDOUT_FILENO)
		close(data->out);
	data->out = open(c->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->out == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	update_redir(t_data *data, t_ast *c)
{
	while (c)
	{
		if (c->token == L_REDIR)
			l_redir(data, c->left);
		else if (c->token == LL_REDIR)
			ll_redir(data, c);
		else if (c->token == R_REDIR)
			r_redir(data, c->left);
		else if (c->token == RR_REDIR)
			rr_redir(data, c->left);
		c = c->right;
	}
	// dprintf(2, "redir end | in: %d | out: %d\n", data->in, data->out);
}
