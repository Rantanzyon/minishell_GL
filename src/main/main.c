/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:46 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/01 08:48:40 by glemaire         ###   ########.fr       */
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
		//parser(data);
		data_destroy(data, NULL);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void) argv;
	if (argc != 1)
		return (ft_putstr_fd("Wrong number of arguments\n", 2), 1);
	data = data_creation(envp);
	ft_loop(data);
	return (0);
}
