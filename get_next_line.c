/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvlad <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 15:58:07 by mvlad             #+#    #+#             */
/*   Updated: 2017/03/02 17:49:37 by mvlad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <stdio.h>

static t_list	*file_manage(t_list **head, const int fd)
{
	t_list			*current;
	size_t			file_des;

	file_des = (size_t)fd;
	current = *head;
	while (current)
	{
		if (current->content_size == file_des)
			return (current);
		current = current->next;
	}
	current = ft_lstnew("\0", file_des);
	ft_lstadd(head, current);
	current = *head;
	return (current);
}

static int		gnl_magic(char **dst, char *src, const char delim)
{
	int	i;
	int	j;
	char *tmp;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i] == delim)
			break ;
		i++;
	}
	if ((*dst = ft_strnew((size_t)i))== NULL)
		return (0);
	tmp = *dst;
	while (src[j] && j < i)
	{
		tmp[j] = src[j];
		j++;
	}
	return (i);
}

int				get_next_line(const int fd, char **line)
{
	int				read_size;
	char			buf[BUFF_SIZE + 1];
	static t_list	*head;
	t_list			*current;

	if (line == NULL || fd < 0)
		return (-1);
	current = file_manage(&head, fd);
	PROTECT_N1((*line = ft_strnew(BUFF_SIZE)));
	while ((read_size = read(fd, buf, BUFF_SIZE)))
	{
		buf[read_size] = '\0';
		PROTECT_N1((current->content = ft_strjoin(current->content, buf)));
		if (ft_strchr(buf, '\n'))
			break ;
	}
	if (read_size < BUFF_SIZE && !ft_strlen(current->content))
		return (0);
	read_size = gnl_magic(line, current->content, '\n');
	if (read_size < (int)ft_strlen(current->content))
		current->content += (read_size + 1);
	else if (read_size >= (int)ft_strlen(current->content))
		ft_strclr(current->content);
	return (1);
}
