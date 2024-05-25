/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:45 by bbialy            #+#    #+#             */
/*   Updated: 2024/05/25 09:26:12 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*data_creation(char **argv, char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		data_destroy_exit(data, EXIT_FAILURE, "data", strerror(ENOMEM));
	data->argv = argv;
	data->envp = envp;
	data->exit = EXIT_SUCCESS;
	return (data);
}
