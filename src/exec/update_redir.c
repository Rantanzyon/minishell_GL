/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:04:18 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/14 00:51:09 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	l_redir(t_data *data, t_ast *c)
{
	if (data->fd_in != STDIN_FILENO)
		close(data->fd_in);
	data->fd_in = open(c->str, O_RDONLY);
	if (data->fd_in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	ll_redir(t_data *data, t_ast *c)
{
	if (data->fd_in != STDIN_FILENO)
		close(data->fd_in);
	data->fd_in = c->hdfd;
}

void	r_redir(t_data *data, t_ast *c)
{
	if (data->fd_out != STDOUT_FILENO)
		close(data->fd_out);
	data->fd_out = open(c->str,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fd_in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	rr_redir(t_data *data, t_ast *c)
{
	if (data->fd_out != STDOUT_FILENO)
		close(data->fd_out);
	data->fd_out = open(c->str,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->fd_in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
}

void	update_redir(t_data *data, t_ast *c)
{
	int	token;

	token = c->token;
	while (token != PIPE && token != WORD && token != FILENAME)
	{
		if (token == L_REDIR)
			l_redir(data, c->left);
		else if (token == LL_REDIR)
			ll_redir(data, c);
		else if (token == R_REDIR)
			r_redir(data, c->left);
		else
			rr_redir(data, c->left);
		c = c->right;
		token = c->token;
	}
}
