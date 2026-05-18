/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:11 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 12:04:41 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    log_init(t_sim *sim)
{
	// Initialises sim->log_mutex
	pthread_mutex_init(&sim->log_mutex, NULL);
}

void    log_destroy(t_sim *sim)
{
	pthread_mutex_destroy(&sim->log_mutex);
}

void    log_event(t_sim *sim, int coder_id, const char *msg)
{
	// Locks log_mutex, writes "timestamp_ms ID msg\n", unlocks.
    // Uses write() for async-signal safety.
	long start_time;
	char *strnumber;
	char *str_coder_id;

	pthread_mutex_lock(&sim->log_mutex);
	start_time = now_ms() - sim->start_ms;
	strnumber = ft_itoa(start_time);
	str_coder_id = ft_itoa(coder_id);
	write(2, strnumber, strlen(strnumber));
	write(2, " ", 1);
	write(2, str_coder_id, strlen(str_coder_id));
	write(2, " ", 1);
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
	free(strnumber);
	free(str_coder_id);
	pthread_mutex_unlock(&sim->log_mutex);
}
