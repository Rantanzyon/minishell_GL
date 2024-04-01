/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 11:15:09 by glemaire          #+#    #+#             */
/*   Updated: 2023/10/28 20:11:17 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*start;
	void	*newcontent;

	if (!f || !del || !lst)
		return (NULL);
	start = NULL;
	while (lst)
	{
		newcontent = f(lst->content);
		new = ft_lstnew(newcontent);
		if (!new)
			return (ft_lstclear(&start, del), del(newcontent), NULL);
		ft_lstadd_back(&start, new);
		lst = lst->next;
	}
	return (start);
}
