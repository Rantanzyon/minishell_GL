/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:46 by bbialy            #+#    #+#             */
/*   Updated: 2024/05/20 23:16:10 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void sig_handler(int signum)
{
    if (signum == SIGUSR1) {
        execve_success = 1;
    }
} */

    
void	ft_loop(t_data *data)
{
	while (1)
	{
		//dprintf(2, "exit = %d\n", data->exit);
		data_init(data);
		prompt(data);
		lexer(data);
		//print_lex(data);
		parser(data);
		//print_ast(*data->ast, 0);
		exec(data);
		wait(NULL);
		data_destroy(data, NULL, NULL);
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

/*
((((MERDE || ls)| >out1 cat >out2) && echo bonjour || echo bonsoir) | <<LIM grep a) && ((waxz || sleep 2) | ls -l) && wc <Makefile -l
*/