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

	printf("Function: \t\tBuffer State: (EOF_IN_BUFFER || NEWLINE_IN_BUFFER)\n");
	printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
	write_buff = ft_strnew(BUFF_SIZE);
	newline_ptr = (char *)ft_strchr(((t_buff *)cur_buff->content)->buffer, '\n');
	if (newline_ptr)
	{
		printf("Function: \t\tBuffer State: NEWLINE_IN_BUFFER");
		if (((t_buff *)cur_buff->content)->read_len < BUFF_SIZE)
				printf(" && EOF_IN_BUFFER");
		printf("\n");
		line_len =  newline_ptr - (char *)((t_buff *)cur_buff->content)->buffer;
		printf("Function: line len: %d\n", line_len);
		ft_strncpy(write_buff, ((t_buff *)cur_buff->content)->buffer, line_len);
		printf("Function: COPY\n");
		*line = ft_strjoin(*line, write_buff);
		printf("Function: line: %s\n", *line);
		ft_strclr(write_buff);

		printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
		ft_memmove(((t_buff *)cur_buff->content)->buffer, newline_ptr + 1,
					((t_buff *)cur_buff->content)->read_len - (line_len + 1));
		printf("Function: SHIFT\n");
		printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
		if (((t_buff *)cur_buff->content)->read_len < BUFF_SIZE)
		{
			printf("Function: CLEAR EXTRA\n");
			ft_bzero(&((t_buff *)cur_buff->content)->buffer[((t_buff *)cur_buff->content)->read_len - line_len - 1], BUFF_SIZE - ((t_buff *)cur_buff->content)->read_len + line_len + 1);
		}
		else
		{
			printf("Function: FILL BUFF\n");
			((t_buff *)cur_buff->content)->read_len = read(fd, &((t_buff *)cur_buff->content)->buffer[BUFF_SIZE - (line_len + 1)], line_len + 1);
			printf("Function: bytes Read3: %d\n", ((t_buff *)cur_buff->content)->read_len);
		}
	}
	//	if no newline in buffer
	else/* if (((t_buff *)cur_buff->content)->read_len < BUFF_SIZE)*/
	{
		printf("Function: \t\tBuffer State: EOF_IN_BUFFER\n");
		printf("Function: bytes Read: %d\n", ((t_buff *)cur_buff->content)->read_len);
		ft_strncpy(write_buff, ((t_buff *)cur_buff->content)->buffer, ((t_buff *)cur_buff->content)->read_len);
		*line = ft_strjoin(*line, write_buff);
		ft_strclr(((t_buff *)cur_buff->content)->buffer);
	}
	ft_strdel(&write_buff);
	printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
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

	newline_ptr = 0;
	cur_buff = ft_lmapget(buffer_map, &fd);
	*line = ft_strnew(0);//needs protection
	if (cur_buff == 0)//	if there is no buffer for this file descriptor make one and fill buffer
	{
		printf("Function: \t\tBuffer State: NO_BUFFER\n");
		cur_buff = ft_lmapnew(&fd, ft_newbuffer(BUFF_SIZE + 1, sizeof(char)),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		((t_buff *)cur_buff->content)->read_len = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
		printf("Function: bytes Read1: %d\n", ((t_buff *)cur_buff->content)->read_len);
		printf("Function: FILL BUFF\n");
		//printf("Function: '%s'\n", ((t_buff *)cur_buff->content)->buffer);
		ft_lmapadd(&buffer_map, cur_buff);
	}
	while(!(newline_ptr = (char *)ft_strchr(((t_buff *)cur_buff->content)->buffer, '\n')) && ((t_buff *)cur_buff->content)->read_len == BUFF_SIZE)
	{//	while no newline in buffer && buffer is full
		printf("Function: \t\tBuffer State: FULL_BUFFER\n");
		printf("Function: BUFF:\n'%s'\n", ((t_buff *)cur_buff->content)->buffer);
		*line = ft_strjoin(*line, ((t_buff *)cur_buff->content)->buffer);
		printf("Function: line: %s\n", *line);
		printf("Function: top off buffer\n");
		((t_buff *)cur_buff->content)->read_len = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
		printf("Function: bytes Read2: %d\n", ((t_buff *)cur_buff->content)->read_len);
	}//null the rest of the buffer if not full
	printf("I MADE IT BRO\n");
	if (((t_buff *)cur_buff->content)->read_len != 0)
		ft_bzero(&((t_buff *)cur_buff->content)->buffer[((t_buff *)cur_buff->content)->read_len], BUFF_SIZE - ((t_buff *)cur_buff->content)->read_len);
	handle_partial_Buffer(cur_buff, newline_ptr, line, fd);
	return (1);
}


/*
int	get_next_line(const int fd, char **line)
{
	static t_lmap	*buffer_map = 0;
	t_lmap			*cur_buff;
	char			*newline_ptr;
	int				((t_buff *)cur_buff->content)->read_len;
	int				line_len;
	char			*write_buff;

	write_buff = ft_strnew(BUFF_SIZE);
	cur_buff = ft_lmapget(buffer_map, &fd);
	//	if there is no buffer for this file descriptor make one and fill buffer
	if (cur_buff == 0)
	{
		cur_buff = ft_lmapnew(&fd, ft_strnew(BUFF_SIZE),
						(sizeof(char) * (BUFF_SIZE + 1)), sizeof(int));
		((t_buff *)cur_buff->content)->read_len = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
		
	}
	//	while no newline in buffer && buffer is full
	while(!(newline_ptr = (char *)ft_strchr(((t_buff *)cur_buff->content)->buffer, '\n')) && ((t_buff *)cur_buff->content)->read_len == BUFF_SIZE)
	{
		*line = ft_strjoin(*line, ((t_buff *)cur_buff->content)->buffer);
		((t_buff *)cur_buff->content)->read_len = read(fd, ((t_buff *)cur_buff->content)->buffer, BUFF_SIZE);
	}
	//null the rest of the buffer if not full
	ft_bzero(&((t_buff *)cur_buff->content)->buffer[((t_buff *)cur_buff->content)->read_len], BUFF_SIZE - ((t_buff *)cur_buff->content)->read_len);
	//	if newline in buffer (should always be true at this point in the code!!)
	if (newline_ptr)
	{
		line_len =  newline_ptr - (char *)((t_buff *)cur_buff->content)->buffer;
		ft_strncpy(write_buff, ((t_buff *)cur_buff->content)->buffer, line_len);
		*line = ft_strjoin(*line, write_buff);
		ft_strclr(write_buff);
		ft_memmove(((t_buff *)cur_buff->content)->buffer, newline_ptr + 1,
					BUFF_SIZE - (line_len + 1));
		if (((t_buff *)cur_buff->content)->read_len < BUFF_SIZE)
			ft_bzero(&((t_buff *)cur_buff->content)->buffer[((t_buff *)cur_buff->content)->read_len - line_len - 1], BUFF_SIZE - ((t_buff *)cur_buff->content)->read_len + line_len + 1);
		else
			((t_buff *)cur_buff->content)->read_len = read(fd, &((t_buff *)cur_buff->content)->buffer[BUFF_SIZE - (line_len + 1)], line_len + ((t_buff *)cur_buff->content)->read_len);
	}
	//	if buffer not full && newline in buffer
	else if (((t_buff *)cur_buff->content)->read_len < BUFF_SIZE)
	{
		ft_strncpy(write_buff, ((t_buff *)cur_buff->content)->buffer, ((t_buff *)cur_buff->content)->read_len);
		*line = ft_strjoin(*line, write_buff);
	}
	ft_lmapadd(&buffer_map, cur_buff);
	ft_strdel(&write_buff);
	return (1);
}
*/




