/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:02 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/11 12:41:22 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int dongle_init(t_dongle *d, int id)
{
	// pthread_mutex_init + pthread_cond_init + heap_init
}

void    dongle_destroy(t_dongle *d)
{
	
}

void    dongle_take(t_dongle *d, t_coder *coder)
{
	// Blocks until dongle is free, cooldown elapsed, AND coder is at
    // the head of the scheduling queue.
    // Uses pthread_cond_timedwait for deadline-aware waiting.
    // Prints "X has taken a dongle" via logger.
}    

void    dongle_put(t_dongle *d, t_coder *coder)
{
	// Releases dongle, records release time for cooldown,
    // calls pthread_cond_broadcast to wake waiters.
}
