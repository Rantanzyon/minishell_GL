/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_exp_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:02 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/01 00:08:32 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_str(t_data *data, char *name, int n)
{
	t_lex	*new_stru;
	t_list	*new_node;
	int		i;

	i = 0;
	while (name[i])
	{
		new_stru = (t_lex *)malloc(sizeof(t_lex));
		if (!new_stru)
			reloop(data, "new_stru : Allocation failure (convert_exp)");
		new_stru->pretok = CHAR;
		new_stru->c = name[i];
		new_node = ft_lstnew(new_stru);
		if (!new_node)
			reloop(data, "new_node : Allocation failure (convert_exp)");
		ft_lstadd_at(data->lex, new_node, n++);
		i++;
	}
}

static char	*create_name(t_data *data, t_list *cursor)
{
	char	*name;
	int		len;
	int		i;
	t_list	*nextnode;

	len = 0;
	nextnode = cursor->next;
	while (nextnode && (((t_lex *)nextnode->content)->c == '_' || \
			ft_isalpha(((t_lex *)nextnode->content)->c)))
	{
		len++;
		nextnode = nextnode->next;
	}
	name = malloc((len + 1) * sizeof(char));
	if (!name)
		reloop(data, "name : Allocation failure (exp_var)");
	i = 0;
	nextnode = cursor->next;
	while (i < len)
	{
		name[i++] = ((t_lex *)nextnode->content)->c;
		nextnode = nextnode->next;
	}
	name[i] = '\0';
	return (name);
}

static void	ft_lstadd_osef(t_data *data, int i)
{
	t_lex	*new_stru;
	t_list	*new_node;

	new_stru = (t_lex *)malloc(sizeof(t_lex));
	if (!new_stru)
		reloop(data, "new_stru : Allocation failure (convert_exp)");
	new_stru->pretok = OSEF;
	new_stru->c = ' ';
	new_node = ft_lstnew(new_stru);
	if (!new_node)
		reloop(data, "new_node : Allocation failure (convert_exp)");
	ft_lstadd_at(data->lex, new_node, i);
}

static void	add_exp(t_data *data, char *name, int len, int i)
{
	char	*exp;
	int		j;

	exp = NULL;
	j = 0;
	while (data->envp[j])
	{
		if (!ft_strncmp(name, data->envp[j], len) && \
			data->envp[j][len] == '=' && !name[len])
		{
			exp = ft_substr(data->envp[j], len + 1, \
				ft_strlen(data->envp[j]) - (len + 1));
			if (!exp)
				reloop(data, "exp : Allocation failure (exp_var)");
		}
		j++;
	}
	free(name);
	if (!exp)
		ft_lstadd_osef(data, i);
	else
		ft_lstadd_str(data, exp, i);
	free(exp);
}

t_list	*exp_var(t_data *data, t_list *cursor)
{
	char	*name;
	int		len;
	int		i;

	name = create_name(data, cursor);
	len = ft_strlen(name);
	i = ft_lstindex(data->lex, cursor);
	ft_lstdel_n(data->lex, len + 1, i);
	add_exp(data, name, len, i);
	cursor = ft_lstat(data->lex, i);
	return (cursor);
}
