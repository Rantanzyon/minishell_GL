/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:07:29 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/20 23:07:50 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstchr(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int		i;

	c = ft_lstat(lex, start);
	i = 0;
	while (c && i < end - start + 1)
	{
		if (token == WORD)
		{
			if (((t_final *)c->content)->token == token)
				return (start + i);
		}
		else
		{
			if (((t_final *)c->content)->token == R_REDIR || \
				((t_final *)c->content)->token == RR_REDIR || \
				((t_final *)c->content)->token == L_REDIR || \
				((t_final *)c->content)->token == LL_REDIR)
				return (start + i);
		}
		i++;
		c = c->next;
	}
	return (-1);
}

int	ft_first_parenthesis(t_list **lex, int start)
{
	t_list *c;

	c = ft_lstat(lex, start);
	if (((t_final *)c->content)->token == PAR_L)
		return (start);
	return (-1);
}

int	find_last_par(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	count = 0;
	while (c && i < end - start + 1)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count == 0 && (((t_final *)c->content)->token == PAR_R))
			return (start + i);
		i++;
		c = c->next;
	}
	return (-1);
}

int	ft_lstchr_andor(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	count = 0;
	while (c && i < end - start)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count == 0 && (((t_final *)c->content)->token == OR || ((t_final *)c->content)->token == AND))
			return (start + i);
		i++;
		c = c->next;
	}
	return (-1);
}

int	ft_lstchr_par2(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	count = 0;
	while (c && i < end - start)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count == 0 && (((t_final *)c->content)->token == OR || ((t_final *)c->content)->token == AND))
			return (start + i);
		i++;
		c = c->next;
	}
	return (-1);
}

int	ft_lstchr_par(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	count = 0;
	while (c && i < end - start)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
		{
			count--;
			if (count == 0)
				return (start + i);
		}
		if (count < 0)
			return (-1);
		i++;
		c = c->next;
	}
	return (-1);
}

int	ft_findlast(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int		i;
	int		n;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	n = -1;
	count = 0;
	while (c && i < end - start + 1)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count == 0 && ((t_final *)c->content)->token == token)
			n = start + i;
		i++;
		c = c->next;
	}
	return (n);
}

int	ft_findlast_andor(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		n;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	n = -1;
	count = 0;
	while (c && i < end - start + 1)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count == 0 && (((t_final *)c->content)->token == AND || ((t_final *)c->content)->token == OR))
			n = start + i;
		i++;
		c = c->next;
	}
	return (n);
}

int	ft_find_andor(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		count;

	c = ft_lstat(lex, start);
	i = 0;
	count = 0;
	while (c && i < end - start + 1)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count == 0 && (((t_final *)c->content)->token == AND || ((t_final *)c->content)->token == OR))
			return (start + i);
		i++;
		c = c->next;
	}
	return (-1);
}

t_ast	*fill_node(t_data *data, t_ast *c, int i)
{
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->hdfd = -1;
	c->right = NULL;
	c->left = NULL;
	return (c);
}
