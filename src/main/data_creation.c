/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:45 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/14 00:24:05 by glemaire         ###   ########.fr       */
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
	return (data);
}
