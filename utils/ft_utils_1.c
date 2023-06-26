/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 16:57:57 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/11 17:21:05 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	*ft_lstnew_data(t_file *file_name, t_cmd *cmd)
{
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (!new)
		return (NULL);
	new->cmd = cmd;
    new->file = file_name;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_data(t_data **lst, t_data *new)
{
	t_data	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

t_file	*ft_lstnew_file(char *file_name, int type)
{
	t_file	*new;

	new = malloc(sizeof(t_file));
	if (!new)
		return (NULL);
    new->filename = file_name;
    new->type = type;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_back_file(t_file **lst, t_file *new)
{
	t_file	*ptr;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

t_cmd	*ft_lstnew_cmd(char *cmd)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
    new->cmd = cmd;
	new->next = NULL;
	return (new);
}
