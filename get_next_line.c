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
//#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> //REMOVE
/*
static void refil_buffer()
{

}
*/
static void handle_partial_Buffer(t_lmap *cur_buff, char *newline_ptr, char **line, int fd)
{
	char			*write_buff;
	int				line_len;
	t_buff			*buffer;

	buffer = (t_buff *)cur_buff->content;
	printf("Function: \t\tBuffer State: (EOF_IN_BUFFER || NEWLINE_IN_BUFFER)\n");
	printf("Function: BUFF:\n'%s'\n", buffer->buffer);
	write_buff = ft_strnew(BUFF_SIZE);
	newline_ptr = (char *)ft_strchr(buffer->buffer, '\n');
	if (newline_ptr)
	{
		printf("Function: \t\tBuffer State: NEWLINE_IN_BUFFER");
		if (buffer->buf_util < BUFF_SIZE)
				printf(" && EOF_IN_BUFFER");
		printf("\n");
		line_len =  newline_ptr - (char *)buffer->buffer;
		printf("Function: line len: %d\n", line_len);
		ft_strncpy(write_buff, buffer->buffer, line_len);
		printf("Function: COPY\n");
		*line = ft_strjoin(*line, write_buff);
		printf("Function: line: %s\n", *line);
		ft_strclr(write_buff);

		printf("Function: BUFF:\n'%s'\n", buffer->buffer);
		ft_memmove(buffer->buffer, newline_ptr + 1,
					buffer->buf_util - (line_len + 1));
		buffer->buf_util -= line_len + 1;
		printf("Function: SHIFT\n");
		printf("Function: read_len:%d\n", buffer->read_len);
		printf("Function: line_len:%d\n", line_len);
		printf("Function: buf_util:%d\n", buffer->buf_util);
		printf("Function: BUFF:\n'%s'\n", buffer->buffer);
		//if (buffer->read_len < BUFF_SIZE)
		//{
			printf("Function: CLEAR EXTRA\n");
			ft_bzero(&buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
			printf("Function: BUFF:\n'%s'\n", buffer->buffer);
		//}
		//else
		//{
			printf("Function: FILL BUFF\n");
			buffer->read_len = read(fd, &buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
			buffer->buf_util += buffer->read_len;
			printf("Function: bytes Read3: %d\n", buffer->read_len);
		//}
	}
	//	if no newline in buffer
	else/* if (buffer->read_len < BUFF_SIZE)*/
	{
		printf("Function: \t\tBuffer State: EOF_IN_BUFFER\n");
		printf("Function: bytes Read: %d\n", buffer->read_len);
		ft_strncpy(write_buff, buffer->buffer, buffer->buf_util);
		*line = ft_strjoin(*line, write_buff);
		//ft_strclr(buffer->buffer);
	}
	ft_strdel(&write_buff);
	printf("Function: BUFF:\n'%s'\n", buffer->buffer);
}
/*
static void delete_buffer()
{
	
}

static void cleanup()
{

}
*/
int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	char			*newline_ptr;	
	t_buff			*buffer;

	newline_ptr = 0;
	cur_buff = ft_lmapget(buffer_map, &fd);
	if (cur_buff)
		printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
	*line = ft_strnew(0);//needs protection
	if (cur_buff == 0)//	if there is no buffer for this file descriptor make one and fill buffer
	{
		printf("Function: \t\tBuffer State: NO_BUFFER\n");
		cur_buff = ft_lmapnew(&fd, ft_newbuffer(BUFF_SIZE + 1, sizeof(char)),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		((t_buff *)cur_buff->content)->read_len = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
		((t_buff *)cur_buff->content)->buf_util += ((t_buff *)cur_buff->content)->read_len;
		printf("Function: bytes Read1: %d\n", ((t_buff *)cur_buff->content)->read_len);
		printf("Function: FILL BUFF\n");
		//printf("Function: '%s'\n", buffer->buffer);
		ft_lmapadd(&buffer_map, cur_buff);
	}
	buffer = (t_buff *)cur_buff->content;
	while(!(newline_ptr = (char *)ft_strchr(buffer->buffer, '\n')) && buffer->read_len == BUFF_SIZE)
	{//	while no newline in buffer && buffer is full
		printf("Function: \t\tBuffer State: FULL_BUFFER\n");
		printf("Function: BUFF:\n'%s'\n", buffer->buffer);
		*line = ft_strjoin(*line, buffer->buffer);
		printf("Function: line: %s\n", *line);
		printf("Function: top off buffer\n");
		buffer->read_len = read(fd, buffer->buffer, BUFF_SIZE);
		buffer->buf_util += buffer->read_len;
		printf("Function: bytes Read2: %d\n", buffer->read_len);
	}//null the rest of the buffer if not full
	printf("I MADE IT BRO\n");
	if (buffer->read_len != 0)
		ft_bzero(&buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
	handle_partial_Buffer(cur_buff, newline_ptr, line, fd);
	return (1);
}


/*
int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	char			*newline_ptr;
	int				buffer->read_len;
	int				line_len;
	char			*write_buff;

	write_buff = ft_strnew(BUFF_SIZE);
	cur_buff = ft_lmapget(buffer_map, &fd);
	//	if there is no buffer for this file descriptor make one and fill buffer
	if (cur_buff == 0)
	{
		cur_buff = ft_lmapnew(&fd, ft_strnew(BUFF_SIZE),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		buffer->read_len = read(fd, buffer->buffer, BUFF_SIZE);
		
	}
	//	while no newline in buffer && buffer is full
	while(!(newline_ptr = (char *)ft_strchr(buffer->buffer, '\n')) && buffer->read_len == BUFF_SIZE)
	{
		*line = ft_strjoin(*line, buffer->buffer);
		buffer->read_len = read(fd, buffer->buffer, BUFF_SIZE);
	}
	//null the rest of the buffer if not full
	ft_bzero(&buffer->buffer[buffer->read_len], BUFF_SIZE - buffer->read_len);
	//	if newline in buffer (should always be true at this point in the code!!)
	if (newline_ptr)
	{
		line_len =  newline_ptr - (char *)buffer->buffer;
		ft_strncpy(write_buff, buffer->buffer, line_len);
		*line = ft_strjoin(*line, write_buff);
		ft_strclr(write_buff);
		ft_memmove(buffer->buffer, newline_ptr + 1,
					BUFF_SIZE - (line_len + 1));
		if (buffer->read_len < BUFF_SIZE)
			ft_bzero(&buffer->buffer[buffer->read_len - line_len - 1], BUFF_SIZE - buffer->read_len + line_len + 1);
		else
			buffer->read_len = read(fd, &buffer->buffer[BUFF_SIZE - (line_len + 1)], line_len + buffer->read_len);
	}
	//	if buffer not full && newline in buffer
	else if (buffer->read_len < BUFF_SIZE)
	{
		ft_strncpy(write_buff, buffer->buffer, buffer->read_len);
		*line = ft_strjoin(*line, write_buff);
	}
	ft_lmapadd(&buffer_map, cur_buff);
	ft_strdel(&write_buff);
	return (1);
}
*/




