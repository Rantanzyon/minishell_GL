/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:04:18 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/04 13:35:42 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	l_redir(t_data *data, t_ast *c)
{
	int	fd;
	
	fd= open(c->str, O_RDONLY);
	if (fd == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
	data->in = fd;
	add_fd(data, fd);
}

void	ll_redir(t_data *data, t_ast *c)
{
	data->in = c->hdfd;
	add_fd(data, c->hdfd);
}

void	r_redir(t_data *data, t_ast *c)
{
	int	fd;
	
	fd = open(c->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
	data->out = fd;
	add_fd(data, fd);
}

void	rr_redir(t_data *data, t_ast *c)
{
	int	fd;
	
	fd = open(c->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str, strerror(errno));
	data->out = fd;
	add_fd(data, fd);
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
