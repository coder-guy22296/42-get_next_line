/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/25 18:16:30 by cyildiri          #+#    #+#             */
/*   Updated: 2016/10/25 18:20:19 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	This version of get_next_line is going to have a linked map implementation
**	that will be used as a buffer. That way, you can call gnl on any files 
**	in any order without having to finish reading a file before reading another.
**	A linked map will allow for any number of buffers(one per file desciriptor)
*/

#include "get_next_line.h"
#include <fcntl.h>

int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	int				line_len;
	char			*newline_ptr;
	char			*write_buff;
	int				bytes_read;

	write_buff = ft_strnew(BUFF_SIZE);
	cur_buff = ft_lmapget(buffer_map, &fd);
	// if there is no buffer for this file descriptor make one and fill buffer
	if (cur_buff == 0)
	{
		cur_buff = ft_lmapnew(&fd, ft_strnew(BUFF_SIZE),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		
		bytes_read = read(fd, cur_buff->content, BUFF_SIZE);
		ft_lmapadd(&buffer_map, cur_buff);
	}

	while(!(newline_ptr = (char *)ft_strchr(cur_buff->content, '\n')) && bytes_read == BUFF_SIZE)
	{
		*line = ft_strjoin(*line, cur_buff->content);
		bytes_read = read(fd, cur_buff->content, BUFF_SIZE);
	}
	if (newline_ptr)//newline in buffer (should always be true at this point in the code!!)
	{
		line_len =  newline_ptr - (char *)cur_buff->content;
		ft_strncpy(write_buff, cur_buff->content, line_len);
		*line = ft_strjoin(*line, write_buff);
		ft_strclr(write_buff);
		ft_memmove(cur_buff->content, newline_ptr + 1,
					BUFF_SIZE - (line_len + 1));
		if (bytes_read < BUFF_SIZE)
			cur_buff->content[BUFF_SIZE - (line_len + 1)] = '\0'
		else
			bytes_read = read(fd, &cur_buff->content[BUFF_SIZE - (line_len + 1)], line_len + bytes_read);
	}
	else if (bytes_read < BUFF_SIZE)
	{
		ft_strncpy(write_buff, cur_buff->content, bytes_read);
		*line = ft_strjoin(*line, write_buff);
	}
	ft_strdel(cur_buff);
	ft_strdel(write_buff);
}
