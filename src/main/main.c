/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:46 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/04 00:19:37 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_loop(t_data *data)
{
	while (1)
	{
		data_init(data);
		prompt(data);
		lexer(data);
		parser(data);
		exec(data, *(data->ast), STDIN_FILENO, STDOUT_FILENO);
		wait(NULL);
		data_destroy(data, NULL);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	if (argc != 1)
		return (ft_putstr_fd("Wrong number of arguments\n", 2), 1);
	data = data_creation(argv, envp);
	ft_loop(data);
	return (0);
}
