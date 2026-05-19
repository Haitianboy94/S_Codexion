/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:02 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 16:52:08 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int dongle_init(t_dongle *d, int id)
{
	// pthread_mutex_init + pthread_cond_init + heap_init
    d->id = id;
    if (pthread_mutex_init(&d->mutex, NULL) != 0)
        return(-1);
    if (pthread_cond_init(&d->cond, NULL) != 0)
    {
        pthread_mutex_destroy(&d->mutex);
        return(-1);
    }
    if (heap_init(&d->queue, 64) != 0)
    {
        pthread_cond_destroy(&d->cond);
        pthread_mutex_destroy(&d->mutex);
        return(-1);
    }
    d->available_at_ms = 0;
    d->held_by = -1;
    return(0);
}

void    dongle_destroy(t_dongle *d)
{
	heap_free(&d->queue);
    pthread_cond_destroy(&d->cond);
    pthread_mutex_destroy(&d->mutex);
}

void dongle_take(t_dongle *d, t_coder *coder, long priority)
{
	// Blocks until dongle is free, cooldown elapsed, AND coder is at
    // the head of the scheduling queue.Uses pthread_cond_timedwait for deadline-aware waiting.
    // Prints "X has taken a dongle" via logger.
    struct timespec ts;

    pthread_mutex_lock(&d->mutex);
    heap_push(&d->queue, (t_heapnode){coder->id, priority});
    while (d->held_by != -1
        || now_ms() < d->available_at_ms
        || heap_peek(&d->queue).coder_id != coder->id)
    {
        if (coder->sim->stop_flag)
        {
            if (d->queue.size > 0)
                heap_pop(&d->queue);
            pthread_mutex_unlock(&d->mutex);
            return ;
        }
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_nsec += 1000000;
        if (ts.tv_nsec >= 1000000000)
        {
            ts.tv_sec += 1;
            ts.tv_nsec -= 1000000000;
        }
        pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
    }
    d->held_by = coder->id;
    sched_next(d);
    log_event(coder->sim, coder->id, PURPLE "has taken a dongle" RESET);
    pthread_mutex_unlock(&d->mutex);
}    

void    dongle_put(t_dongle *d, t_coder *coder)
{
	// Releases dongle, records release time for cooldown,
    // calls pthread_cond_broadcast to wake waiters.
    pthread_mutex_lock(&d->mutex);
    d->held_by = -1;
    d->available_at_ms = now_ms() + coder->sim->args.dongle_cooldown;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}
