/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:11 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:48:26 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    log_init(t_sim *sim)
{
	 // Initialises sim->log_mutex
}

void    log_destroy(t_sim *sim)
{
	
}

void    log_event(t_sim *sim, int coder_id, const char *msg)
{
	// Locks log_mutex, writes "timestamp_ms ID msg\n", unlocks.
    // Uses write() for async-signal safety.
}
