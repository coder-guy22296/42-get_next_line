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

int	get_next_line(const int fd, char **line)
{
	static t_lmap *buffer_map = NULL;
	//check if there is a buffer already for a file descriptor

	// if not, add to the buffer map

	// if there is already a buffer copy till the first \n char into line var
		// shift the remaining amount in the buffer to the front
		// note the end of the data in the buffer and fill in the rest with a fresh read

	//Keep buffer full
	//and copy data from the buffer
	//refill buffer
	//copy the rest of the line
	//refill and copy till the line is read
	//refill
	//return data

	//linked map
		//add(key, value)
		//remove(key)
		//get(key)
}
