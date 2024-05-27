/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 16:20:07 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/26 16:49:38 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	word_fusion(t_data *data, t_list **c)
{
	char	*word;
	char	*temp;

	word = ft_strdup("");
	if (!word)
		data_destroy_exit(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	while (*c && ((t_final *)(*c)->content)->token == WORD)
	{
		temp = ft_strdup(((t_final *)(*c)->content)->str);
		if (!temp)
			data_destroy_exit(data, EXIT_FAILURE, "temp", strerror(ENOMEM));
		
		word = gnl_strjoin(word, temp);
		free(temp);
		if (!word)
			data_destroy_exit(data, EXIT_FAILURE, "word", strerror(ENOMEM));
		*c = (*c)->next;
	}
	ft_add_token(data, data->final_lex, word, WORD);
}

void	create_lst_final(t_data *data)
{
	t_list *c;
	char	*word;
	int		token;

	c = *data->temp_final_lex;
	while (c)
	{
		token = ((t_final *)c->content)->token;
		if (token == WORD)
			word_fusion(data, &c);
		else if (token != ESPACE)
		{
			word = ft_strdup(((t_final *)c->content)->str);
			if (!word)
				data_destroy_exit(data, EXIT_FAILURE, "word", strerror(ENOMEM));
			ft_add_token(data, data->final_lex, word, token);
			c = c->next;
		}
		else
			c = c->next;
	}
}

void	lexer_final(t_data *data)
{
	data->final_lex = (t_list **)malloc(sizeof(t_list *));
	if (!data->final_lex)
		reloop(data, EXIT_FAILURE, "data->final_lex", strerror(ENOMEM));
	*(data->final_lex) = NULL;
	create_lst_final(data);
}