/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:57 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/01 09:23:15 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	free_ast(t_ast *node)
{
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node->content);
	free(node);
} */

void	free_final_lex(t_list **a)
{
	t_list	*temp;
	
	if (!a)
		return ;
	while (*a)
	{
		temp = (*a)->next;
		free(((t_final *)(*a)->content)->str);
		free((*a)->content);
		free(*a);
		*a = temp;
	}
	free(a);
}

void	data_destroy(t_data *data, char *err)
{
	if (err)
	{
		ft_putstr_fd(err, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (data)
	{
		ft_lstclear(data->lex, free);
		free(data->lex);
		//data->final_lex = NULL;
		//printf("oui\n");
		free_final_lex(data->final_lex);
		//printf("oui\n");
		//free_ast(*data->ast);
		free(data->input);
	}
}

void	data_destroy_exit(t_data *data, int status, char *err)
{
	data_destroy(data, err);
	free(data);
	rl_clear_history();
	exit(status);
}

void	reloop(t_data *data, char *err)
{
	data_destroy(data, err);
	ft_loop(data);
}
