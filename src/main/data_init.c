/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:25 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/07 00:02:35 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_creation(t_data *data)
{
	int		i;
	t_list	*new;
	char	*str;
	
	data->env = (t_list **)malloc(sizeof(t_list *));
	if (!data->env)
		reloop(data, "data->env: Allocation Failure (env_creation)");
	*(data->env) = NULL;
	i = 0;
	while (data->envp[i])
	{
		str = ft_strdup(data->envp[i]);
		if (!str)
			reloop(data, "str: Allocation Failure (env_creation)");
		new = ft_lstnew(str);
		if (!new)
			reloop(data, "new: Allocation Failure (env_creation)");
		ft_lstadd_back(data->env, new);
		i++;
	}
}


void	data_init(t_data *data)
{
	data->input = NULL;
	data->lex = NULL;
	data->final_lex = NULL;
	data->ast = NULL;
	data->fd_in = STDIN_FILENO;
	data->fd_out = STDOUT_FILENO;
	//data->here_doc = -1;
	env_creation(data);
	//data->actual_pid = 74749;
}
