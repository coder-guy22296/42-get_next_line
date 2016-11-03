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
	int				bytes_read;

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
		//printf("Function: line len: %d\n", line_len);
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
		//printf("Function: read_len:%d\n", buffer->read_len);
		//printf("Function: line_len:%d\n", line_len);
		//printf("Function: buf_util:%d\n", buffer->buf_util);
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
			bytes_read = read(fd, &buffer->buffer[buffer->buf_util], BUFF_SIZE - buffer->buf_util);
			if (bytes_read > 0)
				buffer->buf_util += bytes_read;
			//printf("Function: bytes Read3: %d\n", buffer->read_len);
		//}
	}
	//	if no newline in buffer
	else/* if (buffer->read_len < BUFF_SIZE)*/
	{
		printf("Function: \t\tBuffer State: EOF_IN_BUFFER\n");
		//printf("Function: bytes Read: %d\n", buffer->read_len);
		ft_strncpy(write_buff, buffer->buffer, buffer->buf_util);
		buffer->buf_util = 0;
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

//#define printf(...)

int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	char			*newline_ptr;	
	t_buff			*buffer;
	int				bytes_read;

	newline_ptr = 0;
	bytes_read = 0;
	cur_buff = ft_lmapget(buffer_map, &fd);
	if (cur_buff)
		printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
	if (!(*line = ft_strnew(0)))
		return (0);
	if (cur_buff == 0)//	if there is no buffer for this file descriptor make one and fill buffer
	{
		printf("Function: \t\tBuffer State: NO_BUFFER\n");
		if(!(cur_buff = ft_lmapnew(&fd, ft_newbuffer(BUFF_SIZE + 1, sizeof(char)),
										sizeof(t_buff), sizeof(int))))
			return (0);

			//printf("buffer address: %p\n", ((t_buff *)cur_buff->content)->buffer);
	//printf("lmap key address: %p\n", cur_buff->key);
	//printf("buffer struct address: %p\n", cur_buff->content);
	//printf("lmap address: %p\n", cur_buff);

		bytes_read = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
		if (bytes_read > 0)
			((t_buff *)cur_buff->content)->buf_util += bytes_read;
		printf("Function: bytes Read1: %d\n", bytes_read);
		printf("Function: FILL BUFF\n");
		//printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
		ft_lmapadd(&buffer_map, cur_buff);
	}
	buffer = (t_buff *)cur_buff->content;
	printf("cur_buff:%p newline_ptr:%p buffer:%p buffer_map:%p \n",cur_buff,newline_ptr,buffer->buffer,buffer_map);
	printf("buff size:%d buf_util:%d\n", BUFF_SIZE, buffer->buf_util);
	write(1, "I MADE IT BRO\n", 14);

	write(1, "I MADE IT BRO\n", 14);
	while(!(newline_ptr = (char *)ft_strchr(buffer->buffer, '\n')) && buffer->buf_util == BUFF_SIZE)
	{//	while no newline in buffer && buffer is full
		printf("Function: \t\tBuffer State: FULL_BUFFER\n");
		printf("Function: BUFF:\n'%s'\n", buffer->buffer);
		*line = ft_strjoin(*line, buffer->buffer);
		buffer->buf_util = 0;
		printf("Function: line: %s\n", *line);
		printf("Function: top off buffer\n");
		bytes_read = read(fd, buffer->buffer, BUFF_SIZE);
		if (bytes_read > 0)
			buffer->buf_util += bytes_read;
		//printf("Function: bytes Read2: %d\n", buffer->read_len);
	}//null the rest of the buffer if not full
	if (buffer->buf_util != 0)
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




