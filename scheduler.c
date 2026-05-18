/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:18 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 10:58:11 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	sched_priority(const t_sim *sim, int coder_id)
{
	// FIFO → now_ms() at request time
    // EDF  → coder->last_compile_start_ms + args.time_to_burnout
	if (sim->args.scheduler == FIFO)
		return(now_ms());
	else if (sim->args.scheduler == EDF)
		return(sim->coders[coder_id].last_compile_start_ms 
			+ sim->args.time_to_burnout);
	return(now_ms());
}

int	sched_request(t_dongle *d, int coder_id, long priority)
{
	// Push a request onto d->queue (caller must hold d->mutex)
	return heap_push(&d->queue, (t_heapnode){coder_id, priority});
}

int	sched_next(t_dongle *d)
{
	t_heapnode node;
	// Pop the highest-priority coder_id from queue (-1 if empty)
	if (d->queue.size == 0)
		return(-1);
	node = heap_pop(&d->queue);
	return(node.coder_id);
}
