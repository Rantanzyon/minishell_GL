/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:36 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/05 06:31:11 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static char	*pwd_resize(t_data *data)
{
	char	*pwd;
	t_list	*cursor;

	cursor = (*data->env);
 	while (cursor)
	{
		if (!ft_strncmp("PWD=", ((char *)cursor->content), 4))
		{
			pwd = ft_substr(((char *)cursor->content), 4, \
				ft_strlen((char *)cursor->content) - 4);
			if (!pwd)
				reloop(data, "pwd : Allocation failure (pwd_resize)");
		}
		cursor = cursor->next;
	}
	return (pwd);
} */

void	prompt(t_data *data)
{
	data->input = readline("\033[34mminishell: \033[0m");
	//printf("%s\n", data->input);
	if (!data->input || !ft_strcmp(data->input, "exit"))
		data_destroy_exit(data, EXIT_SUCCESS, NULL);
	if (ft_strlen(data->input) != 0)
		add_history(data->input);
}
