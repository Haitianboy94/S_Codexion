/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_time.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:08 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 10:27:59 by rulouis          ###   ########.fr       */
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