/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:25 by bbialy            #+#    #+#             */
/*   Updated: 2024/05/26 16:55:17 by glemaire         ###   ########.fr       */
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
		reloop(data, EXIT_FAILURE, "data->env", strerror(ENOMEM));
	*(data->env) = NULL;
	i = 0;
	while (data->envp[i])
	{
		str = ft_strdup(data->envp[i]);
		if (!str)
			reloop(data, EXIT_FAILURE, "str", strerror(ENOMEM));
		new = ft_lstnew(str);
		if (!new)
			reloop(data, EXIT_FAILURE, "new", strerror(ENOMEM));
		ft_lstadd_back(data->env, new);
		i++;
	}
}

void	data_init(t_data *data)
{
	data->input = NULL;
	data->lex = NULL;
	data->temp_final_lex = NULL;
	data->final_lex = NULL;
	data->ast = NULL;
	data->pipelvl = 0;
	data->in = STDIN_FILENO;
	data->out = STDOUT_FILENO;
	data->path = NULL;
	data->args = NULL;
	env_creation(data);
}
