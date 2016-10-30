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
static void handle_partial_Buffer(t_lmap *cur_buff, char *newline_ptr, int bytes_read, char **line, int fd)
{
	char			*write_buff;
	int				line_len;

	printf("Function: \t\tBuffer State: (EOF_IN_BUFFER || NEWLINE_IN_BUFFER)\n");
	printf("Function: BUFF:\n'%s'\n", cur_buff->content);
	write_buff = ft_strnew(BUFF_SIZE);
	newline_ptr = (char *)ft_strchr(cur_buff->content, '\n');
	if (newline_ptr)
	{
		printf("Function: \t\tBuffer State: NEWLINE_IN_BUFFER");
		if (bytes_read < BUFF_SIZE)
				printf(" && EOF_IN_BUFFER");
		printf("\n");
		line_len =  newline_ptr - (char *)cur_buff->content;
		printf("Function: line len: %d\n", line_len);
		ft_strncpy(write_buff, cur_buff->content, line_len);
		printf("Function: copy rest of line to output\n");
		*line = ft_strjoin(*line, write_buff);
		printf("Function: line: %s\n", *line);
		ft_strclr(write_buff);

		printf("Function: BUFF:\n'%s'\n", cur_buff->content);
		ft_memmove(cur_buff->content, newline_ptr + 1,
					bytes_read - (line_len + 1));
		printf("Function: shifting remaining buffer\n");
		printf("Function: BUFF:\n'%s'\n", cur_buff->content);
		if (bytes_read < BUFF_SIZE)
		{
			printf("Function: zero the buffer\n");
			ft_bzero(&cur_buff->content[bytes_read - line_len - 1], BUFF_SIZE - bytes_read + line_len + 1);
		}
		else
		{
			printf("Function: top off buffer\n");
			bytes_read = read(fd, &cur_buff->content[BUFF_SIZE - (line_len + 1)], line_len + 1);
			printf("Function: bytes Read3: %d\n", bytes_read);
		}
	}
	//	if no newline in buffer
	else/* if (bytes_read < BUFF_SIZE)*/
	{
		printf("Function: \t\tBuffer State: EOF_IN_BUFFER\n");
		printf("Function: bytes Read: %d\n", bytes_read);
		ft_strncpy(write_buff, cur_buff->content, bytes_read);
		*line = ft_strjoin(*line, write_buff);
		ft_strclr(cur_buff->content);
	}
	ft_strdel(&write_buff);
	printf("Function: BUFF:\n'%s'\n", cur_buff->content);
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
	int				bytes_read;

	//bytes_read = 0;
	newline_ptr = 0;
	cur_buff = ft_lmapget(buffer_map, &fd);
	*line = ft_strnew(0);//needs protection
	if (cur_buff == 0)//	if there is no buffer for this file descriptor make one and fill buffer
	{
		printf("Function: \t\tBuffer State: NO_BUFFER\n");
		cur_buff = ft_lmapnew(&fd, ft_newbuff(BUFF_SIZE),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		bytes_read = read(fd, cur_buff->content, BUFF_SIZE);
		printf("Function: bytes Read1: %d\n", bytes_read);
		printf("Function: filling buffer\n");
		//printf("Function: '%s'\n", cur_buff->content);
		ft_lmapadd(&buffer_map, cur_buff);
	}
	while(!(newline_ptr = (char *)ft_strchr(cur_buff->content, '\n')) && bytes_read == BUFF_SIZE)
	{//	while no newline in buffer && buffer is full
		printf("Function: \t\tBuffer State: FULL_BUFFER\n");
		printf("Function: BUFF:\n'%s'\n", cur_buff->content);
		*line = ft_strjoin(*line, cur_buff->content);
		printf("Function: line: %s\n", *line);
		printf("Function: top off buffer\n");
		bytes_read = read(fd, cur_buff->content, BUFF_SIZE);
		printf("Function: bytes Read2: %d\n", bytes_read);
	}//null the rest of the buffer if not full
	printf("I MADE IT BRO\n");
	if (bytes_read != 0)
		ft_bzero(&cur_buff->content[bytes_read], BUFF_SIZE - bytes_read);
	handle_partial_Buffer(cur_buff, newline_ptr, bytes_read, line, fd);
	return (1);
}


/*
int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	char			*newline_ptr;
	int				bytes_read;
	int				line_len;
	char			*write_buff;

	write_buff = ft_strnew(BUFF_SIZE);
	cur_buff = ft_lmapget(buffer_map, &fd);
	//	if there is no buffer for this file descriptor make one and fill buffer
	if (cur_buff == 0)
	{
		cur_buff = ft_lmapnew(&fd, ft_strnew(BUFF_SIZE),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		bytes_read = read(fd, cur_buff->content, BUFF_SIZE);
		
	}
	//	while no newline in buffer && buffer is full
	while(!(newline_ptr = (char *)ft_strchr(cur_buff->content, '\n')) && bytes_read == BUFF_SIZE)
	{
		*line = ft_strjoin(*line, cur_buff->content);
		bytes_read = read(fd, cur_buff->content, BUFF_SIZE);
	}
	//null the rest of the buffer if not full
	ft_bzero(&cur_buff->content[bytes_read], BUFF_SIZE - bytes_read);
	//	if newline in buffer (should always be true at this point in the code!!)
	if (newline_ptr)
	{
		line_len =  newline_ptr - (char *)cur_buff->content;
		ft_strncpy(write_buff, cur_buff->content, line_len);
		*line = ft_strjoin(*line, write_buff);
		ft_strclr(write_buff);
		ft_memmove(cur_buff->content, newline_ptr + 1,
					BUFF_SIZE - (line_len + 1));
		if (bytes_read < BUFF_SIZE)
			ft_bzero(&cur_buff->content[bytes_read - line_len - 1], BUFF_SIZE - bytes_read + line_len + 1);
		else
			bytes_read = read(fd, &cur_buff->content[BUFF_SIZE - (line_len + 1)], line_len + bytes_read);
	}
	//	if buffer not full && newline in buffer
	else if (bytes_read < BUFF_SIZE)
	{
		ft_strncpy(write_buff, cur_buff->content, bytes_read);
		*line = ft_strjoin(*line, write_buff);
	}
	ft_lmapadd(&buffer_map, cur_buff);
	ft_strdel(&write_buff);
	return (1);
}
*/




