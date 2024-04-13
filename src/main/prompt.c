/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:36 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/14 00:20:45 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(t_data *data)
{
	data->input = readline("\033[34mminishell: \033[0m");
	if (!data->input || !ft_strcmp(data->input, "exit"))
		data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
	if (ft_strlen(data->input) == 0)
		reloop(data, NULL, NULL);
	else
		add_history(data->input);
}
