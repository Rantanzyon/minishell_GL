/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:52:19 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/05 13:08:23 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	eof(char *lim, t_data *data)
{
	free(lim);
	ft_printf("bash: warning: ");
	ft_printf("here-document at line 1 delimited by end-of-file ");
	ft_printf("(wanted `%s')\n", data->av[2]);
	destroy_all(data, "", EXIT_FAILURE);
}

static void	gnl_heredoc(t_data *data)
{
	char	*str;
	char	*lim;

	lim = ft_strjoin(data->av[2], "\n");
	if (!lim)
		destroy_all(data, "lim", EXIT_FAILURE);
	pipe(data->pipefd);
	while (1)
	{
		str = get_next_line(1);
		if (!str)
			eof(lim, data);
		if (ft_strcmp(str, lim) == 0)
		{
			free(str);
			break ;
		}
		ft_putstr_fd(str, data->pipefd[1]);
		free(str);
	}
	free(lim);
}

void	do_heredoc(t_data *data, t_ast *c)
{
	char	*str;
	char	*lim;
	int		fd[2];
	
	lim = ft_strjoin(data->av[2], "\n");
	if (!lim)
		reloop(data, "lim");
	while (1)
	{
		ft_putstr_fd(">", STDOUT_FILENO);
		
	}
	
	
	
}

void	heredoc(t_data *data, t_ast *c)
{
	if (c && c->token == LL_REDIR)
		do_heredoc(data, c->left);
	else if (c)
	{
		heredoc(data, c->left);
		heredoc(data, c->right);
	}
}
