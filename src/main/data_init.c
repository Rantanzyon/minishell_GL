/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:25 by bbialy            #+#    #+#             */
/*   Updated: 2024/05/23 04:18:42 by glemaire         ###   ########.fr       */
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
		reloop(data, "data->env", strerror(ENOMEM));
	*(data->env) = NULL;
	i = 0;
	while (data->envp[i])
	{
		str = ft_strdup(data->envp[i]);
		if (!str)
			reloop(data, "str", strerror(ENOMEM));
		new = ft_lstnew(str);
		if (!new)
			reloop(data, "new", strerror(ENOMEM));
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
	data->pipelvl = 0;
	env_creation(data);
}
