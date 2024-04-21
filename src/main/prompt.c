/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:36 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/21 01:54:49 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(t_data *data)
{
	char	*prompt_name;

	prompt_name = ft_strjoin(BLU, "minishell: ");
	prompt_name = gnl_strjoin(prompt_name, DEF);
	if (!prompt_name)
		reloop(data, "prompt_name", strerror(ENOMEM));
	data->input = readline(prompt_name);
	free(prompt_name);
	if (!data->input || !ft_strcmp(data->input, "exit"))
		data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
	if (ft_strlen(data->input) == 0)
		reloop(data, NULL, NULL);
	else
		add_history(data->input);
}
