/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:52:19 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/09 15:15:27 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_limiter_cmp(char *limiter, char *line)
{
	if (ft_strlen(limiter) == ft_strlen(line) - 1)
	{
		if (!ft_strncmp(limiter, line, ft_strlen(line) - 1)
			&& line[ft_strlen(line) - 1] == '\n')
			return (0);
	}
	return (-1);
}

static void	fill_heredoc(int fd, char *limiter)
{
	char	*line;

	line = NULL;
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_putstr_fd("\nUser input terminated\n", STDERR_FILENO);
			free(line);
			break ;
		}
		if (!ft_limiter_cmp(limiter, line))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
}

static void	do_heredoc(t_data *data, t_ast *node)
{
	int		hdfd[2];
	char	*limiter;

	if (pipe(hdfd) == -1)
		reloop(data, "pipe hd failure");
	node->hdfd = hdfd[0];
	limiter = node->left->str;
	fill_heredoc(hdfd[1], limiter);
	close(hdfd[1]);
}

void	heredoc(t_data *data, t_ast *node)
{
	if (node)
	{
		if (node->token == LL_REDIR)
			do_heredoc(data, node);
		heredoc(data, node->left);
		heredoc(data, node->right);
	}
}
