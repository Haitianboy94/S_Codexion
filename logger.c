/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:11 by rulouis           #+#    #+#             */
/*   Updated: 2026/06/01 16:41:37 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_init(t_sim *sim)
{
	pthread_mutex_init(&sim->log_mutex, NULL);
}

void	log_destroy(t_sim *sim)
{
	pthread_mutex_destroy(&sim->log_mutex);
}

void	log_event(t_sim *sim, int coder_id, const char *msg)
{
	long	timestamp;
	char	*str_time;
	char	*str_id;

	if (sim_is_stopped(sim) && strcmp(msg, BURNED "burned out" RESET) != 0)
		return ;
	pthread_mutex_lock(&sim->log_mutex);
	timestamp = now_ms() - sim->start_ms;
	str_time = ft_itoa(timestamp);
	str_id = ft_itoa(coder_id);
	write(1, str_time, strlen(str_time));
	write(1, " ", 1);
	write(1, str_id, strlen(str_id));
	write(1, " ", 1);
	write(1, msg, strlen(msg));
	write(1, "\n", 1);
	free(str_time);
	free(str_id);
	pthread_mutex_unlock(&sim->log_mutex);
}

struct timespec	next_timeout(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += 1000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += 1;
		ts.tv_nsec -= 1000000000;
	}
	return (ts);
}
