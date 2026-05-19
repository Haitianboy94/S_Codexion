/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:11 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 13:29:01 by rulouis          ###   ########.fr       */
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

	if (sim->stop_flag && strcmp(msg, "burned out") != 0)
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
