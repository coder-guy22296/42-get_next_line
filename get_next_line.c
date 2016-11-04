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
#include "libft/includes/libft.h"
#include <unistd.h>

static void handle_partial_Buffer(t_lmap *cur_buff, char *newline_ptr, char **line, int fd)
{
	int				line_len;
	t_buff			*buffer;
	int				bytes_read;

	buffer = (t_buff *)cur_buff->content;
	newline_ptr = (char *)ft_strchr(buffer->buffer, '\n');
	if (newline_ptr)
	{
		line_len =  newline_ptr - (char *)buffer->buffer;
		*line = ft_strnjoin(*line, buffer->buffer, line_len);
		ft_memmove(buffer->buffer, newline_ptr + 1,
					buffer->buf_util - (line_len + 1));
		buffer->buf_util -= line_len + 1;
		ft_bzero(&buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
		bytes_read = read(fd, &buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
		if (bytes_read > 0)
			buffer->buf_util += bytes_read;
	}
	else
	{
		*line = ft_strnjoin(*line, buffer->buffer, buffer->buf_util);
		buffer->buf_util = 0;
	}
}

int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	char			*newline_ptr;	
	t_buff			*buffer;
	int				bytes_read;

	newline_ptr = 0;
	bytes_read = 0;
	if (BUFF_SIZE <= 0)
		return (-1);
	cur_buff = ft_lmapget(buffer_map, &fd);
	if (!(*line = ft_strnew(0)))
		return (-1);
	if (cur_buff == 0)
	{
		if(!(cur_buff = ft_lmapnew(&fd, ft_newbuffer(BUFF_SIZE + 1, sizeof(char)),
										sizeof(t_buff), sizeof(int))))
			return (-1);
		bytes_read = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
		if (bytes_read > 0)
			((t_buff *)cur_buff->content)->buf_util += bytes_read;
		ft_lmapadd(&buffer_map, cur_buff);
	}
	buffer = (t_buff *)cur_buff->content;
	if (buffer->buf_util == 0)
		return (0);
	while(!(newline_ptr = (char *)ft_strchr(buffer->buffer, '\n')) && buffer->buf_util == BUFF_SIZE)
	{
		*line = ft_strjoin(*line, buffer->buffer);
		buffer->buf_util = 0;
		bytes_read = read(fd, buffer->buffer, BUFF_SIZE);
		if (bytes_read > 0)
			buffer->buf_util += bytes_read;
	}
	if (buffer->buf_util != 0)
		ft_bzero(&buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
	handle_partial_Buffer(cur_buff, newline_ptr, line, fd);
	return (1);
}