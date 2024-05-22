/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:04:18 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/22 23:22:47 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	l_redir(t_data *data, t_ast *c, int *in)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = open(c->str, O_RDONLY);
	if (*in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	ll_redir(t_ast *c, int *in)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = c->hdfd;
}

void	r_redir(t_data *data, t_ast *c, int *out)
{
	if (*out != STDOUT_FILENO)
		close(*out);
	*out = open(c->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*out == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	rr_redir(t_data *data, t_ast *c, int *out)
{
	if (*out != STDOUT_FILENO)
		close(*out);
	*out = open(c->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*out == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	update_redir(t_data *data, t_ast *c, int in, int out)
{
	while (c)
	{
		if (c->token == L_REDIR)
			l_redir(data, c->left, &in);
		else if (c->token == LL_REDIR)
			ll_redir(c->left, &in);
		else if (c->token == R_REDIR)
			r_redir(data, c->left, &out);
		else if (c->token == RR_REDIR)
			rr_redir(data, c->left, &out);
		c = c->right;
	}
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
}
