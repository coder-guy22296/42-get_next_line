/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/29 11:33:13 by cyildiri          #+#    #+#             */
/*   Updated: 2016/10/29 11:33:15 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void usestackmem()
{
	char str[90000];
	int i = 0;
	while (i < 90000)
	{
		str[i] = 'z';
		i++;
	}
}

int main()
{
	int fd = open("testing.txt", O_RDONLY);
	char *str;
	int i = 1;

	while (i == 1)
	{
		//str = 0;
		printf("==================================\n");
		i = get_next_line(fd, &str);
		printf("==================================\n");
		printf("returned: %d output: %s\n", i, str);
		usestackmem();
		printf("REKTD the STACK\n");
		free(str);
	}
}