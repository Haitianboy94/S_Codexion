/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:02 by rulouis           #+#    #+#             */
/*   Updated: 2026/06/01 16:34:35 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_init(t_dongle *d, int id)
{
	d->id = id;
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
		return (-1);
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&d->mutex);
		return (-1);
	}
	if (heap_init(&d->queue, 64) != 0)
	{
		pthread_cond_destroy(&d->cond);
		pthread_mutex_destroy(&d->mutex);
		return (-1);
	}
	d->available_at_ms = 0;
	d->held_by = -1;
	return (0);
}

void	dongle_destroy(t_dongle *d)
{
	heap_free(&d->queue);
	pthread_cond_destroy(&d->cond);
	pthread_mutex_destroy(&d->mutex);
}

static int	dongle_try_acquire(t_dongle *d, t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&d->mutex);
	if (dongle_ready(d, coder))
	{
		d->held_by = coder->id;
		sched_next(d);
		pthread_mutex_unlock(&d->mutex);
		log_event(coder->sim, coder->id, YELLOW "has taken a dongle" RESET);
		return (1);
	}
	ts = next_timeout();
	pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
	pthread_mutex_unlock(&d->mutex);
	return (0);
}

void	dongle_take(t_dongle *d, t_coder *coder, long priority)
{
	pthread_mutex_lock(&d->mutex);
	heap_push(&d->queue, (t_heapnode){coder->id, priority});
	pthread_mutex_unlock(&d->mutex);
	while (1)
	{
		if (sim_is_stopped(coder->sim))
			return (dongle_cleanup(d));
		if (dongle_try_acquire(d, coder))
			return ;
	}
}

void	dongle_put(t_dongle *d, t_coder *coder)
{
	pthread_mutex_lock(&d->mutex);
	d->held_by = -1;
	d->available_at_ms = now_ms() + coder->sim->args.dongle_cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
