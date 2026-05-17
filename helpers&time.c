/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:08 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/12 11:06:42 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    now_ms(void)
{
	// Returns current wall-clock time in milliseconds (gettimeofday wrapper)
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
    

long    elapsed_ms(long since_ms)
{
	// Returns now_ms() - since_ms
	return (now_ms() - since_ms);
}

void    sleep_ms(long ms)
{
	// Wraps usleep(); clamps to 0 if ms <= 0
	long convert;
	
	if (ms <= 0)
		return;
	convert = ms * 1000;
	usleep(convert);
}

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}