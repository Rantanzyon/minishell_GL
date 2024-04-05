/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:45 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/05 09:20:32 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_creation(t_data *data)
{
	data->exec = (t_exec *)malloc(sizeof(t_exec));
	if (!data->exec)
		reloop(data, "data->exec: Allocation Failure (exec_creation)");
	data->exec->fd_in = STDIN_FILENO;
	data->exec->fd_out = STDOUT_FILENO;
	data->exec->here_doc = -1;
	data->exec->exit_status = 0;
}

t_data	*data_creation(char **argv, char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		data_destroy_exit(data, EXIT_FAILURE,
			"data : Allocation failure (data_creation)");
	data->argv = argv;
	data->envp = envp;
	exec_creation(data);
	return (data);
}
