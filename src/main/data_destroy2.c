/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:00:30 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/25 01:01:02 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_in(t_data *data)
{
	if (data->in != STDIN_FILENO && data->in != -1)
	{
		close(data->in);
		data->in = -1;
	}
}

void	close_out(t_data *data)
{
	if (data->out != STDOUT_FILENO && data->out != -1)
	{
		close(data->out);
		data->out = -1;
	}
}

void	free_env(t_list **a)
{
	t_list	*temp;
	t_list	*cursor;

	if (!a)
		return ;
	cursor = *a;
	while (cursor)
	{
		temp = cursor->next;
		free(cursor->content);
		free(cursor);
		cursor = temp;
	}
	free(a);
}

void	free_final_lex(t_list **a)
{
	t_list	*temp;
	t_list	*cursor;

	if (!a)
		return ;
	cursor = *a;
	while (cursor)
	{
		temp = cursor->next;
		free(((t_final *)cursor->content)->str);
		free(((t_final *)cursor->content));
		free(cursor);
		cursor = temp;
	}
	free(a);
}

void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

