/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:08 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:56:24 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    now_ms(void)
{
	// Returns current wall-clock time in milliseconds (gettimeofday wrapper)
}
    

long    elapsed_ms(long since_ms)
{
	// Returns now_ms() - since_ms
}
    

void    sleep_ms(long ms)
{
	// Wraps usleep(); clamps to 0 if ms <= 0
}
